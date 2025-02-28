#include "main.h"
#include "autons.hpp"
#include "pros/colors.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

using namespace pros;

// Chassis constructor
ez::Drive chassis(
  {-7, -12},  // Right Chassis Ports (negative port will reverse it!)
  {8, 10}, 
  19,        // IMU Port
  3.25,      // Wheel Diameter
  360
); 

bool isMacroFish = true;
int intakeDirection = 1;
bool isSort = false;
int toggle = 1;

void liftControl() {
  fish.move(80);
  pros::delay(550);
  fish.move(-80);
  pros::delay(550);
  fish.move(0);
}

void sortControl() {
  colorSensor.set_led_pwm(100);
  if (colorSensor.get_proximity() >= 60) {
  if (colorSensor.get_hue() >= 180 && colorSensor.get_hue() <= 220) {
    pros::delay(50);
    if (robotColor != Blue) intakeDirection = -1;
    pros::delay(300);
    intakeDirection = 1;
  } else if (colorSensor.get_hue() >= 7 && colorSensor.get_hue() <= 20) {
    pros::delay(50);
    if (robotColor != Red) intakeDirection = -1;
    pros::delay(150);
    intakeDirection = 1;
  } else {
    intakeDirection = 1;
  }
  } else {
  intakeDirection = 1;
  }
}

void initialize() {
  pros::Task sortTask([]{
  while (true) {
    if (isSort) {
    sortControl();
    }
    pros::delay(10);
  }
  });

  ez::ez_template_print();
  pros::delay(500);  
  chassis.opcontrol_curve_buttons_toggle(false);
  chassis.opcontrol_drive_activebrake_set(0.0);
  chassis.opcontrol_curve_default_set(0.0, 0.0);
  default_constants();

  ez::as::auton_selector.autons_add({
  // {"Partial Red AWP\n\nScores 5 rings, 1 alliance, and 4 mogo", partialRedLeftAWP},
  // {"Red Right AWP\n\nScores 1 ring", soloRedRightAWP},
  // {"Solo Blue Left AWP\n\nScores 1 ring", soloBlueLeftAWP},
  // {"Solo Blue Right AWP\n\nScores 1 ring", soloBlueRightAWP},
  // {"Red Left AWP\n\nScores 1 ring", soloRedLeftAWP},
  {"Skills\n\nScores 1 ring", skills},
  });

  chassis.initialize();
  ez::as::initialize();
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
}

void disabled() {}

void competition_initialize() {}

void spinIntake(int voltage) {
  intake.move(voltage);
  secondStage.move(voltage);
}

void clampMogo() {
  toggle *= -1;
  if (toggle == 1) {
  mogoClamp.retract();
  } else {
  mogoClamp.extend();
  }
  delay(250);
}

void autonomous() {
  chassis.pid_targets_reset();
  chassis.drive_imu_reset();
  chassis.drive_sensor_reset();
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
  ez::as::auton_selector.selected_auton_call();
}

void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  if (tracker != nullptr) {
  tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());
  tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());
  }
  ez::screen_print(tracker_value + tracker_width, line);
}

void ez_screen_task() {
  while (true) {
  if (!pros::competition::is_connected()) {
    if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
    if (ez::as::page_blank_is_on(0)) {
      ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
               "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
               "\na: " + util::to_string_with_precision(chassis.odom_theta_get()), 1);
      screen_print_tracker(chassis.odom_tracker_left, "l", 4);
      screen_print_tracker(chassis.odom_tracker_right, "r", 5);
      screen_print_tracker(chassis.odom_tracker_back, "b", 6);
      screen_print_tracker(chassis.odom_tracker_front, "f", 7);
    }
    }
  } else {
    if (ez::as::page_blank_amount() > 0)
    ez::as::page_blank_remove_all();
  }
  pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task ezScreenTask(ez_screen_task);

void ez_template_extras() {
  if (!pros::competition::is_connected()) {
  if (master.get_digital_new_press(DIGITAL_X))
    chassis.pid_tuner_toggle();

  if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
    pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
    autonomous();
    chassis.drive_brake_set(preference);
  }

  chassis.pid_tuner_iterate();
  } else {
  if (chassis.pid_tuner_enabled())
    chassis.pid_tuner_disable();
  }
}

void opcontrol() {
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  while (true) {
  ez_template_extras();
  chassis.opcontrol_tank();

  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) clampMogo();
  if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) {
    if(isMacroFish) pros::Task liftControlTask([]{ liftControl(); pros::delay(10); });
  }
  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) intakeLift.toggle();
  if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    intake.move(127 * intakeDirection);
    secondStage.move(127);
  } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    intake.move(-127);
    secondStage.move(-127);
  } else {
    spinIntake(0);
    secondStage.move(0);
  }

  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
    isSort = !isSort;
    master.rumble(isSort ? ".." : ".");
  }

  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    if (robotColor == None) {
    robotColor = Blue;
    master.rumble("_._.");
    } else if (robotColor == Blue) {
    robotColor = Red;
    master.rumble("._.");
    } else if (robotColor == Red) {
    robotColor = None;
    master.rumble(".");
    }
  }

  if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
    isMacroFish = !isMacroFish;
    master.rumble(isMacroFish ? ".." : ".");
  }

  pros::delay(ez::util::DELAY_TIME);
  }
}

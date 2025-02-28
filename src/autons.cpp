#include "main.h"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 110;

///
// Constants
///
void default_constants() {
  // P, I, D, and Start I
  chassis.pid_drive_constants_set(20.0, 0.0, 100.0);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(3.0, 0.05, 20.0, 15.0);     // Turn in place constants
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // Exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

void partialRedLeftAWP() {
  fish.move(127);
  pros::delay(400);
  fish.move(-127);
  pros::delay(400);
  fish.move(0);

  chassis.pid_drive_set(-34_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-19_in, 80);
  chassis.pid_wait();
  pros::delay(150);
  mogoClamp.toggle();
  pros::delay(200);
  chassis.pid_turn_set(140, TURN_SPEED);
  chassis.pid_wait();
  intakeLift.toggle();
  intake.move(127);
  secondStage.move(127); 
  chassis.pid_drive_set(30_in, 127);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-30_in, 127);
  chassis.pid_wait();
  chassis.drive_angle_set(0);
  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(23_in, 127);
  chassis.pid_wait();
  pros::delay(250);
  chassis.pid_turn_set(30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(5_in, 127);
  chassis.pid_wait();
  pros::delay(300);
  chassis.pid_drive_set(-5_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(225_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeLift.toggle();
  chassis.pid_drive_set(44_in, 127);
  chassis.pid_wait();
  intakeLift.toggle();
  pros::delay(300);
  chassis.pid_drive_set(-5_in, 127);
  chassis.pid_wait();
  intake.move(0);
  secondStage.move(0);
}


void soloRedRightAWP() {
  fish.move(127);
  pros::delay(400);
  fish.move(-127);
  pros::delay(400);
  fish.move(0);

  chassis.pid_drive_set(-32_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-20_in, 80);
  chassis.pid_wait();
  pros::delay(150);
  mogoClamp.toggle();
  pros::delay(200);
  chassis.pid_turn_set(-140, TURN_SPEED);
  chassis.pid_wait();
  intakeLift.toggle();
  intake.move(127);
  secondStage.move(127);
  chassis.pid_drive_set(27_in, 70);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-27_in, 70);
  chassis.pid_wait();
  chassis.pid_turn_set(-360_deg, TURN_SPEED);
  chassis.pid_wait();
  intakeLift.toggle();
  chassis.pid_drive_set(38_in, 70);
  chassis.pid_wait();
  intakeLift.toggle();
  pros::delay(300);
  chassis.pid_drive_set(-15_in, 127);
  mogoClamp.toggle();
}

void soloBlueLeftAWP() {
  fish.move(127);
  pros::delay(500);
  fish.move(-127);
}

void soloBlueRightAWP() {
  fish.move(127);
  pros::delay(500);
  fish.move(-127);
}

void skills() {
  fish.move(127);
  pros::delay(300);
  fish.move(-127);
  pros::delay(300);
  fish.move(0);
  chassis.pid_drive_set(-10_in, 127);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-16_in, 70);
  chassis.pid_wait();
  pros::delay(150);
  mogoClamp.toggle();
  pros::delay(500);
  chassis.drive_angle_set(0);
  intakeLift.toggle();
  intake.move(127);
  secondStage.move(127);
  chassis.pid_turn_set(180, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(42_in, 80);
  chassis.pid_wait();
  chassis.pid_drive_set(-12_in, 80);
  chassis.pid_wait();
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(23_in, 80);
  chassis.pid_wait();
  chassis.drive_angle_set(0);
  chassis.pid_turn_set(270, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(10_in, 80);
  chassis.pid_wait();
  chassis.drive_angle_set(0);
  chassis.pid_turn_set(60+180, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(48_in, 80);
  chassis.pid_wait();
  chassis.pid_drive_set(-5_in, 80);
  chassis.pid_wait();
  chassis.drive_angle_set(0);
  chassis.pid_turn_set(180, 127);
  chassis.pid_wait();
  chassis.pid_drive_set(-9_in, 80);
  chassis.pid_wait();
  pros::delay(500);
  mogoClamp.toggle();

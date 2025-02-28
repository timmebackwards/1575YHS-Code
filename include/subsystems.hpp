#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"
#include "pros/colors.hpp"
#include "pros/optical.hpp"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');
inline pros::adi::Pneumatics mogoClamp('H', false);
inline pros::adi::Pneumatics intakeLift('G', true);
inline pros::Motor intake(11, pros::MotorGearset::blue);
inline pros::Motor secondStage(-9);
inline pros::Motor fish(15);
inline pros::Rotation fishRotation(2);
inline pros::GPS gpsSensor(-1);
inline pros::Optical colorSensor(13);

enum Colors 
{
    None,
    Red, 
    Blue
};

inline Colors robotColor = Red;
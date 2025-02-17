#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');
inline pros::adi::Pneumatics mogoClamp('H', false);
inline pros::adi::Pneumatics intakeLift('G', true);
inline pros::MotorGroup intake({11}, pros::MotorGearset::blue);
inline pros::Motor secondStage(-20);
inline pros::Motor fish(15);
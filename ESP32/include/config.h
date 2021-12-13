#include <secrets.h>

// DC Motor (LN293D)
const int    motorEN1              = 25;
const int    motorIN1A             = 26;
const int    motorIN1B             = 27;

// Defaults
const String defaultControlMode    = "automatic";
const String defaultMotorDirection = "cw";
const bool   defaultMotorStatus    = true;
const int    defaultMotorSpeed     = 255;
const int    defaultTurnsCycles    = 12;
const int    defaultTurnsPerDay    = 640;
const int    defaultTurnsPerHour   = 54;

// PWM Settings
// Note: pwmSpeed is a map from input to the dutyCycle: map(pwmSpeedMin, pwmSpeedMax, 170, 255)
const int    freq                  = 30000;
const int    resolution            = 8;
const int    pwmChannel            = 0;
const int    pwmSpeedMin           = 0;
const int    pwmSpeedMax           = 100; 
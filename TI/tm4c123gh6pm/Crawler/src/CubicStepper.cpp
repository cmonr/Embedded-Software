#include "CubicStepper.h"

CubicStepper::CubicStepper(float p0, float p1, float steps) {
  a0 = p0;
  a1 = 0;
  a3 = (p1 - p0) / (-.5 * steps * steps * steps);
  a2 = -3 * a3 * steps / 2.0;
} 

float CubicStepper::f(float t) {
  return a0 + (a1 * t) + (a2 * t * t) + (a3 * t * t * t);
}


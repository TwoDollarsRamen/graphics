#include "maths.h"

f64 todeg(f64 rad) {
	return rad * (180.0f / pi_d);
}

f64 torad(f64 deg) {
	return deg * (pi_d / 180.0f);
}

f32 todegf(f32 rad) {
	return rad * (180.0f / pi_f);
}

f32 toradf(f32 deg) {
	return deg * (pi_f / 180.0f);
}

#include "../../include/utils/Utils.h"

int math::clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

float math::clamp(float min, float val, float max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
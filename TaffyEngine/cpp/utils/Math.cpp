#include "../../include/utils/Math.h"
#include <sstream>
#include <iomanip>

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

std::string math::display_sig(double value, int significant)
{
	std::ostringstream string;
	string << std::setprecision(significant) << value;

	return string.str();
}

std::string display(double value, int round) {

	std::ostringstream string;
	string << std::fixed << std::setprecision(round) << value;

	return string.str();
}
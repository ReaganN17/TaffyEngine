#pragma once

#include <string>

//Math Utilities
struct math final {
	static int clamp(int min, int val, int max);
	static float clamp(float min, float val, float max);

	static std::string display_round(double val, int round);
	static std::string display_sig(double val, int significant);

private:

	math() = delete;
};
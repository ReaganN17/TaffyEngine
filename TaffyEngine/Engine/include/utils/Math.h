#pragma once

//Math Utilities
struct math final {
	static int clamp(int min, int val, int max);
	static float clamp(float min, float val, float max);
};
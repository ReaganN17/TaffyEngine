inline int clamp(int min, int val, int max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

inline float clampF(float min, float val, float max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

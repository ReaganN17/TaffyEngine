#include "../../include/utils/Utils.h"

Shader::Shader(u8 opacity, u32 shade_color, u8 shade_scale) : opacity(opacity), shade_color(shade_color), shade_scale(shade_scale) {}

Shader::Shader() {
	opacity = 255;
	shade_color = 0x000000;
	shade_scale = 0;
}
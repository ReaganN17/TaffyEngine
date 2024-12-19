#pragma once

#include "../utils/Utils.h"
struct Shader final {
	u32 shade_color;
	u8 shade_scale, opacity;
	Shader();
	Shader(u8 opac, u32 shade_color, u8 shade_scale);
};

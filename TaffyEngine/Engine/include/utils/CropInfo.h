#pragma once

#include "../utils/Utils.h"

struct CropInfo final {
	u16 cx = 0;
	u16 cy = 0;
	u16 cw = 0;
	u16 ch = 0;

	CropInfo();
	CropInfo(u16 cx, u16 cy, u16 cw, u16 ch);
};
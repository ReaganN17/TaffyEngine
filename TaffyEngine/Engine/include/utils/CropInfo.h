#pragma once

#include "../utils/Utils.h"

struct CropInfo final {
	u16 cx;
	u16 cy;
	u16 cw;
	u16 ch;

	CropInfo();
	CropInfo(u16 cx, u16 cy, u16 cw, u16 ch);
};
#include "../../include/utils/Utils.h"

CropInfo::CropInfo() : cx(0), cy(0), cw(0), ch(0) {}

CropInfo::CropInfo(u16 x, u16 y, u16 w, u16 h) : cx(x), cy(y), cw(w), ch(h) {}
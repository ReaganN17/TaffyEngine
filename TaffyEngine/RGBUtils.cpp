struct RGB {
	u8 r;
	u8 g;
	u8 b;

	RGB& set(u8 red, u8 gre, u8 blu) {
		r = red;
		g = gre;
		b = blu;

		return *this;
	}
};

u32 RGBToHex(RGB rgb) {
	return (u32)((rgb.r & 0xff) << 16) + ((rgb.g & 0xff) << 8) + (rgb.b & 0xff);
}

u32 rgbToHex(u8 r, u8 g, u8 b) {
	return (u32)((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

RGB hexToRGB(u32 hex) {
	RGB result;

	result.r = ((hex >> 16) & 0xff);  // Extract the RR byte
	result.g = ((hex >> 8) & 0xff);   // Extract the GG byte
	result.b = ((hex) & 0xff);        // Extract the BB byte

	return result;
}
	
u32 alphaPerPixel(u32 back, u32 orig, float opac) {
	opac = clampF(0, opac, 1);

	RGB backRGB = hexToRGB(back);
	RGB origRGB = hexToRGB(orig);

	RGB result;

	result.r = opac * (origRGB.r - backRGB.r) + backRGB.r;
	result.g = opac * (origRGB.g - backRGB.g) + backRGB.g;
	result.b = opac * (origRGB.b - backRGB.b) + backRGB.b;

	return RGBToHex(result);
}


#define OPAQUE 0xFF;
#define TRANSPARENT 0;

#define ALPHA(rgb) (uint8_t)(rgb >> 24)
#define RED(rgb)   (uint8_t)(rgb >> 16)
#define GREEN(rgb) (uint8_t)(rgb >> 8)
#define BLUE(rgb)  (uint8_t)(rgb)

#define UNMULTIPLY(color, alpha) ((0xFF * color) / alpha)
#define BLEND(back, front, alpha) (u8) (((front * alpha) + (back * (255 - alpha))) / 255)


struct RGBA {
	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 a = 255;

	RGBA(u8 r, u8 g, u8 b, u8 a = 255) : r(r), g(g), b(b), a(a){}

	RGBA(u32 hex) {
		a = ALPHA(hex);
		r = RED(hex);
		g = GREEN(hex);
		b = BLUE(hex);
	}

	RGBA& set(u8 r, u8 b, u8 g, u8 a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		return *this;
	}

	RGBA& set(u32 hex) {
		this->a = ALPHA(hex);
		this->r = RED(hex);
		this->g = GREEN(hex);
		this->b = BLUE(hex);

		return *this;
	}

	//hex without A value
	u32 toHexNoA() {
		return (a << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
	}

	//either transparent or opaque
	u32 toHexOneA(u32 back) {
		if (a == 0) { return back; }
		return toHexNoA();
	}

	//hex with A value 0 - 255, laggy for big objects
	u32 toHexWithA(u32 back, u8 opac = 255) {
		return ((BLEND(RED(back), r, a * opac / 255) & 0xFF) << 16) | ((BLEND(GREEN(back), g, a * opac / 255) & 0xFF) << 8) | (BLEND(BLUE(back), b, a * opac / 255) & 0xFF);
	}
};
enum ButtonType {
	STICK, PULSE,
};

enum Shape {
	CIRCLE, RECTA,
};

//There is a better way for this and it is to make more class inheriters of different types of buttons which i will do if i feel like it
enum HoverType {
	GLOW, EXPAND,
};

enum SelectType {
	SHRINK, DARKEN,
};


struct Button : Object {
	bool select = false;
	bool hover = false;

	ButtonType bType = PULSE;
	HoverType hType = EXPAND;
	SelectType sType = DARKEN;
	Shape shType = RECTA;

	Button(float x, float y, float w, float h, const char* filename, zLayer z);
	Button(float x, float y, float w, float h, u32 color, float alpha, zLayer z);

	Button& settings(ButtonType b, HoverType h, SelectType s, Shape sh);

	virtual void render();
	bool isMouseOver(POINT mouse);
};

//long ass constructor cause i dont feel like being neat
Button::Button(float x, float y, float w, float h, const char* filename, zLayer z = MIDDLE) : Object(x, y, w, h, filename, z) {}

Button::Button(float x, float y, float w, float h, u32 color, float alpha, zLayer z = MIDDLE) : Object(x, y, w, h, color, alpha, z) {}

Button& Button::settings(ButtonType b, HoverType h, SelectType s, Shape sh) {
	bType = b;
	hType = h;
	sType = s;
	shType = sh;
	return *this;
}


//UI Buttons coordinates are different from game coordinates but still have the same proportiions
//to render all objects required camera parameter but we dont use it for buttons
//if i get lazy enough im gonna combine them into one struct
void Button::render() {
	hover = isMouseOver(mouse);

	if (isImg()) {
		if (hover) {
			renderImage(file, x, y, w * 1.3, h * 1.3, alpha);
		}
		else if (select) {
			renderImage(file, x, y, w, h, alpha);
		}
		else {
			renderImage(file, x, y, w, h, alpha);
		}
	}
	else {
		renderRect(x, y, w / 2, h / 2, color, alpha);
	}
}

bool Button::isMouseOver(POINT mouse) {
	switch (shType) {
	case RECTA: {
		return (mouse.x >= x - w / 2 && mouse.x <= x + w / 2) && (mouse.y >= y - h / 2 && mouse.y <= y + h / 2);
	}break;
	case CIRCLE: {
		return (4 * (mouse.x * mouse.x) / (w * w) + 4 * (mouse.y * mouse.y) / (h * h)) <= 1;
	}break;
	}
}

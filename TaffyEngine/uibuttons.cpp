struct Button : Object {
	bool down = false;
	bool pressed = false;
	bool released = false;
	bool hover = false;
	bool stick = false;
	bool circle = false;

	//only purpose is to be a switch for the pulse button so you cant click it if ur key was down before hovering
	//god give me a better way to do this
	bool erm = false;

	Button(int x, int y, int w, int h, const char* filename, zLayer z, bool stick, bool circle);
	Button(int x, int y, int w, int h, u32 color, u8 alpha, zLayer z, bool stick, bool circle);
	Button();

	~Button();

	Button& update(Inputs key);

	virtual void render();
	bool isMouseOver();
};

Button::~Button() {}

Button::Button() {}

//long ass constructor cause i dont feel like being neat
Button::Button(int x, int y, int w, int h, const char* filename, zLayer z = MIDDLE, bool stick = false, bool circle = false) :
	Object(x, y, w, h, filename, z), stick(stick), circle(circle) {}

Button::Button(int x, int y, int w, int h, u32 color, u8 alpha, zLayer z = MIDDLE, bool stick = false, bool circle = false) :
	Object(x, y, w, h, color, alpha, z), stick(stick), circle(circle) {}

//placed in the render function so it updates with the game loop
Button& Button::update(Inputs key) {
	pressed = false;
	released = false;

	hover = isMouseOver();

	if (stick) {
		if (hover && pressed(key) && !down) {
			pressed = true;
			down = true;
		} else if (hover && pressed(key) && down) {
			released = true;
			down = false;
		}
	}
	else {
		down = false;
		if (hover && pressed(key)) {
			pressed = true;
			erm = true;
		}
		if (erm && hover && isdown(key)) {
			down = true;
		}
		if (erm && hover && released(key)) {
			released = true;
			erm = false;
		}
		if (!hover) {
			erm = false;
		}
	}


	return *this;
}


//UI Buttons coordinates are different from game coordinates but still have the same proportiions
void Button::render() {
	update(LEFT_MOUSE);
	
	if (isImg()) {
		renderImageV2(file, x, y, (hover)?w*1.3:w, (hover) ? h * 1.3 : h, alpha, (down)?50:0);

	}
	else {
		renderRect(x, y, w / 2, h / 2, color, alpha);
	}
}

bool Button::isMouseOver() {
	if(circle) return (4 * (mouse.x * mouse.x) / (w * w) + 4 * (mouse.y * mouse.y) / (h * h)) <= 1;
	else return (mouse.x >= x - w / 2 && mouse.x <= x + w / 2) && (mouse.y >= y - h / 2 && mouse.y <= y + h / 2);
}

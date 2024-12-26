

struct LogoIntro : Event {
	Object logo;
	int opacity = 0;
	bool flip = false;

	LogoIntro() {
		
	}

	void init() {
		new (&logo) Object(0, 0, Win32Render::coord_width, Win32Render::coord_height, "Taffy_Logo", Z_MIDDLE);
		logo.setShader(0);
	}

	void loop() {
		opacity += (flip) ? -125 * Win32Window::delta_time : 125 * Win32Window::delta_time;

		if (opacity >= 255) flip = true;
		logo.setShader(math::clamp(0, opacity, 255));

		if (Input::is_pressed(VK_RETURN)) flip = true;
	}

	bool isFinished() {
		return opacity <= 0 && flip;
	}

	void end(bool interrupted) {
		logo.setShader(0);
		logo.getSprite()->unload();
		ObjectHandler::getInstance().removeObject(&logo);
	}

};
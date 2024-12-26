
struct MoveBackground : Event {
	Object bg;
	Timer timer;

	float time = 4;
	float change_time = 2;

	MoveBackground() {}

	void init() {
		new (&bg) Object(0, 0, "Moving_BG_0", Z_FARBACK);
		bg.setSize(Win32Render::coord_width * 2, Win32Render::coord_height * 2);

		bg.setPos(bg.getW() / 4, bg.getH() / 4);
		timer.start();
	}

	void loop() {
		if (timer.getTime() > change_time * 3 * 1000) { timer.start(); bg.setSprite("Moving_BG_0"); }
		else if (timer.getTime() > change_time * 2 * 1000) {bg.setSprite("Moving_BG_1");}
		else if (timer.getTime() > change_time * 1000) { bg.setSprite("Moving_BG_2"); }

		bg.setPos(
			fmod(
				bg.getX() 
				- Win32Render::coord_width * Win32Window::delta_time / time
				- Win32Render::coord_width/2, Win32Render::coord_width) + Win32Render::coord_width / 2,
			fmod(
				bg.getY() 
				- Win32Render::coord_height * Win32Window::delta_time / time 
				- Win32Render::coord_height/2, Win32Render::coord_height) + Win32Render::coord_height/2);
	}

	void end(bool interrupted) {
		ObjectHandler::getInstance().removeObject(&bg);
	
	}

	bool isFinished() {return false;}
};
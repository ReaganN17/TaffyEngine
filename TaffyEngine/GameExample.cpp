
struct GameExample {
	Object mr_test;

	void init() {
		SpriteHandler::getInstance().addSprite("test_sprite", new Sprite("Resources/bob.png"));
		new (&mr_test) Object(0, 0, "test_sprite", MIDDLE);
	}

	void play() {
		if (pressed('A')) { 
			EventHandler::getInstance().scheduleEvent(
				(new RunEvent([&]() {mr_test.setPos(mr_test.getX() + 20 * Win32Window::delta_time, mr_test.getY()); }, { &mr_test }))->
				onlyWhile ([]() {return isdown('D'); }));
		}


		if (isdown('W')) {
			Camera::default_camera.setZoom(Camera::default_camera.getZoom() * pow(1.1, Win32Window::delta_time));
		}
		if (isdown('S')) {
			Camera::default_camera.setZoom(Camera::default_camera.getZoom() * pow(0.9, Win32Window::delta_time));
		}
		if (pressed(VK_ESCAPE)) { Win32Window::end(); }
	}
};
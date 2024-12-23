
struct GameExample {
	Object bg;
	Object mr_test;
	Controls& controller = Controls::getInstance();
	void configureBindings() {
		controller.addTrigger(
			new Trigger('A', WHILE_TRUE, [&]() {return
				new RunEvent([&]() {mr_test.setPos(mr_test.getX() + 20 * Win32Window::delta_time, mr_test.getY()); }, { &mr_test }); }));
	}


	void init() {
		SpriteHandler::getInstance().addSprite("test_sprite", new Sprite("Resources/bob.png"));
		SpriteHandler::getInstance().addSprite("map", new Sprite("Resources/circles.png"));
		new (&mr_test) Object(0, 0, "test_sprite", MIDDLE);
		new (&bg) Object(0, 0, "map", BACK);

		configureBindings();
	}

	void play() {
		if (pressed('Q')) { bg.~Object(); }

		if (pressed(VK_ESCAPE)) { Win32Window::end(); }
	}

	
};
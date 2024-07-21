struct Level1Event : BasicEvent {
	Level1Event() {}
	~Level1Event() {}
	Level1Event(BasicEvent** pointer) : BasicEvent(pointer) {}

	Grid grid;
	Player bob;
	BasicEnemy rah;
	PlayerUI huh;
	Object test;

	void init() {
		new (&grid) Grid(-480, 270, 50, "resources/testrender.png", 0, 0, 1, "resources/testrender.png");
		new (&bob) Player(&grid, 1, 1, &globalSpriteSheet);
		new (&rah) BasicEnemy(&grid, 5, 5, MIDDLE, 3);
		new (&huh) PlayerUI(&globalSpriteSheet);

		new (&test) Object(0, 0, 50, 50, (u32)0, FRONT);
	}

	void loop() {
		if (!eb.running) return;

		if (pressed(W)) { test.animateToPos(bob.x, bob.y, test.w, test.h, 2, false); }


		bob.control(controls);
		mainCam.setPos(bob.x, bob.y);
		huh.update(&bob);

		if (pressed(ESC)) { running = false; }


		if (bob.health <= 0) {
			bob.destroyObject();
		}


	}

	
};
//intro screen event
struct IntroEvent : BasicEvent {

	IntroEvent() {}
	~IntroEvent() {}
	IntroEvent(BasicEvent** pointer) : BasicEvent(pointer) {}

	short alpha = 0;
	bool into = true;

	void init() {
		alpha = 0;
		into = true;
	}

	void loop() {
		if (!eb.running) return;
		if (eb.interupt) { interrupt(); return; }


		if (into) alpha += 200 * dt;
		if (!into) alpha -= 200 * dt;

		if (alpha >= 255) { into = false; }
		
		if (pressed(ENTER)) { into = false; }

		renderStaticBG("resources/taffyEngine.png", clamp(0, alpha, 255), 0);

		if (alpha < 0) {
			currentScreen = MAINMENU;  
			end();
		}
	}
};

//custom background
struct CustomBG : BasicEvent {
	CustomBG() {}
	~CustomBG() {}
	CustomBG(BasicEvent** pointer) : BasicEvent(pointer) {}

	float customBGx = 0, customBGy = 0, change = 0;
	u8 shade = 0;

	void init() {
		customBGx = 0, customBGy = 0, change = 0;
	}

	void loop() {
		if (eb.running) {
			customBGx -= 960 * dt * 0.2;
			customBGy -= 540 * dt * 0.2;
			change += 1 * dt;
		}

		if (change >= 3) change -= 3;
		if (customBGx <= -960) customBGx += 960;
		if (customBGy <= -540) customBGy += 540;

		switch ((int)change % 3) {
		case 0: { renderMovingBG("resources/triangles.png", customBGx + 480, customBGy + 270, 2, shade); } break;
		case 1: { renderMovingBG("resources/circles.png", customBGx + 480, customBGy + 270, 2, shade); } break;
		case 2: { renderMovingBG("resources/squares.png", customBGx + 480, customBGy + 270, 2, shade); } break;
		}
	}
};

struct Transition : BasicEvent {
	Transition() {}
	Transition(BasicEvent** pointer, bool down) : BasicEvent(pointer) { this->down = down; }
	~Transition() {}

	Object guh;

	bool down = true;
	float y;
	float rate;
	float acc;

	void init() {
		acc = 0;

		y = (down) ? 700 : 0;
		rate = (down) ? -700 : 700;

		new (&guh) Object(0, y, 960, 540, (u32)0, FARFRONT);
		guh.ob.cameraLinked = false;
	}

	void loop() {
		if (!eb.running) return;

		y += rate * dt + 0.5 * acc * dt * dt;
		rate += acc * dt;

		guh.setPos(0, clamp(0, y, 700));

		if ((y < 0 && down) || (y > 700 && !down)) { end(); }
	}
};

struct ExitS : BasicEvent {
	ExitS() {}
	ExitS(BasicEvent** pointer) : BasicEvent(pointer) {}
	~ExitS() {}

	UIObject ExitBox;
	Button ExitYea;
	Button ExitNah;

	void init() {
		new (&ExitBox) UIObject(0, 0, 250, 150, FRONT, &globalSpriteSheet, CropInfo(31, 688, 329, 205));
		new (&ExitYea) Button(90, -50, 30, 15, FARFRONT, &globalSpriteSheet, CropInfo(374, 689, 97, 44));
		new (&ExitNah) Button(-90, -50, 30, 15, FARFRONT, &globalSpriteSheet, CropInfo(375, 750, 96, 48));
	}

	void loop() {
		if (pressed(ESC) || ExitNah.bb.pressed) { end(); }
		if (ExitYea.bb.pressed) { running = false; }

		ExitBox.update();
		ExitYea.update();
		ExitNah.update();
	}

	void end() {
		ExitYea.destroyObject();
		ExitNah.destroyObject();
		ExitBox.destroyObject();

		BasicEvent::end();
	}
};

struct MainMenuS : BasicEvent {
	MainMenuS() {}
	MainMenuS(BasicEvent** pointer, CustomBG* bg) : BasicEvent(pointer) { this->bg = bg; }
	~MainMenuS() {}

	CustomBG* bg;

	BasicEvent* exitevent;

	Button LevelSelect;
	Button Options;
	Button Controls;
	Button Credits;
	
	UIObject Title;

	bool pause = false;

	void init() {
		new (&LevelSelect) Button(-160, -50, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(276, 559, 175, 51));
		new (&Options) Button(160, -50, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(451, 559, 175, 51));
		new (&Credits) Button(160, -150, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(276, 610, 175, 51));
		new (&Controls) Button(-160, -150, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(451, 610, 175, 51));
		new (&Title) UIObject(0, 120, 350, 200, MIDDLE, &globalSpriteSheet, CropInfo(494, 691, 453, 273));

		pause = false;
	}

	void loop() {
		if (LevelSelect.bb.pressed) { endNtransfer(0); }

		if (pause && exitevent == nullptr) { dontexit(); }
		if (pressed(ESC) && !pause) { exit(); }

	}
	
	void exit() {
		pause = true;
		bg->shade = 50, bg->eb.running = false;

		LevelSelect.bb.disable = true;
		Options.bb.disable = true;
		Credits.bb.disable = true;
		Controls.bb.disable = true;

		LevelSelect.update();
		Options.update();
		Credits.update();
		Controls.update();

		LevelSelect.setShade(50);
		Options.setShade(50);
		Credits.setShade(50);
		Controls.setShade(50);
		Title.setShade(50);

		update = false;

		exitevent = new ExitS(&exitevent);
		exitevent->start();
	}

	void dontexit() {
		pause = false;
		update = true;

		bg->shade = 0, bg->eb.running = true;

		LevelSelect.bb.disable = false;
		Options.bb.disable = false;
		Credits.bb.disable = false;
		Controls.bb.disable = false;

		Title.setShade(0);
	}

	void endNtransfer(u8 s);

	void end() {  destroyAllObjects(); BasicEvent::end(); }

};

struct LevelSelectS : BasicEvent {
	LevelSelectS() {}
	LevelSelectS(BasicEvent** pointer, CustomBG* bg) : BasicEvent(pointer) { this->bg = bg; }
	~LevelSelectS() {}

	CustomBG* bg;

	BasicEvent* trans;

	Button Level1;
	Button Level2;
	Button Level3;
	Button Level4;
	Button Level5;
	Button Level6;
	Button Level7;
	Button Level8;
	Button Level9;
	Button Level10;
	
	Button Back;

	UIObject LevelSelect;

	void init() {
		//create buttons yea
		new (&Level1) Button(-320, 0, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(20, 453, 50, 50));
		new (&Level2) Button(-160, 0, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(70, 453, 50, 50));
		new (&Level3) Button(0, 0, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(120, 453, 50, 50));
		new (&Level4) Button(160, 0, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(170, 453, 50, 50));
		new (&Level5) Button(320, 0, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(220, 453, 50, 50));
		new (&Level6) Button(-320, -150, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(20, 503, 50, 50));
		new (&Level7) Button(-160, -150, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(70, 503, 50, 50));
		new (&Level8) Button(0, -150, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(120, 503, 50, 50));
		new (&Level9) Button(160, -150, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(170, 503, 50, 50));
		new (&Level10) Button(320, -150, 75, 75, MIDDLE, &globalSpriteSheet, CropInfo(220, 503, 50, 50));

		new (&Back) Button(-360, 225, 100, 50, MIDDLE, &globalSpriteSheet, CropInfo(290, 448, 141, 80));

		//reuse button as UI im really lazy
		new (&LevelSelect) UIObject(0, 150, 350, 100, MIDDLE, &globalSpriteSheet, CropInfo(276, 559, 175, 51));

		//goes down the line depending on the max level unlocked and disables the buttons above the max level unlocked
		//i think its a unique way of using switch and case system
		switch (levelUnlocked) {
		case 1: Level2.bb.disable = true; Level2.changeCrop(CropInfo(70, 553, 50, 50));
		case 2: Level3.bb.disable = true; Level3.changeCrop(CropInfo(120, 553, 50, 50));
		case 3: Level4.bb.disable = true; Level4.changeCrop(CropInfo(170, 553, 50, 50));
		case 4: Level5.bb.disable = true; Level5.changeCrop(CropInfo(220, 553, 50, 50));
		case 5: Level6.bb.disable = true; Level6.changeCrop(CropInfo(20, 603, 50, 50));
		case 6: Level7.bb.disable = true; Level7.changeCrop(CropInfo(70, 603, 50, 50));
		case 7: Level8.bb.disable = true; Level8.changeCrop(CropInfo(120, 603, 50, 50));
		case 8: Level9.bb.disable = true; Level9.changeCrop(CropInfo(170, 603, 50, 50));
		case 9: Level10.bb.disable = true; Level10.changeCrop(CropInfo(220, 603, 50, 50));
		case 10: break;
		}
	}


//tempting to create method to disable multiple buttons at once but it seems so expensive to do so
#define process(Lnum, num)\
	if (Lnum.bb.pressed) {\
	curLevel = num;\
	Level1.bb.disable = true;\
	Level2.bb.disable = true;\
	Level3.bb.disable = true;\
	Level4.bb.disable = true;\
	Level5.bb.disable = true;\
	Level6.bb.disable = true;\
	Level7.bb.disable = true;\
	Level8.bb.disable = true;\
	Level9.bb.disable = true;\
	Level10.bb.disable = true;\
	Back.bb.disable = true;\
	trans = new Transition(&trans, true);\
	trans->start();\
}
	

	void loop() {
		if (Back.bb.pressed) { endNtransfer(); }
		
		process(Level1, 1);
		process(Level2, 2);
		process(Level3, 3);
		process(Level4, 4);
		process(Level5, 5);
		process(Level6, 6);
		process(Level7, 7);
		process(Level8, 8);
		process(Level9, 9);
		process(Level10, 10);

		if (Level1.bb.disable && trans == nullptr) { end(); }
	}

	void endNtransfer() {
		*pointer = new MainMenuS(pointer, bg);

		destroyAllObjects();

		BasicEvent::endNtransfer();

		(*pointer)->start();
	}

};

void MainMenuS::endNtransfer(u8 s) {
	switch (s) {
		case 0: (*pointer) = new LevelSelectS(pointer, bg); break;
	}
	
	destroyAllObjects();

	BasicEvent::endNtransfer();

	(*pointer)->start();
}

//main menu screen
struct MainScreen : BasicEvent {
	MainScreen() {}
	MainScreen(BasicEvent** pointer) : BasicEvent(pointer) {}
	~MainScreen() {}

	CustomBG bg;
	BasicEvent* curScreen;

	void init() {
		new (&bg) CustomBG();
		curScreen = new MainMenuS(&curScreen, &bg);

		curScreen->start();
		bg.start();
	}

	void loop() {
		if (curScreen == nullptr) {currentScreen = GAME;  bg.end();  end();}
	}

	void end() { destroyAllObjects(); BasicEvent::end(); }
};
struct IntroVar {
	short alpha = 0;
	u8 sequence = 0;
};
IntroVar introVar;
internal void intro(float dt) {

	switch (introVar.sequence) {
	case 0: {
		introVar.alpha += 150 * dt;
		renderStaticBG("resources/taffyEngine.png", clamp(0, introVar.alpha, 255) , 0);
		if (introVar.alpha >= 300 || pressed(ENTER)) introVar.sequence = 2;
	} break;
	case 2: {
		introVar.alpha -= 150 * dt;
		renderStaticBG("resources/taffyEngine.png", clamp(0, introVar.alpha, 255), 0);
		
		if (introVar.alpha <= -50) introVar.sequence = 3;
	} break;
	case 3: {
		introVar.sequence = 0;
		introVar.alpha = 0;
		currentScreen = MAINMENU;
	}

	}
}

float customBGx = 0, customBGy = 0, change = 0;
internal void renderCustomBG(float dt, bool stop) {
	if (stop) { customBGx = 0, customBGy = 0, change = 0; }
	else {
		customBGx -= 960 * dt * 0.2;
		customBGy -= 540 * dt * 0.2;
		change += 1 * dt;
		if (change >= 3) change -= 3;
		if (customBGx <= -960) customBGx += 960;
		if (customBGy <= -540) customBGy += 540;
		switch ((int)change % 3) {
		case 0: {
			renderImageV2("resources/triangles.png", customBGx + 480, customBGy + 270, 1920, 1080);
		} break;
		case 1: {
			renderImageV2("resources/circles.png", customBGx + 480, customBGy + 270, 1920, 1080);
		} break;
		case 2: {
			renderImageV2("resources/squares.png", customBGx + 480, customBGy + 270, 1920, 1080);
		} break;
		}
	}
}

struct MainMenuVar {
	u8 sequence = 0;
};
MainMenuVar mmVar;

Button LevelSelect;
Button Options;
Button Controls;
Object Credits;

internal void mainMenu(float dt) {
	renderCustomBG(dt, false);
	switch (mmVar.sequence) {
	case 0: {
		renderCustomBG(dt, true);
		new (&LevelSelect) Button(-160, -50, 250, 70, "resources/levelselect.png");
		new (&Options) Button(160, -50, 250, 70, "resources/options.png");
		new (&Credits) Object(160, -150, 250, 70, "resources/credits.png");
		new (&Controls) Button(-160, -150, 250, 70, "resources/controls.png");
		mmVar.sequence = 1;
	}break;
	case 1: {
		renderAllObjects();
		if (LevelSelect.pressed) { 
			destroyAllObjects();
			mmVar.sequence = 0; 
			currentScreen = GAME;
		}
	}break;
	case 2: {
		
	} break;
	case 3: {

	} break;
	case 4: {

	} break;
	case 5: {

	} break;
	case 6: {

	} break;
	}
}
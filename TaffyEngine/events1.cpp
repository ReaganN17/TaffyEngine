// these items are more related to the game itself 
// it helps me to make the tools i need if i make the game alongside


//intro sequence
struct IntroVar {
	short alpha = 0;
	u8 sequence = 0;
};
IntroVar introVar;
internal void intro() {

	switch (introVar.sequence) {
	case 0: {
		introVar.alpha += 200 * dt;
		renderStaticBG("resources/taffyEngine.png", clamp(0, introVar.alpha, 255) , 0);
		if (introVar.alpha >= 300 || pressed(ENTER)) introVar.sequence = 2;
	} break;
	case 2: {
		introVar.alpha -= 200 * dt;
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

//my moving background
float customBGx = 0, customBGy = 0, change = 0;
internal void renderCustomBG(bool stop, u8 shade = 0, bool reset = false) {
	if (reset) {
		customBGx = 0, customBGy = 0, change = 0;
	}
	else {
		if (!stop) {
			customBGx -= 960 * dt * 0.2;
			customBGy -= 540 * dt * 0.2;
			change += 1 * dt;
		}
		if (change >= 3) change -= 3;
		if (customBGx <= -960) customBGx += 960;
		if (customBGy <= -540) customBGy += 540;
		switch ((int)change % 3) {
			case 0: {renderMovingBG("resources/triangles.png", customBGx + 480, customBGy + 270, 2, shade);} break;
			case 1: {renderMovingBG("resources/circles.png", customBGx + 480, customBGy + 270, 2, shade);} break;
			case 2: {renderMovingBG("resources/squares.png", customBGx + 480, customBGy + 270, 2, shade);} break;
		}
	}
}


enum TransitionEvents {
	TRANSIDLE, TRANSOUTINIT, TRANSOUT, TRANSININIT, TRANSIN
};

struct TransVar {
	int y_value = 700;
	int rate_value = -1400;
	short acc_value = 0;
	u8 sequence = TRANSIDLE;
};

TransVar tsV;
internal void renderTransition(bool objects, bool updates) {
	switch (tsV.sequence) {

	case TRANSOUTINIT: {
		tsV.y_value = 700;
		tsV.rate_value = -700;
		tsV.sequence = TRANSOUT;
	}
	case TRANSOUT: {
		tsV.y_value += tsV.rate_value * dt + 0.5 * tsV.acc_value * dt * dt;
		tsV.rate_value += tsV.acc_value;


		if (updates) updateAllObjects();

		if (objects) renderAllObjects();

		renderRect(0, max(tsV.y_value, 0), 480, 270, 0);

		if (tsV.y_value <= 0) { tsV.sequence = TRANSIDLE; }
	} break;

	case TRANSININIT: {
		tsV.y_value = 0;
		tsV.rate_value = 700;
		tsV.sequence = TRANSIN;
	}
	case TRANSIN: {
		tsV.y_value += tsV.rate_value * dt + 0.5 * tsV.acc_value * dt * dt;
		tsV.rate_value += tsV.acc_value;

		if (updates) updateAllObjects();

		if (objects) renderAllObjects();

		renderRect(0, min(tsV.y_value, 700), 480, 270, 0);

		if (tsV.y_value >= 700) { tsV.sequence = TRANSIDLE; }
	} break;

	case TRANSIDLE: {
		renderRect(0, tsV.y_value, 480, 270, 0);
	} break;
	}
}


//events are named so its easier to keep track of them
enum MainMenuEvents {
	MMINIT, MMLOOP, EXITINIT, EXITLOOP, LEVELINIT, LEVELLOOP, TRANSITIONOUT
};

//main menu event
struct MainMenuVar {
	u8 sequence = MMINIT;
	u8 levelUnlocked = 1;
};

MainMenuVar mmVar;

Button LevelSelect;
Button Options;
Button Controls;
Button Credits;
UIObject Title;

UIObject Exit;
Button ExitYea;
Button ExitNah;

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

#include "gameevents.cpp"

internal void mainMenu() {
	switch (mmVar.sequence) {
	case MMINIT: {
		renderCustomBG(true, 0, true);
		new (&LevelSelect) Button(-160, -50, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(276, 559, 175, 51));
		new (&Options) Button(160, -50, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(451, 559, 175, 51));
		new (&Credits) Button(160, -150, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(276, 610, 175, 51));
		new (&Controls) Button(-160, -150, 250, 70, MIDDLE, &globalSpriteSheet, CropInfo(451, 610, 175, 51));
		new (&Title) UIObject(0, 120, 350, 200, MIDDLE, &globalSpriteSheet, CropInfo(494, 691, 453, 273));

		mmVar.sequence = MMLOOP;
	}break;
	case MMLOOP: {
		renderCustomBG(false);
		updateAllObjects();
		renderAllObjects();
		

		if (LevelSelect.bb.released) { 
			destroyAllObjects();
			mmVar.sequence = LEVELINIT; 
		}

		if (pressed(ESC)) {
			mmVar.sequence = EXITINIT;
		}
	}break;
	case EXITINIT: {
		LevelSelect.bb.disable = true;
		Options.bb.disable = true;
		Credits.bb.disable = true;
		Controls.bb.disable = true;

		updateAllObjects();

		LevelSelect.setShade(50);
		Options.setShade(50);
		Credits.setShade(50);
		Controls.setShade(50);
		Title.setShade(50);

		new (&Exit) UIObject(0, 0, 250, 150, FRONT, &globalSpriteSheet, CropInfo(31, 688, 329, 205));
		new (&ExitYea) Button(90, -50, 30, 15, FARFRONT, &globalSpriteSheet, CropInfo(374, 689, 97, 44));
		new (&ExitNah) Button(-90, -50, 30, 15, FARFRONT, &globalSpriteSheet, CropInfo(375, 750, 96, 48));
		
		mmVar.sequence = EXITLOOP;
		
	} break;
	case EXITLOOP: {
		renderCustomBG(true, 50, false);
		renderZObjects(&Middle_2);

		updateZObjects(&Front_3);
		updateZObjects(&FarFront_4);
		renderZObjects(&Front_3);
		renderZObjects(&FarFront_4);
		

		if (ExitYea.bb.pressed) {
			running = false;
		}

		if (pressed(ESC) || ExitNah.bb.pressed) {

			ExitYea.destroyObject();
			ExitNah.destroyObject();
			Exit.destroyObject();
			

			LevelSelect.bb.disable = false;
			Options.bb.disable = false;
			Credits.bb.disable = false;
			Controls.bb.disable = false;

			Title.setShade(0);

			mmVar.sequence = MMLOOP;
		}

		
	} break;
	case LEVELINIT: {
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
		switch (mmVar.levelUnlocked) {
		case 1: Level2.bb.disable = true; Level2.changeCrop(CropInfo(50, 100, 50, 50));
		case 2: Level3.bb.disable = true; Level3.changeCrop(CropInfo(100, 100, 50, 50));
		case 3: Level4.bb.disable = true; Level4.changeCrop(CropInfo(150, 100, 50, 50));
		case 4: Level5.bb.disable = true; Level5.changeCrop(CropInfo(200, 100, 50, 50));
		case 5: Level6.bb.disable = true; Level6.changeCrop(CropInfo(0, 150, 50, 50));
		case 6: Level7.bb.disable = true; Level7.changeCrop(CropInfo(50, 150, 50, 50));
		case 7: Level8.bb.disable = true; Level8.changeCrop(CropInfo(100, 150, 50, 50));
		case 8: Level9.bb.disable = true; Level9.changeCrop(CropInfo(150, 150, 50, 50));
		case 9: Level10.bb.disable = true; Level10.changeCrop(CropInfo(200, 150, 50, 50));
		case 10: break;
		}
		
		mmVar.sequence = LEVELLOOP;

	} break;

#define LevelButton(b, n) \
if (b.bb.released) {\
		mgVar.level = n;\
		tsV.sequence = TRANSOUTINIT; \
		mmVar.sequence = TRANSITIONOUT;\
}

	case LEVELLOOP: {
		renderCustomBG(false, false);
		updateAllObjects();
		renderAllObjects();

		LevelButton(Level1, 1);
		LevelButton(Level2, 2);
		LevelButton(Level3, 3);
		LevelButton(Level4, 4);
		LevelButton(Level5, 5);
		LevelButton(Level6, 6);
		LevelButton(Level7, 7);
		LevelButton(Level8, 8);
		LevelButton(Level9, 9);
		LevelButton(Level10, 10);

		if (Back.bb.released || pressed(ESC)) {
			destroyAllObjects();
			mmVar.sequence = MMINIT;
		}

	} break;
	case TRANSITIONOUT: {
		renderCustomBG(false);
		
		renderTransition(true, false);
		
		if (tsV.sequence == TRANSIDLE) {
			destroyAllObjects();
			currentScreen = GAME;
		}
	} break;
	}
	
}


struct IntroVar {
	float alpha = 0;
	int sequence = 0;
};
IntroVar introVar;
internal void intro(float dt) {

	switch (introVar.sequence) {
	case 0: {
		introVar.alpha += 0.7 * dt;
		renderStaticBG("resources/taffyEngine.png", introVar.alpha, 0);
		if (introVar.alpha >= 1.5 || pressed(ENTER)) introVar.sequence = 2;
	} break;
	case 2: {
		introVar.alpha -= 0.7 * dt;
		renderStaticBG("resources/taffyEngine.png", introVar.alpha, 0);
		if (introVar.alpha <= -0.5) introVar.sequence = 3;
	} break;
	case 3: {
		introVar.sequence = 0;
		introVar.alpha = 0;
		currentScreen = GAME;
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
		if (change >= 3) change = 0;
		if (customBGx <= -960) customBGx = 0;
		if (customBGy <= -540) customBGy = 0;
		switch ((int)change % 3) {
		case 0: {
			renderMovingBG("resources/triangles.png", customBGx + 480, customBGy + 270, 1);
		} break;
		case 1: {
			renderMovingBG("resources/squares.png", customBGx + 480, customBGy + 270, 1);
		} break;
		case 2: {
			renderMovingBG("resources/circles.png", customBGx + 480, customBGy + 270, 1);
		} break;
		}
	}
}
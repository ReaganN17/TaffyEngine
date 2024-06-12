struct IntroVar {
	float alpha = 0;
	int sequence = 0;
};
IntroVar introVar;
internal void intro(Input* input, float dt) {

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


struct PlayerUI : Object {
	Player* player;

	UIObject* HealthBar;
	UIObject* HealthBackGround;
	UIObject* Health;
	UIObject* HealthEnd;
	UIObject* PowerUP;
	UIObject* State;

	PlayerUI();
	~PlayerUI();

	PlayerUI(Image* spritesheet, Player* player);

	PlayerUI& destroy();

	PlayerUI& hide();
	PlayerUI& unhide();

	PlayerUI& update();

};

PlayerUI::PlayerUI() {}
PlayerUI::~PlayerUI() {}

PlayerUI::PlayerUI(Image* spritesheet, Player* player) : Object(0, 0, 960, 540, 5) {
	HealthBar = new UIObject(250, 220, 400, 50, FARFRONT, spritesheet, CropInfo(4, 18, 934, 120));
	HealthBackGround = new UIObject(250, 220, 390, 40, 0x4f4f4f, FRONT);
	Health = new UIObject(249, 220, 380, 40, 0xED1C24, FRONT);
	HealthEnd = new UIObject(439, 220, 10, 40, FRONT, spritesheet, CropInfo(14, 144, 32, 92));

	PowerUP = new UIObject(-200, 200, 100, 50, FRONT, spritesheet, CropInfo(5, 275, 255, 130));
	State = new UIObject(-200, -200, 50, 50, FRONT, spritesheet, CropInfo(980, 37, 120, 113));

	this->player = player;
}

PlayerUI& PlayerUI::destroy() {
	HealthBar->destroyObject();
	Health->destroyObject();
	HealthEnd->destroyObject();
	HealthBackGround->destroyObject();

	PowerUP->destroyObject();
	State->destroyObject();

	return *this;
}

PlayerUI& PlayerUI::hide() {
	HealthBar->ob.renderable = false;
	HealthBackGround->ob.renderable = false;
	HealthEnd->ob.renderable = false;
	Health->ob.renderable = false;
	PowerUP->ob.renderable = false;
	State->ob.renderable = false;

	return *this;
}

PlayerUI& PlayerUI::unhide() {
	HealthBar->ob.renderable = true;
	HealthBackGround->ob.renderable = true;
	HealthEnd->ob.renderable = true;
	Health->ob.renderable = true;

	PowerUP->ob.renderable = true;
	State->ob.renderable = true;

	return *this;
}

PlayerUI& PlayerUI::update() {
	float health = (max((float)player->health, 0));


	if (player->inst.stateChange) {
		switch (player->state) {
		case SQUARE: {
			PowerUP->changeCrop(CropInfo(5, 275, 255, 130));
		} break;

		case CIRCLE: {
			PowerUP->changeCrop(CropInfo(265, 294, 255, 130));
		} break;

		case TRIANGLE: {
			PowerUP->changeCrop(CropInfo(54, 142, 255, 130));
		} break;
		}

		player->inst.stateChange = false;
	}

	if (player->inst.healthChange) {
		HealthEnd->animateToPos(health / 250 * 380 + 59, 220, HealthEnd->w, HealthEnd->h, 0.5, true);
		Health->animateToPos(health / 250 * 190 + 59, 220, health / 250 * 380, 40, 0.5, true);

		player->inst.healthChange = false;
	}



	return *this;
}


enum GeoState {
	SQUARE, TRIANGLE, CIRCLE
};

struct Player : SentientGO {

	Player();
	~Player();
	Player(Grid* grid, u16 x, u16 y, zLayer z);

	void control(PControls inputs, float dt);

	Player& move(PControls inputs, float dt);
	Player& ability(PControls inputs, float dt);
	Player& rotate(PControls inputs, float dt);
	Player& takeDamage(short base);
	Player& toPos(int x, int y);
	Player& changeChar(PControls inputs, float dt);

	void render();

	Player& update(float dt);
};

//obligatory constructor
Player::Player() {}

//obligatory destructor
Player::~Player() { if (!ob.instance) return; }

//actual constructor
Player::Player(Grid* grid, u16 x, u16 y, zLayer z = MIDDLE) : SentientGO(grid, x, y, z, "resources/playerSpriteSheetV1.png", 161, 165, 170, 170, 0.65, 2) 
{
	health = 250;
	state = SQUARE;
	speed = 0;
	range = 1;
	power = 5;
}

void Player::control(PControls inputs, float dt) {
	changeChar(inputs, dt);
	ability(inputs, dt);
	rotate(inputs, dt);
	move(inputs, dt);
}

Player& Player::changeChar(PControls inputs, float dt) {
	if (pressed(inputs.square)) { changeCrop("resources/playerSpriteSheetV1.png", 161, 165, 170, 170); state = SQUARE; }
	if (pressed(inputs.circle)) { changeCrop("resources/playerSpriteSheetV1.png", 611, 158, 185, 177); state = CIRCLE; }
	if (pressed(inputs.triangle)) { changeCrop("resources/playerSpriteSheetV1.png", 348, 166, 229, 164); state = TRIANGLE; }

	return *this;
}

Player& Player::rotate(PControls inputs, float dt) {
	if (pressed(inputs.rotleft)) { mmb.direction++;}
	if (pressed(inputs.rotright)) { mmb.direction--;}

	return *this;
}

Player& Player::ability(PControls inputs, float dt) {
	if (pressed(inputs.power)) {
		switch (state) {
			case SQUARE: break;
			case TRIANGLE: break;
			case CIRCLE: mmb.sequence = MOVEINIT; range = 7; speed = 70; break;
		}
	}

	return *this;
}

Player& Player::move(PControls inputs, float dt) {
	
	//4 directional movement
	switch (mmb.sequence) {
		case SELECTION: {

			if (pressed(inputs.left)) { mmb.direction = MLEFT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.right)) { mmb.direction = MRIGHT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.up)) { mmb.direction = MUP; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.down)) { mmb.direction = MDOWN; mmb.sequence = MOVEINIT; }
		} break;
	
		case MOVEINIT: {
			//custom speed can overwrite other speed
			if (speed == 0) {
				switch (state) {
				case SQUARE: speed = 30; break;
				case TRIANGLE: speed = 60; break;
				case CIRCLE: speed = 100; break;
				}
			}

			moveinit();
			mmb.sequence = MOVE;
		} break;
	
		default: {
			SentientGO::move(dt);
		} break;
	}

	return *this;
}


Player& Player::takeDamage(short base) {
	switch (state) {
		case SQUARE: base *= 0.8;  break;
		case TRIANGLE: base *= 1.2; break;
		case CIRCLE: break;
	}

	SentientGO::takeDamage(base);

	return *this;
}

Player& Player::toPos(int x, int y) {
	grid->grid[xG + yG * grid->gw] = overlapped;
	xG = x, yG = y;
	overlapped = grid->grid[xG + yG * grid->gw];
	grid->grid[xG + yG * grid->gw] = id;
	return *this;
}

void Player::render() {
	Image arrow("resources/playerSpriteSheetV1.png");

	switch (state) {
		case SQUARE:arrow.crop(164, 383, 151, 55); break;
		case TRIANGLE: arrow.crop(383, 375, 141, 59); break;
		case CIRCLE: arrow.crop(633, 379, 130, 50); break;
	}

	switch (mmb.direction) {
		case MLEFT:arrow.rotate(90); break;
		case MRIGHT:arrow.rotate(-90); break;
		case MUP: break;
		case MDOWN:arrow.rotate(-180); break;
	}

	if (state == TRIANGLE) {
		switch (mmb.direction) {
			case MLEFT:sprite.rotateTo(90); break;
			case MRIGHT:sprite.rotateTo(-90); break;
			case MUP: sprite.rotateTo(0); break;
			case MDOWN:sprite.rotateTo(-180); ; break;
		}
	}
	
	int paraX = (((this->x) + ((mmb.direction == MLEFT) ? -0.5 : (mmb.direction == MRIGHT) ? 0.5 : 0) * grid->grid_scale) - mainCam.x) * mainCam.zoom;
	int paraY = (((this->y) + ((mmb.direction == MDOWN) ? -0.5 : (mmb.direction == MUP) ? 0.5 : 0) * grid->grid_scale) - mainCam.y) * mainCam.zoom;
	int paraW = ((mmb.direction % 2 == 0) ? 10 : 20) * mainCam.zoom;
	int paraH = ((mmb.direction % 2 == 1) ? 10 : 20) * mainCam.zoom;

	renderImageV2(&arrow, paraX, paraY, paraW, paraH);

	Object::render();
}

Player& Player::update(float dt) {
	PathMap.goalUpdated = false;
	if (mmb.sequence != SELECTION) { PathMap.setGoal(xG, yG); PathMap.goalUpdated = true; }

	return *this;
}
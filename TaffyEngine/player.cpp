enum movementSeq {
	SELECTION, MOVEINIT, MOVE, MOVEINTERUPTINIT, MOVEINTERUPT, BOUNCEINIT, BOUNCEMOVE, MOVEEND
};

enum GeoState {
	SQUARE, TRIANGLE, CIRCLE
};

union MovementByte {
	u8 byte = 0;

	struct {
		u8 sequence : 6;
		u8 direction : 2;
	};
};

struct Player : GridObject {

	Player();
	~Player();
	Player(Grid* grid, u16 x, u16 y, zLayer z);

	//moving purposes
	float pos;
	float acc;
	float rate = 0;
	float posborder;
	u8 distance;
	MovementByte mmb;

	//player characteristics
	short health = 250;
	u8 state = SQUARE;
	u8 power = 5;
	u8 range = 1;

	void control(PControls inputs, float dt);

	Player& move(PControls inputs, float dt);
	Player& ability(PControls inputs, float dt);
	Player& rotate(PControls inputs, float dt);
	Player& toPos(int x, int y);
	Player& changeChar(PControls inputs, float dt);
	bool checkValidVector(u8 dir, float mag);	

	bool checkValid(int x, int y);

	void render();
};

//obligatory constructor
Player::Player() {}

//obligatory destructor
Player::~Player() { if (!ob.instance) return; }

//actual constructor
Player::Player(Grid* grid, u16 x, u16 y, zLayer z = MIDDLE) : GridObject(grid, x, y, "resources/playerSpriteSheetV1.png", 161, 165, 170, 170, 0.65, z, 2) {}

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
			case CIRCLE: mmb.sequence = MOVEINIT; range = 7; rate = 7; break;
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
			float speed;

			//custom rate can overrite speeds
			if (rate == 0) {
				switch (state) {
					case SQUARE: speed = 3; break;
					case TRIANGLE: speed = 6; break;
					case CIRCLE: speed = 10; break;
				}
			}
			else {
				speed = rate;
			}

			distance = range;
			pos = 0.5;

			rate = 2 * speed * distance;
			acc = -2 * distance * speed * speed;
			mmb.sequence = MOVE;
		}

		case MOVE: {
			if (!checkValidVector(mmb.direction, 1)) { mmb.sequence = MOVEINTERUPTINIT; break; }

			grid->grid[xG + yG * grid->gw] = overlapped;
	
			//kinematics (sorta)
			pos +=  rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;
		
			switch (mmb.direction) {
				case MLEFT: xG -= floor(pos); break;
				case MRIGHT: xG += floor(pos); break;
				case MUP: yG -= floor(pos); break;
				case MDOWN: yG += floor(pos); break;
			}
	
			overlapped = grid->grid[xG + yG * grid->gw];
			grid->grid[xG + yG * grid->gw] = id;


			pos = fmod(pos, 1);

			setGridVector(mmb.direction, pos - 0.5);

			

			if (rate <= 0) { mmb.sequence = MOVEEND;}
		} break;

		case MOVEINTERUPTINIT: {
			float b;

			switch(grid->getIDVector(mmb.direction, 1, xG, yG)) {
				case 1: b = 1; break;
				default: b = -2; break;
			}
			
			posborder = 0.5 * (b - (float)scale/1000) + 0.5;

			mmb.sequence = MOVEINTERUPT;
		} break;

		case MOVEINTERUPT: {

			pos += rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;

			setGridVector(mmb.direction, pos - 0.5);
			if (pos >= posborder) { mmb.sequence = BOUNCEINIT; setGridVector(mmb.direction, posborder - 0.5); break; }

			if (rate <= 0) { mmb.sequence = MOVEEND;}
		} break;

		case BOUNCEINIT: {
			float factor = 0.1;
			
			acc = 2 * (posborder - 0.5) / (factor * factor);
			rate = -2 * (posborder - 0.5) / (factor);

			pos = posborder;

			mmb.sequence = BOUNCEMOVE;

		}break;

		case BOUNCEMOVE: {
			pos +=  rate * dt + acc * dt * dt * 0.5f;
			rate += acc * dt;

			setGridVector(mmb.direction, pos - 0.5);

			if (rate >= 0) { mmb.sequence = MOVEEND; }
		} break;

		case MOVEEND: {
			rate = 0;
			range = 1;
			setGridVector(mmb.direction, 0);
			mmb.sequence = SELECTION;
		}break;
	}

	return *this;
}


Player& Player::toPos(int x, int y) {
	grid->grid[xG + yG * grid->gw] = overlapped;
	xG = x, yG = y;
	overlapped = grid->grid[xG + yG * grid->gw];
	grid->grid[xG + yG * grid->gw] = id;
	return *this;
}

bool Player::checkValidVector(u8 dir, float mag) {
	switch (dir) {
		case MLEFT: return checkValid(xG - mag, yG);
		case MRIGHT: return checkValid(xG + mag, yG);
		case MUP: return checkValid(xG, yG - mag);
		case MDOWN: return checkValid(xG, yG + mag);
	}
}

bool Player::checkValid(int x, int y) {
	if (grid->grid[x + y * grid->gw] > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
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
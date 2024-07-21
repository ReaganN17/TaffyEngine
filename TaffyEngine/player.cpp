enum GeoState {
	SQUARE, TRIANGLE, CIRCLE
};

struct Player : GridObject {


	Player();
	~Player();
	Player(Grid* grid, u16 x, u16 y, Image *img);

	void control(PControls inputs);

	Player& move(PControls inputs);
	Player& ability(PControls inputs);
	Player& rotate(PControls inputs);
	Player& takeDamage(short base);
	Player& changeChar(PControls inputs);

	void render();

	Player& update();

	bool checkValidVector(u8 dir, float mag);
	bool checkValid(int x, int y);
};

//obligatory constructor
Player::Player() {}

//obligatory destructor
Player::~Player() { if (!ob.instance) return; }

//actual constructor

Player::Player(Grid* grid, u16 x, u16 y, Image *spritesheet) : GridObject(grid, x, y, MIDDLE, spritesheet, CropInfo(980, 37, 120, 113), 0.65, 2, CHARA)
{	
	health = 250;
	state = SQUARE;
	speed = 0;
	range = 1;
	power = 5;
}

void Player::control(PControls inputs) {
	changeChar(inputs);
	ability(inputs);
	rotate(inputs);
	move(inputs);
}

Player& Player::changeChar(PControls inputs) {
	if (pressed(inputs.square)) { changeCrop(CropInfo(980, 37, 120, 113)); state = SQUARE; inst.stateChange = true; }
	if (pressed(inputs.circle)) { changeCrop(CropInfo(1300, 34, 118, 116)); state = CIRCLE; inst.stateChange = true;}
	if (pressed(inputs.triangle)) { changeCrop(CropInfo(1114, 34, 158, 114)); state = TRIANGLE; inst.stateChange = true;}

	return *this;
}

Player& Player::rotate(PControls inputs) {
	if (pressed(inputs.rotleft)) { mmb.direction++;}
	if (pressed(inputs.rotright)) { mmb.direction--;}

	return *this;
}

Player& Player::ability(PControls inputs) {
	if (pressed(inputs.power)) {
		switch (state) {
			case SQUARE: break;
			case TRIANGLE: break;
			case CIRCLE: mmb.sequence = MOVEINIT; range = 7; rate = 70; break;
		}
	}

	return *this;
}

Player& Player::move(PControls inputs) {
	
	//4 directional movement
	switch (mmb.sequence) {
		case SELECTION: {

			if (pressed(inputs.left)) { mmb.direction = MLEFT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.right)) { mmb.direction = MRIGHT; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.up)) { mmb.direction = MUP; mmb.sequence = MOVEINIT; }
			if (pressed(inputs.down)) { mmb.direction = MDOWN; mmb.sequence = MOVEINIT; }

		} break;
	
		case MOVEINIT: {
			//custom rate can overwrite speed
			if (rate == 0) {
				switch (state) {
				case SQUARE: speed = 30; break;
				case TRIANGLE: speed = 60; break;
				case CIRCLE: speed = 100; break;
				}
			}
			else { speed = rate; }

			moveinit();
			mmb.sequence = MOVE;
		} break;

		
		case MOVEEND: {

			//reset range
			range = 1;
			moveend();
			mmb.sequence = SELECTION;
		} break;

		default: {
			GridObject::move(0.2);
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
	inst.healthChange = true;
	GridObject::takeDamage(base);

	return *this;
}

void Player::render() {
	Image arrow;

	switch (state) {
		case SQUARE: new(&arrow) Image(spritesheet->produceCrop(986, 193, 99, 28)); break;
		case TRIANGLE: new(&arrow) Image(spritesheet->produceCrop(1137, 191, 99, 28)); break;
		case CIRCLE: new(&arrow) Image(spritesheet->produceCrop(1312, 188, 95, 29)); break;
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

Player& Player::update() {
	if (mmb.sequence == MOVEEND || grid->nodeEnd == nullptr) {
		grid->setEnd(xG, yG);
	}

	inst.byte = 0;

	return *this;
}

//basic valid checkers for moving
bool Player::checkValidVector(u8 dir, float mag) {
	switch (dir) {
	case MLEFT: return checkValid(xG - mag, yG);
	case MRIGHT: return checkValid(xG + mag, yG);
	case MUP: return checkValid(xG, yG - mag);
	case MDOWN: return checkValid(xG, yG + mag);
	}
}

bool Player::checkValid(int x, int y) {
	if (grid->nodes[x + y * grid->gw].occupants.size() > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
}
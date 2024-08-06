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
	speed = 30;
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
	if (pressed(inputs.square)) { changeCrop(CropInfo(980, 37, 120, 113)); state = SQUARE; speed = 30;  inst.stateChange = true; }
	if (pressed(inputs.circle)) { changeCrop(CropInfo(1300, 34, 118, 116)); state = CIRCLE; speed = 100; inst.stateChange = true; }
	if (pressed(inputs.triangle)) { changeCrop(CropInfo(1114, 34, 158, 114)); state = TRIANGLE; speed = 60; inst.stateChange = true; }

	return *this;
}

Player& Player::rotate(PControls inputs) {
	if (pressed(inputs.rotleft)) { inst.direction++;}
	if (pressed(inputs.rotright)) { inst.direction--;}

	return *this;
}

Player& Player::ability(PControls inputs) {
	if (pressed(inputs.power)) {
		switch (state) {
			case SQUARE: break;
			case TRIANGLE: break;
			case CIRCLE: range = 7; speed = 70; GridObject::move(0.2); range = 1; speed = 60; break;
		}
	}

	return *this;
}

Player& Player::move(PControls inputs) {
	
	if (pressed(inputs.left)) { inst.direction = MLEFT; GridObject::move(0.2); }
	if (pressed(inputs.right)) { inst.direction = MRIGHT; GridObject::move(0.2);}
	if (pressed(inputs.up)) { inst.direction = MUP; GridObject::move(0.2);}
	if (pressed(inputs.down)) { inst.direction = MDOWN; GridObject::move(0.2);}

	return *this;
}


Player& Player::takeDamage(short base) {
	switch (state) {
		case SQUARE: base *= 0.8;  break;
		case TRIANGLE: base *= 1.2; break;
		case CIRCLE: break;
	}
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

	switch (inst.direction) {
		case MLEFT:arrow.rotate(90); break;
		case MRIGHT:arrow.rotate(-90); break;
		case MUP: break;
		case MDOWN:arrow.rotate(-180); break;
	}

	if (state == TRIANGLE) {
		switch (inst.direction) {
			case MLEFT:sprite.rotateTo(90); break;
			case MRIGHT:sprite.rotateTo(-90); break;
			case MUP: sprite.rotateTo(0); break;
			case MDOWN:sprite.rotateTo(-180); ; break;
		}
	}
	
	int paraX = (((this->x) + ((inst.direction == MLEFT) ? -0.5 : (inst.direction == MRIGHT) ? 0.5 : 0) * grid->grid_scale) - mainCam.x) * mainCam.zoom;
	int paraY = (((this->y) + ((inst.direction == MDOWN) ? -0.5 : (inst.direction == MUP) ? 0.5 : 0) * grid->grid_scale) - mainCam.y) * mainCam.zoom;
	int paraW = ((inst.direction % 2 == 0) ? 10 : 20) * mainCam.zoom;
	int paraH = ((inst.direction % 2 == 1) ? 10 : 20) * mainCam.zoom;

	renderImageV2(&arrow, paraX, paraY, paraW, paraH);
	

	Object::render();
}

Player& Player::update() {
	grid->setEnd(xG, yG);

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
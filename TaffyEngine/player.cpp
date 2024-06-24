enum movementSeq {
	SELECTION, MOVEINIT, MOVE, MOVEEND
};

union MovementByte {
	u8 byte = 0;

	struct {
		u8 sequence : 4;
		u8 direction : 4;
	};
};

struct Player : GridObject {

	Player();
	~Player();
	Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z);

	//player characteristics
	short health = 250;
	u8 power = 5;
	u8 range = 3;
	float speed = 5; //time between moves is 1/speed seconds;
	
	//moving purposes
	float pos = 0;
	float acc = 0;
	float rate = 0;
	u8 distance = 0;
	MovementByte mmb;

	Player& move(PControls inputs, float dt);

	Player& toPos(int x, int y);
	bool checkPosVector(u8 dir, float mag);
	Player& toGridVector(u8 dir, u8 mag);
	Player& gridPushBack(u8 dir, u8 mag);

	bool checkValid(int x, int y);
};

//obligatory constructor
Player::Player() {}

//obligatory destructor
Player::~Player() { if (!ob.instance) return; }

//actual constructor
Player::Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z = MIDDLE) : GridObject(grid, x, y, file, scale, z, 2) {}

Player& Player::move(PControls inputs, float dt) {

	//4 directional movement
	switch (mmb.sequence) {
	case SELECTION: {
		//god give me a better way
		if (pressed(inputs.left)) { mmb.direction = MLEFT; mmb.sequence = MOVEINIT; }
		if (pressed(inputs.right)) { mmb.direction = MRIGHT; mmb.sequence = MOVEINIT; }
		if (pressed(inputs.up)) { mmb.direction = MUP; mmb.sequence = MOVEINIT; }
		if (pressed(inputs.down)) { mmb.direction = MDOWN; mmb.sequence = MOVEINIT; }
	} break;

	case MOVEINIT: {
		distance = range;
		pos = 0.5;
		rate = 2 * speed;
		acc = rate * rate * -0.5;
		mmb.sequence = MOVE;
	}
	case MOVE: {
		grid->grid[xG + yG * grid->gw] = overlapped;

		//kinematics (sorta)
		pos += distance * rate * dt + distance * acc * dt * dt * 0.5;
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

		setGridVector(mmb.direction, clampF(-0.5, pos - 0.5, 0.5));

		if (rate <= 0) { mmb.sequence = MOVEEND; }
	} break;
	case MOVEEND: {
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

bool Player::checkPosVector(u8 dir, float mag) {

}

bool Player::checkValid(int x, int y) {
	if (grid->grid[x + y * grid->gw] > 0) return false;
	if (x < 0 || x >= grid->gw) return false;
	if (y < 0 || y >= grid->gh) return false;
	return true;
}
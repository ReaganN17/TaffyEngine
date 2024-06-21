enum movementSeq {
	SELECTION, MOVELEFT, MOVERIGHT, MOVEUP, MOVEDOWN, MOVEEND
};



struct Player : GridObject {

	//obligatory constructor
	Player() {}

	//obligatory destructor
	~Player() { if (!ob.instance) return; }

	//actual constructor
	Player(Grid* grid, u16 x, u16 y, const char* file, float scale, zLayer z = MIDDLE, u8 id = 2) : GridObject(grid, x, y, file, scale, z, id) {}


	//things i care about
	u8 sequence = 0;
	float pos = -1;
	float rate = 10;
	float acc = -20;

	void move(PControls inputs, float dt) {

		switch (sequence) {
		case SELECTION: {
			if (pressed(inputs.up)) { if (checkValid(xG, yG - 1)) { movement(xG, yG - 1); sequence = MOVEUP; } }
			if (pressed(inputs.down)) { if (checkValid(xG, yG + 1)) { movement(xG, yG + 1); sequence = MOVEDOWN; } }
			if (pressed(inputs.left)) { if (checkValid(xG - 1, yG)) { movement(xG - 1, yG); sequence = MOVELEFT; } }
			if (pressed(inputs.right)) { if (checkValid(xG + 1, yG)) { movement(xG + 1, yG); sequence = MOVERIGHT; } }
		} break;
		case MOVELEFT: {
			//kinematics
			pos += rate * dt + acc * dt * dt * .5f;
			rate += acc * dt;

			//minimum rate;
			rate = max(0.5, rate);

			setPos(grid->dx + (xG - min(pos, 0)) * grid->grid_scale, grid->dy - yG * grid->grid_scale);

			if (pos >= 0) { sequence = MOVEEND; }
		} break;
		case MOVERIGHT: {
			//kinematics
			pos += rate * dt + acc * dt * dt * .5f;
			rate += acc * dt;

			//minimum rate;
			rate = max(0.5, rate);

			setPos(grid->dx + (xG + min(pos, 0)) * grid->grid_scale, grid->dy - yG * grid->grid_scale);

			if (pos >= 0) { sequence = MOVEEND; }

		} break;
		case MOVEUP: {
			//kinematics
			pos += rate * dt + acc * dt * dt * .5f;
			rate += acc * dt;

			//minimum rate;
			rate = max(0.5, rate);

			setPos(grid->dx + xG * grid->grid_scale, grid->dy - (yG - min(pos, 0)) * grid->grid_scale);

			if (pos >= 0) { sequence = MOVEEND; }

		} break;
		case MOVEDOWN: {
			//kinematics
			pos += rate * dt + acc * dt * dt * .5f;
			rate += acc * dt;

			//minimum rate;
			rate = max(0.5, rate);

			setPos(grid->dx + xG * grid->grid_scale, grid->dy - (yG + min(pos, 0)) * grid->grid_scale);

			if (pos >= 0) { sequence = MOVEEND; }
		} break;
		case MOVEEND: {
			sequence = 0;
			pos = -1;
			rate = 5;
			acc = -10;
		} break;
		}
	}

	Player& movement(int x, int y) {
		grid->grid[xG + yG * grid->gw] = overlapped;
		xG = x, yG = y;
		grid->grid[xG + yG * grid->gw] = id;
		return *this;
	}

	bool checkValid(int x, int y) {
		if (x < 0 || x >= grid->gw) return false;
		if (y < 0 || y >= grid->gh) return false;
		if (grid->grid[x + y * grid->gw] != 0) return false;
		return true;
	}

	void toPos() {

	}


};
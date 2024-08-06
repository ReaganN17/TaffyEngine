struct BasicWall : GridObject {

	BasicWall() {}
	~BasicWall() {}

	BasicWall(Grid* grid) : GridObject(grid, 0, 0, (zLayer)5, (u32)0, 1.f, 1, OBSTACLE) {}

	BasicWall& takeDamage(short base) {return *this;}
};
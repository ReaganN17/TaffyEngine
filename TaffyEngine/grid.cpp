
//0 - traverse
//1 to whatever number - no access

//2D array
//built from top down, left right, no negatives


struct Grid : Object{
	u8* grid;
	u16 gw, gh;
	int dx, dy;
	float grid_scale = 1;

	//obligatory default constructor
	Grid() {}

	//obligatory destructor
	~Grid() { if (!ob.instance) return; }

	Grid& PNGToGrid(const char* file) {
		Image ref(file);
		gw = ref.w;
		gh = ref.h;

		grid = new u8[gw * gh];

		for (int i = 0; i < ref.w * ref.h; i++) {
			grid[i] = (ref.data[ref.channels * i] == 0) ? 1 : 0;
		}

		return *this;
	}


	//grid object constructor
	//x y coordinates are where the center of 0, 0 coord is
	//offsetX, offsetY is the pixel coord of the display map where the topleft of 0, 0 coord
	//pixelsPerSqr is scale of the grid
	//pixelsPerSqrD is the scale of the display
	Grid(int x, int y, float pixelsPerSqr, const char* gridMap, int offsetX, int offsetY, float pixelsPerSqrD, const char* displayMap, zLayer  z = BACK) : Object(0, 0, displayMap, z) {
		PNGToGrid(gridMap);
		w = gw, h = gh;

		grid_scale = pixelsPerSqr, dx = x + grid_scale * 0.5, dy = y - grid_scale * 0.5;
		ob.layer = z;

		setScale(w * pixelsPerSqr/pixelsPerSqrD, h * pixelsPerSqr / pixelsPerSqrD);
		setPos(x + w * (0.5 + offsetX), y - h * (0.5 + offsetY));

		ob.cameraLinked = true;
	}
};



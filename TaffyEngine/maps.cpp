struct Map : Object {
	bool repeat = false;
	float rate = 1;
	const char* file;

	Map(int x0, int y0, int x1, int y1, const char* filename, u8 z, bool repeat, float rate);
	Map();
	~Map();

	virtual void render();
	virtual Map& update();
};

Map::Map() {}

Map::~Map() { if (!ob.instance) return; }

Map::Map(int x0, int y0, int x1, int y1, const char*filename, u8 z = BACK, bool repeat = false, float rate = 1) 
	:Object(x0 + (x1 - x0) * 0.5, y0 + (y1 - y0) * 0.5, abs(x1 - x0), abs(y1 - y0), filename, z), repeat(repeat), rate(rate) {
	file = filename;
}

void Map::render() {
	if (repeat) {
		renderRepeatMap(file, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y)* mainCam.zoom, w * mainCam.zoom , h * mainCam.zoom);
	}
	else {
		renderImageV2(&sprite, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom);
	}
}

Map& Map::update() {
	if (repeat) {
		if (x < -w) x += w;
		if (x > w) x -= w;
		if (y < -h) y += h;
		if (y > h) y -= h;
	}

	return *this;
}
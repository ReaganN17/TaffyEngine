struct Map : Object {
	bool repeat = false;
	float rate = 1;

	Map(int x0, int y0, int x1, int y1, const char* filename, zLayer z, bool repeat, float rate);
	Map();

	virtual void render();
};

Map::Map() {}

Map::Map(int x0, int y0, int x1, int y1, const char*filename, zLayer z = BACK, bool repeat = false, float rate = 1) 
	:Object(x0 + (x1-x0) * 0.5, y0 + (y1 - y0) * 0.5, abs(x1-x0), abs(y1-y0), filename, z), repeat(repeat), rate(rate) {}

void Map::render() {
	if (repeat) {
		renderRepeatMap(file, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom);
	}
	else {
		renderImageV2(file, (x - mainCam.x) * mainCam.zoom, (y - mainCam.y) * mainCam.zoom, w * mainCam.zoom, h * mainCam.zoom);
	}
	
}
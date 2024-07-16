struct UIObject : Object {

	UIObject(int x, int y, int w, int h, const char* filename, zLayer z);
	UIObject(int x, int y, int w, int h, zLayer z, Image* spritesheet, CropInfo crop);
	UIObject(int x, int y, int w, int h, u32 c, zLayer z);
	UIObject();
	~UIObject();

};

UIObject::UIObject() {}

UIObject::UIObject(int x, int y, int w, int h, const char* filename, zLayer z = MIDDLE)
	:Object(x, y, w, h, filename, z) {
	ob.cameraLinked = false;
}

UIObject::UIObject(int x, int y, int w, int h, zLayer z, Image* spritesheet, CropInfo crop)
	:Object(x, y, w, h, z, spritesheet, crop) {
	ob.cameraLinked = false;
}

UIObject::UIObject(int x, int y, int w, int h, u32 c, zLayer z = MIDDLE)
	:Object(x, y, w, h, c, z) {
	ob.cameraLinked = false;
}

UIObject::~UIObject() {
	if (!ob.instance) return;
}
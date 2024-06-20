struct UIObject : Object {

	UIObject(int x, int y, int w, int h, const char* filename, zLayer z);
	UIObject(int x, int y, int w, int h, u32 c, zLayer z);
	UIObject();
	~UIObject();

};

UIObject::UIObject() {}

UIObject::UIObject(int x, int y, int w, int h, const char* filename, zLayer z = MIDDLE)
	:Object(x, y, w, h, filename, z) {}

UIObject::UIObject(int x, int y, int w, int h, u32 c, zLayer z = MIDDLE)
	:Object(x, y, w, h, c, z) {}

UIObject::~UIObject() {
	if (!ob.instance) return;
}
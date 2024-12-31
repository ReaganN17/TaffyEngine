
class MoveBackground : public Event {
	Object bg;
	Timer timer;

	float time = 4;
	u8 bg_num = 0;
public:
	MoveBackground() {}
private:
	void init() {
		new (&bg) Object(0, 0, "Moving_BG_0", Z_FARBACK);
		bg.setSize(Win32Render::coord_width * 2, Win32Render::coord_height * 2);

		bg.setPos(bg.getW() / 4, bg.getH() / 4);
		timer.start();
	}

	void loop() {
		if (timer.hasElapsed(6000)) { 
			timer.start(); 
			bg.setSprite("Moving_BG_0"); 
			bg_num = 0; 
		}
		else if (timer.hasElapsed(4000)) {
			if (bg_num != 1) { bg.setSprite("Moving_BG_1"); bg_num = 1; }
			
		}
		else if (timer.hasElapsed(2000)) { 
			if (bg_num != 2) { bg.setSprite("Moving_BG_2"); bg_num = 2; }
		}

		bg.setPos(
			fmod(
				bg.getX() 
				- Win32Render::coord_width * Win32Window::getDT() / time
				- Win32Render::coord_width/2, Win32Render::coord_width) + Win32Render::coord_width / 2,
			fmod(
				bg.getY() 
				- Win32Render::coord_height * Win32Window::getDT()/ time 
				- Win32Render::coord_height/2, Win32Render::coord_height) + Win32Render::coord_height/2);
	}

	void end(bool interrupted) {
		ObjectHandler::removeObject(&bg);
	
	}

	bool isFinished() {return false;}
};
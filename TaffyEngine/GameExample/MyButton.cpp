

class MyButton : public ScreenButton {
	float default_w, default_h;
public:
	MyButton() {}

	MyButton(ScreenButton sb) : ScreenButton(sb) {
		default_w = sb.getW();
		default_h = sb.getH();

		ObjectHandler::addObject(this);
	}
	
private:

	//TODO consider a better way to do this
	void periodic() {
		if (is_hovered()) {
			setSize(default_w * 1.1, default_h * 1.1);
		}
		else { setSize(default_w, default_h); }

		if (is_down()) {
			setShade(0x000000, 50);
		}
		else {
			setShade(0x000000, 0);
		}
	}

};
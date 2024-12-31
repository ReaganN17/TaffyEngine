#pragma once

#include "../../utils/Utils.h"

#include "../../base/Input.h"

#include "Object.h"

//Button Type
enum ButtonType : u8 {
	TOGGLE, //Toggle; Changes to opposite state when pressed
	HELD, //Held; down if down
	PULSE, //Pulse; down if pressed
	HOVER //Hover; down if hovered
};

/*
ScreenButton Class; inherits Object Class

A screen button for user mouse
*/
class ScreenButton : public Object {
	friend class ObjectHandler;

	Button_State state;
	bool disabled = false;
	ButtonType type = HELD;
	u8 button = VK_LBUTTON;
	
	//Register button updates
	void update();

public:

	//Default Constructor
	ScreenButton();

	/**
	* Constructor
	* 
	* @param Object Constructor
	* @param Button Type
	*/
	ScreenButton(Object object, ButtonType type);

	//@return If down
	bool is_down();

	//@return If pressed
	bool is_pressed();

	//@return If released
	bool is_released();

	//@return If mouse hovers over
	bool is_hovered();

	//Disables the button from updating down states
	void disable();

	//Enables the button to update down states
	void enable();

	//Sets the button to not down
	void reset();
protected:

	//Periodic
	virtual void periodic();
};
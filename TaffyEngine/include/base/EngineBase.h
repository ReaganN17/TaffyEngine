#pragma once

#include "../utils/Utils.h"
#include "Win32Render.h"
#include "Win32Window.h"
#include "controls/Controls.h"
#include "../handlers/EventHandler.h"
#include "../handlers/ObjectHandler.h"
#include "../handlers/SpriteHandler.h"

/*
EngineBase Class

Runs the Engine and everything with it
*/
class EngineBase final {

private:
    static std::function<void()> init_outside;
    static std::function<void()> loop_outside;

    //Disables construction
    EngineBase() = delete;

    //Initializes the Engine; ie initializes Window, Controls, etc
    static void initialize();

    //Engine loop method; ie renders objects, run events, update controls, etc
    static void execute();

public:

    /**Starts and runs the engine
    *
    * @param initialize method
    * @param loop method
    */
    static void run(std::function<void()> init, std::function<void()> loop);

    //Ends the Engine and closes out the Window
    static void end();
};
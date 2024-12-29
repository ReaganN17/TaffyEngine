#pragma once

#include "../utils/Utils.h"
#include "../../include/base/controls/Controls.h"
#include "../../include/base/Win32Render.h"
#include "../../include/base/Win32Window.h"
#include "../../include/handlers/EventHandler.h"
#include "../../include/handlers/ObjectHandler.h"
#include "../../include/handlers/SpriteHandler.h"

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
public:
    //Initializes the Engine; ie initializes Window, Controls, etc
    static void initialize();

    //Engine loop method; ie renders objects, run events, update controls, etc
    static void execute();

    /**Starts and runs the engine
    * 
    * @param initialize method
    * @param loop method
    */
    static void run(std::function<void()> init, std::function<void()> loop);
}
#pragma once

namespace EngineBase {
    extern std::function<void()> init_outside;
    extern std::function<void()> loop_outside;

    //Initializes the Engine; ie initializes Window, Controls, etc
    void initialize();

    //Engine loop method; ie renders objects, run events, update controls, etc
    void execute();

    /**Starts and runs the engine
    * 
    * @param initialize method
    * @param loop method
    */
    void run(std::function<void()> init, std::function<void()> loop);
}
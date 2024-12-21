#pragma once

namespace EngineBase {
    extern std::function<void()> init_outside;
    extern std::function<void()> loop_outside;

    void initialize();
    void execute();
    void run(std::function<void()> init, std::function<void()> loop);
}
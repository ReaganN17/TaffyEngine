#pragma once

namespace EngineBase {
    extern void (*init)();
    extern void (*loop)();

    void initialize();
    void execute();
    void run(void (*init)(), void (*loop)());
}
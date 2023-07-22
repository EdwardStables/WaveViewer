#pragma once
#include "store.h"
#include "olcPixelGameEngine.h"

class WaveViewer : public olc::PixelGameEngine {
    Store* store;
public:
    WaveViewer(Store* store) : store(store) {
        sAppName = "WaveViewer";
    }

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

#pragma once
#include "store.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "view_manager.h"
class WaveViewer : public olc::PixelGameEngine {
    Store* store;
    Manager manager;

public:
    WaveViewer(Store* store)
        : store(store), manager(Manager(*this))
    {
        sAppName = "WaveViewer";
    }

public: //pge
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;


public: //api

};

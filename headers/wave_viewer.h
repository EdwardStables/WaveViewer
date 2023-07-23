#pragma once
#include "store.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "pane_base.h"
class WaveViewer : public olc::PixelGameEngine {
    Store* store;
    PaneManager manager;

public:
    WaveViewer(Store* store)
        : store(store), manager(PaneManager(*this))
    {
        sAppName = "WaveViewer";
    }

public: //pge
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;


public: //api

};

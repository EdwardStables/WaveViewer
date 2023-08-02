#pragma once
#include "store.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "pane_base.h"
#include "manager.h"
#include <filesystem>

namespace fs = std::filesystem;
class WaveViewer : public olc::PixelGameEngine {
    Store* store;
    PaneManager pane_manager;
    Manager manager;

public:
    WaveViewer(fs::path vcd_path)
        : pane_manager(PaneManager(*this)), manager(vcd_path)
    {
        sAppName = "WaveViewer";
    }

public: //pge
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;


public: //api

};

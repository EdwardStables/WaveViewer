#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "panes.h"
#include <cstdint>
#include <vector>

class Manager {
    std::vector<Pane*> pane_stack;
    olc::PixelGameEngine& pge;

public:
    
    Manager(olc::PixelGameEngine& pge) : pge(pge) {}
    void add_pane(Pane* window, olc::vi2d initial_pos);

    void update(float t);
    void draw();
};

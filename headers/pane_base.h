#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include <cstdint>
#include <vector>

class Manager;

class Pane {
public: //temp
    olc::vi2d size;
    olc::Pixel fill_colour = olc::BLACK;
    olc::Pixel border_colour = olc::WHITE;
public:
    olc::TransformedView* tv;
    Manager* manager;
    Pane(Manager* manager, olc::TransformedView* tv) : manager(manager), tv(tv) {};
    Pane(Manager* manager) : manager(manager), tv(nullptr) {};
    virtual void update(float t);
    virtual void draw();
    virtual void draw_frame() final;
    virtual bool point_in_bb(olc::vi2d) final;
    virtual void set_tv(olc::TransformedView* tv) final;
    virtual void set_size(olc::vi2d) final;
    virtual void set_pos(olc::vi2d) final;
    virtual olc::vi2d get_mpos() final;
    virtual olc::vf2d get_pos() final;
    virtual olc::vi2d get_size() final;
};


class PaneManager {
    std::vector<Pane*> pane_stack;
    olc::PixelGameEngine& pge;

public:
    
    PaneManager(olc::PixelGameEngine& pge) : pge(pge) {}
    void add_pane(Pane* window, olc::vi2d initial_pos);

    void update(float t);
    void draw();
};

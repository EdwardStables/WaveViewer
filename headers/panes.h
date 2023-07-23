#pragma once

#include "olcPGEX_TransformedView.h"

class Pane {
    olc::TransformedView* tv;
    olc::vi2d size;
public: //temp
    olc::Pixel fill_colour = olc::BLACK;
    olc::Pixel border_colour = olc::WHITE;
public:
    Pane(olc::TransformedView* tv) : tv(tv) {};
    Pane() : tv(nullptr) {};
    virtual void update(float t);
    virtual void draw();
    virtual void draw_frame() final;
    virtual bool point_in_bb(olc::vi2d) final;
    virtual void set_tv(olc::TransformedView* tv) final;
    virtual void set_size(olc::vi2d) final;
    virtual void set_pos(olc::vi2d) final;
};

class Heirarchy : public Pane {
public:
    Heirarchy(olc::TransformedView* tv) : Pane(tv) {};
    Heirarchy() : Pane() {};
    virtual void update(float t) override;
    virtual void draw() override;
};

#pragma once

#include "store.h"
#include "olcPGEX_TransformedView.h"

class Pane {
public: //temp
    olc::vi2d size;
    olc::Pixel fill_colour = olc::BLACK;
    olc::Pixel border_colour = olc::WHITE;
public:
    olc::TransformedView* tv;
    Pane(olc::TransformedView* tv) : tv(tv) {};
    Pane() : tv(nullptr) {};
    virtual void update(float t);
    virtual void draw();
    virtual void draw_frame() final;
    virtual bool point_in_bb(olc::vi2d) final;
    virtual void set_tv(olc::TransformedView* tv) final;
    virtual void set_size(olc::vi2d) final;
    virtual void set_pos(olc::vi2d) final;
    virtual olc::vi2d get_mpos() final;
};

class Heirarchy : public Pane {
    Store* store;
    int selected_row;
    float scale_factor = 2.0f;
public:
    Heirarchy(olc::TransformedView* tv, Store* store) : Pane(tv), store(store) {};
    Heirarchy(Store* store) : Pane(), store(store) {};
    virtual void update(float t) override;
    virtual void draw() override;

    void draw_tree(int& row, int depth, Scope* current_scope);
};

#pragma once

#include "pane_base.h"
#include "store.h"
#include "olcPGEX_TransformedView.h"
#include "olcPGEX_QuickGUI.h"

class Manager;

class Heirarchy : public Pane {
    Store* store;
    float scale_factor = 2.0f;
    int selected_row = -1;
    std::vector<std::tuple<int,Scope*,bool>> draw_list;
public:
    Heirarchy(Manager* manager, olc::TransformedView* tv, Store* store) : Pane(manager, tv), store(store) {};
    Heirarchy(Manager* manager, Store* store) : Pane(manager), store(store) {};
    virtual void update(float t) override;
    virtual void draw() override;

    void build_draw_list(int& row, int depth, Scope* current_scope);
};

class WaveList : public Pane {
    Scope* scope = nullptr;
    int selected_row = -1;
    float scale_factor = 2.0f;
    std::vector<std::tuple<Var*,bool>> draw_list;
public:
    WaveList(Manager* manager, olc::TransformedView* tv, Store* store) : Pane(manager, tv) {};
    WaveList(Manager* manager) : Pane(manager) {};
    virtual void update(float t) override;
    virtual void draw() override;
    void set_scope(Scope*);
};

class WavePane : public Pane {
    olc::QuickGUI::Manager guiManager;
    olc::QuickGUI::Slider* min_slider = nullptr;
    olc::QuickGUI::Slider* max_slider = nullptr;
    std::vector<Var*> waves;

    int min_time_limit = 0;
    int max_time_limit = 2300;
    int min_time = min_time_limit;
    int max_time = max_time_limit;

    float scale_factor = 2.0f;
    int gap = 4;
    int wave_x = 100;
    int wave_width;

public:
    WavePane(Manager* manager, olc::TransformedView* tv) : Pane(manager, tv) {};
    WavePane(Manager* manager) : Pane(manager) {
        min_slider = new olc::QuickGUI::Slider(guiManager, { 600.0f, 800.0f }, { 900.0f, 800.0f }, min_time_limit, max_time_limit, min_time_limit);
        max_slider = new olc::QuickGUI::Slider(guiManager, { 1000.0f, 800.0f }, { 1300.0f, 800.0f }, min_time_limit, max_time_limit, max_time_limit);
    };
    virtual void update(float t) override;
    virtual void draw() override;
    void add_wave(Var* var);

private:
    void render_single_bit_wave(Var*& w, olc::vf2d row_start);
    void render_single_bit_line_segment(Var*& w, olc::vf2d row_start);
    void render_single_bit_line_segment(
        BitVector* value, int time, BitVector*& last_value, int& last_time, olc::vf2d row_start
    );
};

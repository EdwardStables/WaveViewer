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
    void set_scope(Scope*, bool add_waves=false);
};

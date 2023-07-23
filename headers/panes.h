#pragma once

#include "pane_base.h"
#include "store.h"
#include "olcPGEX_TransformedView.h"

class Manager;

class Heirarchy : public Pane {
    Store* store;
    int hovered_row;
    int selected_row;
    Scope* hovered_scope = nullptr;
    Scope* selected_scope = nullptr;
    float scale_factor = 2.0f;
public:
    Heirarchy(Manager* manager, olc::TransformedView* tv, Store* store) : Pane(manager, tv), store(store) {};
    Heirarchy(Manager* manager, Store* store) : Pane(manager), store(store) {};
    virtual void update(float t) override;
    virtual void draw() override;

    void draw_tree(int& row, int depth, Scope* current_scope);
};

class WaveList : public Pane {
    Scope* scope = nullptr;
    int selected_row;
    int hovered_row;
    Var* hovered_var = nullptr;
    Var* selected_var = nullptr;
    float scale_factor = 2.0f;
public:
    WaveList(Manager* manager, olc::TransformedView* tv, Store* store) : Pane(manager, tv) {};
    WaveList(Manager* manager) : Pane(manager) {};
    virtual void update(float t) override;
    virtual void draw() override;
    void set_scope(Scope*);
};

class WavePane : public Pane {
    std::vector<std::string> waves;
public:
    WavePane(Manager* manager, olc::TransformedView* tv) : Pane(manager, tv) {};
    WavePane(Manager* manager) : Pane(manager) {};
    virtual void update(float t) override;
    virtual void draw() override;
    void add_wave(Var* var);
};

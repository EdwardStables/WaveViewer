#pragma once

#include "pane_base.h"
#include "store.h"
#include "olcPGEX_TransformedView.h"

class Heirarchy : public Pane {
    Store* store;
    int hovered_row;
    int selected_row;
    Scope* hovered_scope = nullptr;
    Scope* selected_scope = nullptr;
    float scale_factor = 2.0f;
public:
    Heirarchy(olc::TransformedView* tv, Store* store) : Pane(tv), store(store) {};
    Heirarchy(Store* store) : Pane(), store(store) {};
    virtual void update(float t) override;
    virtual void draw() override;

    void draw_tree(int& row, int depth, Scope* current_scope);
    Scope* get_selected_scope();
};

class WaveList : public Pane {
    Scope* scope = nullptr;
    int selected_row;
    int hovered_row;
    Var* hovered_var = nullptr;
    Var* selected_var = nullptr;
    float scale_factor = 2.0f;
public:
    WaveList(olc::TransformedView* tv, Store* store) : Pane(tv) {};
    WaveList() : Pane() {};
    virtual void update(float t) override;
    virtual void draw() override;
    void set_scope(Scope*);
    Var* get_selected_var();
};

class WavePane : public Pane {
    std::vector<std::string> waves;
public:
    WavePane(olc::TransformedView* tv) : Pane(tv) {};
    WavePane() : Pane() {};
    virtual void update(float t) override;
    virtual void draw() override;
    void add_wave(Var* var);
};

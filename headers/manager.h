#pragma once

#include "panes.h"
#include "store.h"
#include "wave_pane.h"
#include <filesystem>

namespace fs = std::filesystem;
struct Manager {

    fs::path vcd_file;
    Store* store;

    Heirarchy* heirarchy;
    WaveList* wave_list;
    WavePane* wave_pane;

    Manager(fs::path vcd_file) : vcd_file(vcd_file) {}
    void load_store();
    void reload_store();
    void heirarchy_focus_scope(Scope* scope, bool add_waves=false);
    void wave_list_select_var(Var* var);
};
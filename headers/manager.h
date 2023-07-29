#pragma once

#include "panes.h"
#include "store.h"
#include "wave_pane.h"

struct Manager {
    Heirarchy* heirarchy;
    WaveList* wave_list;
    WavePane* wave_pane;

    void heirarchy_focus_scope(Scope* scope, bool add_waves=false);
    void wave_list_select_var(Var* var);
};
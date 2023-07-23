#include "manager.h"

void Manager::heirarchy_focus_scope(Scope* scope) {
    wave_list->set_scope(scope); 
}

void Manager::wave_list_select_var(Var* var) {
    wave_pane->add_wave(var);
}
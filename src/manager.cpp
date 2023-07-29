#include "manager.h"

void Manager::heirarchy_focus_scope(Scope* scope, bool add_waves) {
    wave_list->set_scope(scope, add_waves); 
}

void Manager::wave_list_select_var(Var* var) {
    wave_pane->add_wave(var);
}
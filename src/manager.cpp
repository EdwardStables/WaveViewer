#include "manager.h"

void Manager::reload_store() {
    Store* old_store = store;
    load_store();

    heirarchy->reload_waves(store);
    wave_list->reload_waves(store);
    wave_pane->reload_waves(store);

    delete old_store;
}

void Manager::load_store() {
    store = build_store(vcd_file);
    store->extend_all_to_zero();    
}

void Manager::heirarchy_focus_scope(Scope* scope, bool add_waves) {
    wave_list->set_scope(scope, add_waves); 
}

void Manager::wave_list_select_var(Var* var) {
    wave_pane->add_wave(var);
}
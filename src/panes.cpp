#include "panes.h"
#include "manager.h"
#include "math.h"

void Heirarchy::update(float t) {
    //TODO: don't need to clear this every frame
    int saved_selected = selected_row;
    draw_list.clear();    
    int row = 0;
    build_draw_list(row, 0, store->top_scope);
    if (saved_selected != -1 && saved_selected < draw_list.size()){
        std::get<2>(draw_list[saved_selected]) = true;
    }

    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        int hovered_row = mpos.y / (8*scale_factor);
        if (hovered_row < draw_list.size()){
            std::get<2>(draw_list[hovered_row]) = true;
            if(tv->GetPGE()->GetMouse(0).bPressed){
                bool add_all_waves = tv->GetPGE()->GetKey(olc::SHIFT).bHeld;
                manager->heirarchy_focus_scope(std::get<1>(draw_list[hovered_row]), add_all_waves);
                selected_row = hovered_row;
            }
        }
    }
}

void Heirarchy::draw() {
    draw_frame();
    int row = 0;
    for (auto [depth, scope, highlight] : draw_list){
        float width = size.x - depth * 8 * scale_factor;
        if (highlight){
            tv->FillRectDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                {width, 8*scale_factor}, olc::WHITE
            );
            tv->DrawStringDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                scope->name, olc::BLACK, {scale_factor, scale_factor}
            );
        } else {
            tv->DrawStringDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                scope->name, olc::WHITE, {scale_factor, scale_factor}
            );
        }
        row++;
    }
}

void Heirarchy::build_draw_list(int& row, int depth, Scope* scope) {
    row++;

    draw_list.push_back(std::make_tuple(depth, scope, false));

    if (scope->child_scopes.size()){
        depth++;
    }

    for (auto c : scope->child_scopes){
        build_draw_list(row, depth, c.second);
    }
}

void Heirarchy::reload_waves(Store* store) {
    this->store = store;
    draw_list.clear();    
}

void WaveList::update(float t) {
    if (!scope) return;

    //TODO: don't need to clear this every frame
    int saved_selected = selected_row;
    draw_list.clear();    
    
    for (auto [name, var] : scope->identifier_to_var){
        draw_list.push_back(std::make_tuple(var, false));
    }

    if (saved_selected != -1 && saved_selected < draw_list.size()){
        std::get<1>(draw_list[saved_selected]) = true;
    }


    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        int hovered_row = mpos.y / (8*scale_factor);
        if (hovered_row < draw_list.size()){
            std::get<1>(draw_list[hovered_row]) = true;
            if(tv->GetPGE()->GetMouse(0).bPressed){
                manager->wave_list_select_var(std::get<0>(draw_list[hovered_row]));
                selected_row = hovered_row;
            }
        }
    }
}

void WaveList::draw() {
    draw_frame();
    if (!scope) return;

    int row = 0;
    for (auto& [var, highlight] : draw_list){
        olc::vf2d start_pos = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        if (highlight){
            tv->FillRectDecal(
                start_pos,
                {float(size.x), 8*scale_factor}, olc::WHITE
            );
            tv->DrawStringDecal(
                start_pos,
                var->identifier, olc::BLACK, {scale_factor, scale_factor}
            );
        } else {
            tv->DrawStringDecal(
                start_pos,
                var->identifier, olc::WHITE, {scale_factor, scale_factor}
            );
        }
        row++;
    }
}

void WaveList::set_scope(Scope* scope, bool add_waves) {
    this->scope = scope;

    if (!add_waves) return;

    for (auto& [i, v] : this->scope->identifier_to_var){
        manager->wave_list_select_var(v);
    }
}

void WaveList::reload_waves(Store* store) {
    draw_list.clear();    


    if (scope != nullptr){
        std::string target_scope = scope->name;
        set_scope(store->find_scope(target_scope));
    }
}
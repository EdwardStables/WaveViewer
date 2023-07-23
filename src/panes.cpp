#include "panes.h"
#include "manager.h"

void Heirarchy::update(float t) {
    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        hovered_row = mpos.y / (8*scale_factor);
        if (tv->GetPGE()->GetMouse(0).bPressed){
            selected_row = hovered_row;
            selected_scope = hovered_scope;
            manager->heirarchy_focus_scope(selected_scope);
        }
    }
}

void Heirarchy::draw() {
    draw_frame();

    int row = 0;
    int depth = 0;
    Scope* s = store->top_scope;
    draw_tree(row, depth, s);
}

void Heirarchy::draw_tree(int& row, int depth, Scope* scope) {
    if (row == hovered_row || row == selected_row){
        float width = size.x - depth * 8 * scale_factor;
        hovered_scope = scope;
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

    if (scope->child_scopes.size()){
        depth++;
    }

    for (auto c : scope->child_scopes){
        draw_tree(row, depth, c.second);
    }
}

void WaveList::update(float t) {
    auto mpos = get_mpos();

    if (point_in_bb(mpos)){
        hovered_row = mpos.y / (8*scale_factor);

        if (tv->GetPGE()->GetMouse(0).bPressed){
            selected_row = hovered_row;
            selected_var = hovered_var;
            manager->wave_list_select_var(selected_var);
        }
    }
}

void WaveList::draw() {
    draw_frame();
    if (!scope) return;

    int row = 0;
    for (auto& [name, var] : scope->identifier_to_var){
        olc::vf2d start_pos = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        hovered_var = var;
        if (row == hovered_row || row == selected_row){
            tv->FillRectDecal(
                start_pos,
                {float(size.x), 8*scale_factor}, olc::WHITE
            );
            tv->DrawStringDecal(
                start_pos,
                name, olc::BLACK, {scale_factor, scale_factor}
            );
        } else {
            tv->DrawStringDecal(
                start_pos,
                name, olc::WHITE, {scale_factor, scale_factor}
            );
        }
        row++;
    }
}

void WaveList::set_scope(Scope* scope) {
    this->scope = scope;
}

void WavePane::update(float t) {

}

void WavePane::draw() {
    draw_frame();
    for (auto& w : waves)
        std::cout << w << " ";
    std::cout << std::endl;
}

void WavePane::add_wave(Var* var) {
    std::cout << var->identifier << std::endl;
    waves.push_back(var->identifier);
}
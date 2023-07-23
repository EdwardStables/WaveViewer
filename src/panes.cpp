#include "panes.h"

void Pane::set_tv(olc::TransformedView* tv) {
    this->tv = tv;
}

void Pane::set_size(olc::vi2d size) {
    this->size = size;
}

void Pane::set_pos(olc::vi2d pos) {
    if (!tv) return;
    tv->SetWorldOffset(-pos);
}

bool Pane::point_in_bb(olc::vi2d p) {
    if (!tv) return false;
    return p.x >= 0 && p.y >= 0 &&
           p.x < size.x && p.y < size.y;
}

void Pane::draw_frame() {
    if (!tv) return;
    tv->FillRectDecal({0,0}, size, fill_colour);
    tv->DrawRectDecal({0,0}, size, border_colour);
}

void Pane::update(float t) {
}

void Pane::draw() {
    draw_frame();
}

olc::vi2d Pane::get_mpos() {
    return olc::vi2d(tv->ScreenToWorld(tv->GetPGE()->GetMousePos()));
}

olc::vf2d Pane::get_pos() {
    return tv->GetWorldOffset();
}

olc::vi2d Pane::get_size() {
    return size; 
}

void Heirarchy::update(float t) {
    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        hovered_row = mpos.y / (8*scale_factor);
        if (tv->GetPGE()->GetMouse(0).bPressed){
            selected_row = hovered_row;
            selected_scope = hovered_scope;
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

Scope* Heirarchy::get_selected_scope() {
    return selected_scope;
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

Var* WaveList::get_selected_var() {
    Var* s = selected_var;
    selected_var = nullptr;
    return s;
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
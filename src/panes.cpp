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
    olc::vi2d window_pos = tv->ScreenToWorld(p);
    return window_pos.x >= 0 && window_pos.y >= 0 &&
            window_pos.x < size.x && window_pos.y < size.y;
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
        selected_row = mpos.y / (8*scale_factor);
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
    if (row == selected_row){
        float width = size.x - depth * 8 * scale_factor;
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
        selected_row = mpos.y / (8*scale_factor);
    }
}

void WaveList::draw() {
    draw_frame();
    if (!scope) return;

    int row = 0;
    for (auto& [name, var] : scope->identifier_to_var){
        olc::vf2d start_pos = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        if (row == selected_row){
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
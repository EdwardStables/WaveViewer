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


void Heirarchy::update(float t) {

}

void Heirarchy::draw() {
    draw_frame();
}

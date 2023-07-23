#include "panes.h"
#include "pane_base.h"

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

void PaneManager::add_pane(Pane* pane, olc::vi2d initial_pos) {
    olc::TransformedView* tv = new olc::TransformedView();
    pane->set_tv(tv);
    pane->set_pos(initial_pos);
    pane_stack.push_back(pane);
}

void PaneManager::update(float t) {
    for (auto w : pane_stack){
        w->update(t);
    }

/* Pane overlap testing */
//    for (auto p : pane_stack) {
//        p->fill_colour = olc::BLACK;
//    }
//
//    if (pge.GetMouse(0).bHeld){
//        auto mpos = pge.GetMousePos();
//        for (int i = pane_stack.size()-1; i >= 0; i--){
//            if (pane_stack[i]->point_in_bb(mpos)){
//                pane_stack[i]->fill_colour = olc::GREEN;
//                break;
//            }
//        }
//    }
}

void PaneManager::draw() {
    for (auto p : pane_stack) {
        p->draw();
    }
}
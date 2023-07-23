#include "view_manager.h"

void Manager::add_pane(Pane* pane, olc::vi2d initial_pos) {
    olc::TransformedView* tv = new olc::TransformedView();
    pane->set_tv(tv);
    pane->set_pos(initial_pos);
    pane_stack.push_back(pane);
}

void Manager::update(float t) {
    for (auto w : pane_stack){
        w->update(t);
    }

    for (auto p : pane_stack) {
        p->fill_colour = olc::BLACK;
    }

    if (pge.GetMouse(0).bHeld){
        auto mpos = pge.GetMousePos();
        for (int i = pane_stack.size()-1; i >= 0; i--){
            if (pane_stack[i]->point_in_bb(mpos)){
                pane_stack[i]->fill_colour = olc::GREEN;
                break;
            }
        }
    }
}

void Manager::draw() {
    for (auto p : pane_stack) {
        p->draw();
    }
}
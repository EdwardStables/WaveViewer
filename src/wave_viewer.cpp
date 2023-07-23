#include "wave_viewer.h"
#include "panes.h"

bool WaveViewer::OnUserCreate() {
    //heirarchy display
    Heirarchy* heir = new Heirarchy(store);
    olc::vf2d heir_size = GetScreenSize()-olc::vi2d(2,2);
    heir_size.x *= 0.2f;
    heir_size.y *= 0.5f;
    heir->set_size(heir_size);
    manager.add_pane(heir, {1,1});

    //wave list
    WaveList* sigs = new WaveList();
    olc::vf2d sigs_size = GetScreenSize()-olc::vi2d(2,2);
    sigs_size.x *= 0.2f;
    sigs_size.y *= 0.5f;
    sigs->set_size(sigs_size);
    sigs->set_scope(store->top_scope->child_scopes["t1"]);
    manager.add_pane(sigs, {1, heir->get_size().y + heir->get_pos().y + 1});

	return true;
}

bool WaveViewer::OnUserUpdate(float fElapsedTime) {
    manager.update(fElapsedTime);
    manager.draw();

	return true;
}
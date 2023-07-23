#include "wave_viewer.h"
#include "panes.h"

bool WaveViewer::OnUserCreate() {
    //test panes
    Pane* p1 = new Heirarchy(store);
    
    olc::vf2d p1_size = GetScreenSize()-olc::vi2d(2,2);
    p1_size.x *= 0.2f;
    p1->set_size(p1_size);
    manager.add_pane(p1, {1,1});

	return true;
}

bool WaveViewer::OnUserUpdate(float fElapsedTime) {
    manager.update(fElapsedTime);
    manager.draw();

	return true;
}
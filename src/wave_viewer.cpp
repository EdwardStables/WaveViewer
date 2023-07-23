#include "wave_viewer.h"
#include "panes.h"

bool WaveViewer::OnUserCreate() {
    //test panes
    Pane* p1 = new Pane();
    p1->set_size({100,100});
    manager.add_pane(p1, {30,30});

    Pane* p2 = new Pane();
    p2->set_size({200,200});
    manager.add_pane(p2, {60,60});

	return true;
}

bool WaveViewer::OnUserUpdate(float fElapsedTime) {
    manager.update(fElapsedTime);
    manager.draw();

	return true;
}
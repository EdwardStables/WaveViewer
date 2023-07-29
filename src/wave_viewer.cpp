#include "wave_viewer.h"
#include "panes.h"
#include "manager.h"
#include "wave_pane.h"

bool WaveViewer::OnUserCreate() {

    //heirarchy display
    Heirarchy* heir = new Heirarchy(&manager, store);
    olc::vf2d heir_size = GetScreenSize()-olc::vi2d(2,2);
    heir_size.x *= 0.2f;
    heir_size.y *= 0.5f;
    heir->set_size(heir_size);
    pane_manager.add_pane(heir, {1,1});
    manager.heirarchy = heir;

    //wave list
    WaveList* sigs = new WaveList(&manager);
    olc::vf2d sigs_size = GetScreenSize()-olc::vi2d(2,2);
    sigs_size.x *= 0.2f;
    sigs_size.y *= 0.5f;
    sigs->set_size(sigs_size);
    pane_manager.add_pane(sigs, {1, heir->get_size().y + int(heir->get_pos().y) + 1});
    manager.wave_list = sigs;

    //wave pane
    WavePane* waves = new WavePane(&manager);
    olc::vf2d waves_size = GetScreenSize()-olc::vi2d(2,3);
    waves_size.x *= 0.8f;
    waves->set_size(waves_size);
    pane_manager.add_pane(waves, {heir->get_size().x + 1, 1});
    manager.wave_pane = waves;

	return true;
}

bool WaveViewer::OnUserUpdate(float fElapsedTime) {
    pane_manager.update(fElapsedTime);
    pane_manager.draw();

	return true;
}
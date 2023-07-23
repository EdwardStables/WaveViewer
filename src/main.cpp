#include <iostream>
#include "store.h"
#include "wave_viewer.h"

int main() {
    Store* s = build_store("../../vcd_parse/samples/spec_example.vcd");
    WaveViewer* wave_viewer = new WaveViewer(s);

    if (wave_viewer->Construct(1920, 1080, 1, 1))
        wave_viewer->Start();
	return 0;
}
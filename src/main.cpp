#include <iostream>
#include "store.h"
#include "wave_viewer.h"

int main() {
    Store* s = build_store("../../vcd_parse/samples/wikipedia_example.vcd");
    WaveViewer* wave_viewer = new WaveViewer(s);

    if (wave_viewer->Construct(256, 240, 4, 4))
        wave_viewer->Start();
	return 0;
}
#include <iostream>
#include "store.h"
#include "wave_viewer.h"
#include "getopt.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char **argv) {

    fs::path vcd_file;

    int opt;
    while ((opt = getopt(argc, argv, "f:h")) != -1){
        switch(opt){
            case 'f': vcd_file = optarg; break;
            case 'h':
                std::cout << "This is a help message that hasn't been written yet :)" << std::endl;
                break;
        }
    }

    if (!fs::exists(vcd_file)) {
        std::cout << "VCD file " << vcd_file << " not found." << std::endl;
        exit(1);
    }


    Store* s = build_store(vcd_file);
    s->extend_all_to_zero();    

    WaveViewer* wave_viewer = new WaveViewer(s);

    if (wave_viewer->Construct(1920, 1080, 1, 1))
        wave_viewer->Start();
	return 0;
}
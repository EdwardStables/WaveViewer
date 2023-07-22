#include "wave_viewer.h"

bool WaveViewer::OnUserCreate() {
	return true;
}

bool WaveViewer::OnUserUpdate(float fElapsedTime) {
	for (int x = 0; x < ScreenWidth(); x++)
		for (int y = 0; y < ScreenHeight(); y++)
			Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
	return true;
}
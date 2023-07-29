#pragma once

#include "pane_base.h"
#include "store.h"
#include "olcPGEX_TransformedView.h"

class WavePane : public Pane {
    std::vector<Var*> waves;
    int scroll_start_index = 0;

    enum {
        NAMES_AND_WAVES, VALUES_AND_WAVES
    } display_mode = NAMES_AND_WAVES;

    int min_time_limit = 0;
    int max_time_limit = 2300;
    int min_time = min_time_limit;
    int max_time = max_time_limit;

    float scale_factor = 2.0f;
    int gap = 4;
    //initial offsets
    int wave_x = 100;
    int wave_y = 20;
    int wave_width;

    //cursor
    int cursor_time = min_time;
    
    //zoom
    enum {
        NONE, FIRST_SELECTED, SECOND_SELECTED
    } zoom_select_state = NONE;
    int zoom_cancel_width = 10;
    int minimum_time_width = 10;
    int grabbed_position_first;
    int grabbed_position_second;

public:
    WavePane(Manager* manager, olc::TransformedView* tv) : Pane(manager, tv) {};
    WavePane(Manager* manager) : Pane(manager) {};
    virtual void update(float t) override;
    virtual void draw() override;
    void add_wave(Var* var);
    void reset_zoom();
    void set_cursor(int time);
    int get_cursor();

    //drawing
private:
    void draw_timeline();
    void draw_waves();
    void draw_zoom();
    void draw_cursor();
    void render_wave(Var*& w, olc::vf2d row_start);
    void render_line_segment(
        BitVector* value, int time, BitVector*& last_value, int& last_time, olc::vf2d row_start
    );
    void render_single_bit_line_segment(
        olc::Pixel colour, 
        BitVector* last_value, olc::vi2d draw_start, olc::vi2d draw_stop,
        BitVector::Bit last, BitVector::Bit curr, olc::vi2d row_start
    );
    void render_vector_line_segment(olc::Pixel colour, BitVector* last_value, olc::vi2d draw_start, olc::vi2d draw_stop);

    //utilities
private:
    int pixel_to_time(int pixel);
    int time_to_pixel(int pixel);
    olc::Pixel get_line_colour(BitVector*& value);
};
#pragma once

#include "pane_base.h"
#include "store.h"
#include "olcPGEX_TransformedView.h"

class WavePane : public Pane {
    std::vector<std::pair<std::string,Var*>> waves;
    int scroll_start_index = 0;

    int selected_index = -1;
    int selected_index_move = -1;

    std::string group_hover = "";

    enum {
        NAMES_AND_WAVES, VALUES_AND_WAVES
    } display_mode = NAMES_AND_WAVES;

    const int min_time_limit = 0;
    const int max_time_limit;
    int min_time = min_time_limit;
    int max_time = max_time_limit;

    float group_space = 12.0f;
    float scale_factor = 2.0f;
    int gap = 4;
    //initial offsets
    int wave_x = 100;
    int wave_y = 20;
    int wave_width;

    //dividor range
    int divider_range = 4;

    //cursor
    bool clamp_cursor_to_center = false;
    int cursor_time = min_time;
    
    //zoom
    enum {
        NONE,
        FIRST_SELECTED_ZOOM, SECOND_SELECTED_ZOOM,
        FIRST_SELECTED_DIVIDER, SECOND_SELECTED_DIVIDER,
        FIRST_SELECTED_SIGNAL, SECOND_SELECTED_SIGNAL,
        FIRST_SELECTED_GROUP, SECOND_SELECTED_GROUP,
    } mouse_select_state = NONE;
    bool divider_hover = false;
    int zoom_cancel_width = 10;
    int minimum_time_width = 10;
    int grabbed_position_first;
    int grabbed_position_second;

public:
    WavePane(Manager* manager, const int max_time, olc::TransformedView* tv) : Pane(manager, tv), max_time_limit(max_time) {};
    WavePane(Manager* manager, const int max_time) : Pane(manager), max_time_limit(max_time) {};
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
    int y_pixel_to_signal(int pixel);
    int pixel_to_time(int pixel);
    int time_to_pixel(int pixel);
    olc::Pixel get_line_colour(BitVector*& value);
};
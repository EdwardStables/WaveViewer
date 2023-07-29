#include "wave_pane.h"

void WavePane::update(float t) {
    wave_width = size.x - wave_x; 
    auto pge = tv->GetPGE();

    /* Default reset key */
    if (pge->GetKey(olc::F).bPressed) reset_zoom();

    /* Draw state update */
    if (pge->GetKey(olc::V).bHeld) {
        display_mode = VALUES_AND_WAVES;
    } else {
        display_mode = NAMES_AND_WAVES;
    }

    /* Scrolling */
    if (pge->GetMouseWheel() != 0 && waves.size()){
        bool up = pge->GetMouseWheel() >= 0;
        if (up){
            scroll_start_index = std::max(0, scroll_start_index-1);
        } else {
            scroll_start_index = std::min(int(waves.size())-1, scroll_start_index+1);
        }
    }

    /* mouse zoom handling */
    //inputs
    bool mdown = tv->GetPGE()->GetMouse(0).bPressed;
    bool mup = tv->GetPGE()->GetMouse(0).bReleased;
    auto mpos = get_mpos();
    
    //hover update
    if (point_in_bb(mpos)){
        divider_hover =
            mpos.x > wave_x - divider_range && mpos.x < wave_x + divider_range &&
            (
                mouse_select_state == NONE ||
                mouse_select_state == FIRST_SELECTED_DIVIDER ||
                mouse_select_state == SECOND_SELECTED_DIVIDER
            )
        ;
    }

    //state update
    if (point_in_bb(mpos)){
        if (mouse_select_state == NONE && mdown){
            if (mpos.x > wave_x - divider_range && mpos.x < wave_x + divider_range)
                mouse_select_state = FIRST_SELECTED_DIVIDER;
            else
                mouse_select_state = FIRST_SELECTED_ZOOM;
            
            if (mouse_select_state == FIRST_SELECTED_ZOOM)
                grabbed_position_first = std::max(mpos.x, wave_x);
            else
                grabbed_position_first = mpos.x;
        }

        if (mouse_select_state == FIRST_SELECTED_ZOOM && mup){
            mouse_select_state = SECOND_SELECTED_ZOOM;
        }

        if (mouse_select_state == FIRST_SELECTED_DIVIDER && mup){
            mouse_select_state = SECOND_SELECTED_DIVIDER;
        }
    }

    if (mouse_select_state == FIRST_SELECTED_ZOOM)
        grabbed_position_second = std::max(mpos.x, wave_x);
    else if (mouse_select_state == FIRST_SELECTED_DIVIDER)
        grabbed_position_second = std::max(mpos.x, 0);


    //state resolution
    if (mouse_select_state == SECOND_SELECTED_ZOOM){
        int provisional_min_time;
        int provisional_max_time;

        if (grabbed_position_first < grabbed_position_second) {
            provisional_min_time = pixel_to_time(grabbed_position_first-wave_x);
            provisional_max_time = pixel_to_time(grabbed_position_second-wave_x);
        } else {
            provisional_min_time = pixel_to_time(grabbed_position_second-wave_x);
            provisional_max_time = pixel_to_time(grabbed_position_first-wave_x);
        }

        //update cursor instead
        if (grabbed_position_first == grabbed_position_second){
            set_cursor(provisional_min_time);
        }
        else if (
            //if the zoom range is very narrow then just cancel
            std::abs(grabbed_position_first - grabbed_position_second) > zoom_cancel_width && 
            //if the new time range is smaller than the minimum time range then just cancel
            provisional_max_time - provisional_min_time > minimum_time_width
        ){
            min_time = provisional_min_time;
            max_time = provisional_max_time;
        }

        mouse_select_state = NONE;
    }

    if (mouse_select_state == FIRST_SELECTED_DIVIDER) {
        wave_x = grabbed_position_second;
    }
    if (mouse_select_state == SECOND_SELECTED_DIVIDER) {
        wave_x = grabbed_position_second;
        mouse_select_state = NONE;
    }

    if (max_time - min_time < minimum_time_width){
        max_time = min_time + minimum_time_width;
    }

}

void WavePane::reset_zoom() {
    min_time = min_time_limit;
    max_time = max_time_limit;
}

int WavePane::pixel_to_time(int pixel) {
    float time_per_pixel = float(max_time - min_time) / std::max(wave_width, 1);
    float time_offset = pixel * time_per_pixel;
    int actual_time = int(time_offset) + min_time;
    return actual_time;
}

int WavePane::time_to_pixel(int time) {
    return (time - min_time)  * wave_width / (max_time - min_time);
}

void WavePane::draw_timeline() {
    int timeline_resolution = std::pow(10, std::floor(std::log10(max_time - min_time)));
    if (((max_time - min_time) / timeline_resolution) < 5) timeline_resolution /= 10;
    int timeline_value = timeline_resolution * int(min_time / timeline_resolution);
    while (timeline_value <= max_time) {
        if (timeline_value >= min_time){
            int pixel = time_to_pixel(timeline_value);
            tv->DrawLineDecal({wave_x + pixel, 0}, {wave_x + pixel, 10});
            tv->DrawStringDecal({wave_x + pixel + 2, 2}, std::to_string(timeline_value));
        }
        timeline_value += timeline_resolution;
    }
}

void WavePane::set_cursor(int time) {
    cursor_time = time;
}

int WavePane::get_cursor() {
    return cursor_time;
}

void WavePane::draw_cursor() {
    if (cursor_time < min_time || cursor_time > max_time) return;

    int pixel = time_to_pixel(cursor_time) + wave_x;

    tv->DrawLineDecal({pixel, 0}, {pixel, size.y});
}

void WavePane::draw_waves() {
    int row = 0;
    for (size_t i = scroll_start_index; i < waves.size(); i++){
        auto w = waves[i];
        olc::vf2d row_start = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        row_start.y += gap*(row+1) + wave_y;

        if (row_start.y > size.y) break;
        
        if (display_mode == NAMES_AND_WAVES)
            tv->DrawStringDecal(
                row_start,
                w->identifier, olc::WHITE, {scale_factor, scale_factor}
            );
        else
            tv->DrawStringDecal(
                row_start + olc::vf2d(0, 4.0f),
                w->value_at(cursor_time)->as_hex_string(), olc::WHITE
            );
        row++;

        render_wave(w, row_start);
    }
}


void WavePane::draw_zoom() {
    if (
        mouse_select_state == NONE ||
        mouse_select_state == FIRST_SELECTED_DIVIDER ||
        mouse_select_state == SECOND_SELECTED_DIVIDER
    ) return;

    olc::vf2d first_top = {float(grabbed_position_first), 0};
    olc::vf2d first_bottom = {float(grabbed_position_first), size.y};
    olc::vf2d second_top = {float(grabbed_position_second), 0};
    olc::vf2d second_bottom = {float(grabbed_position_second), size.y};

    tv->DrawLineDecal(first_top, first_bottom);
    tv->DrawLineDecal(second_top, second_bottom);
    tv->DrawStringDecal(first_bottom - olc::vf2d(0, 24), std::to_string(pixel_to_time(grabbed_position_first-wave_x)));
    tv->DrawStringDecal(second_bottom - olc::vf2d(0, 24), std::to_string(pixel_to_time(grabbed_position_second-wave_x)));
    
    olc::vf2d highlight_size = second_bottom - first_top;
    tv->GetPGE()->SetPixelMode(olc::Pixel::ALPHA);
    tv->FillRectDecal(first_top, highlight_size , olc::Pixel(255, 255, 255, 64));
    tv->GetPGE()->SetPixelMode(olc::Pixel::NORMAL);
}

void WavePane::draw() {
    draw_timeline();

    /* min/max time debug info */
    tv->DrawStringDecal({wave_x, size.y-16}, std::to_string(min_time));
    tv->DrawStringDecal({wave_x + 8*16, size.y-16}, std::to_string(max_time));

    draw_waves();
    draw_zoom();
    draw_cursor();

    /* Name/Wave separator */
    olc::Pixel divider_colour = divider_hover ? olc::YELLOW : olc::WHITE;    
    tv->DrawLineDecal({wave_x, 0}, {wave_x, size.y}, divider_colour);
    tv->DrawRectDecal({0,0}, size, border_colour); //override default frame drawing and do it at the end
}
    
olc::Pixel WavePane::get_line_colour(BitVector*& value) {
    olc::Pixel orange = olc::Pixel(0xFF, 0xA5, 0x00, 0xFF);

    if (value->width() == 1){
        switch((*value)[0]){
            case BitVector::Bit::X:  return olc::RED;
            case BitVector::Bit::Z:  return orange;
            case BitVector::Bit::_0: return olc::GREEN;
            case BitVector::Bit::_1: return olc::GREEN;
        }
    } else {
        //any X makes the whole thing red
        //Z with no X makes it orange
        olc::Pixel colour = olc::GREEN;
        for (int i = 0; i < value->width(); i++){
            if ((*value)[i] == BitVector::Bit::X)
                return olc::RED;
            if ((*value)[i] == BitVector::Bit::Z)
                colour = orange;
        }
        return colour;
    }
}

void WavePane::render_single_bit_line_segment(olc::Pixel colour, BitVector* last_value, olc::vi2d draw_start, olc::vi2d draw_stop, BitVector::Bit last, BitVector::Bit curr, olc::vi2d row_start) {
    tv->DrawLineDecal(draw_start, draw_stop, colour);

    if (last == curr ||
        last == BitVector::Bit::Z && curr == BitVector::Bit::X ||
        last == BitVector::Bit::X && curr == BitVector::Bit::Z
    ) {
        return;
    }

    olc::vi2d start = draw_stop;  
    olc::vi2d stop = start;  
    //line from center
    switch(curr){
        case BitVector::Bit::X:
        case BitVector::Bit::Z:
            stop.y = row_start.y + 4*scale_factor;
            break;
        case BitVector::Bit::_0:
            stop.y = row_start.y + 8*scale_factor;
            break;
        default: //1
            stop.y = row_start.y;
    }

    tv->DrawLineDecal(start, stop, colour);
}

void WavePane::render_vector_line_segment(olc::Pixel colour, BitVector* last_value, olc::vi2d draw_start, olc::vi2d draw_stop) {
    tv->DrawRectDecal(draw_start, draw_stop - draw_start, colour);
    tv->FillRectDecal(draw_start + olc::vf2d(1.0f, 1.0f), draw_stop - draw_start - olc::vf2d(1.0f, 1.0f) , olc::BLACK);
    std::string s = last_value->as_hex_string();
    tv->DrawStringDecal(draw_start + olc::vf2d{2.0f, 4.0f}, s);
}
           
void WavePane::render_wave(Var*& w, olc::vf2d row_start) {
    int last_time = min_time;
    BitVector* last_value = w->value_at(min_time);
    for (auto& [time, value] : w->values) {

        //skip this iteration when the minimum time is not zero
        if (time < min_time){
            last_time = time;
            last_value = value;
            continue;
        }
        render_line_segment(value, time, last_value, last_time, row_start);
    }
    render_line_segment(w->value_at(max_time), max_time, last_value, last_time, row_start);
}

void WavePane::render_line_segment(BitVector* value, int time, BitVector*& last_value, int& last_time, olc::vf2d row_start) {
    bool vector = value->width() != 1;
    int voffset;

    if (vector){
        voffset = 8 * scale_factor;
    } else {
        switch((*last_value)[0]){
            case BitVector::Bit::X:
            case BitVector::Bit::Z:  voffset = 4*scale_factor; break;
            case BitVector::Bit::_0: voffset = 8*scale_factor; break;
            case BitVector::Bit::_1: voffset = 0; break;
        }
    }

    int start_pos = time_to_pixel(last_time);
    int end_pos = time_to_pixel(time);

    olc::vf2d draw_start = row_start + olc::vi2d(wave_x + start_pos, vector ? 0 : voffset);
    olc::vf2d draw_stop = row_start + olc::vi2d(wave_x + end_pos, voffset);

    BitVector::Bit last = (*last_value)[0]; 
    BitVector::Bit curr = (*value)[0]; 


    olc::Pixel colour = get_line_colour(last_value);

    //don't draw hidden segments
    if (draw_stop.x < wave_x || draw_start.x > size.x) {
        return;
    }

    //cut the left side off partial segments
    if (draw_start.x < wave_x) {
        draw_start.x = wave_x;
    }

    //and the right side off partial segments
    if (draw_stop.x > size.x) {
        draw_stop.x = size.x;
    }

    if (vector){
        render_vector_line_segment(colour, last_value, draw_start, draw_stop);
    } else {
        render_single_bit_line_segment(colour, last_value, draw_start, draw_stop, last, curr, row_start);
    }

    //important update
    last_time = time;
    last_value = value;

}

void WavePane::add_wave(Var* var) {
    waves.push_back(var);
}
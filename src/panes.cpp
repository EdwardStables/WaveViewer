#include "panes.h"
#include "manager.h"
#include "math.h"

void Heirarchy::update(float t) {
    //TODO: don't need to clear this every frame
    int saved_selected = selected_row;
    draw_list.clear();    
    int row = 0;
    build_draw_list(row, 0, store->top_scope);
    if (saved_selected != -1 && saved_selected < draw_list.size()){
        std::get<2>(draw_list[saved_selected]) = true;
    }

    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        int hovered_row = mpos.y / (8*scale_factor);
        if (hovered_row < draw_list.size()){
            std::get<2>(draw_list[hovered_row]) = true;
            if(tv->GetPGE()->GetMouse(0).bPressed){
                manager->heirarchy_focus_scope(std::get<1>(draw_list[hovered_row]));
                selected_row = hovered_row;
            }
        }
    }
}

void Heirarchy::draw() {
    draw_frame();
    int row = 0;
    for (auto [depth, scope, highlight] : draw_list){
        float width = size.x - depth * 8 * scale_factor;
        if (highlight){
            tv->FillRectDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                {width, 8*scale_factor}, olc::WHITE
            );
            tv->DrawStringDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                scope->name, olc::BLACK, {scale_factor, scale_factor}
            );
        } else {
            tv->DrawStringDecal(
                olc::vf2d{float(depth),float(row)} * 8 * scale_factor,
                scope->name, olc::WHITE, {scale_factor, scale_factor}
            );
        }
        row++;
    }
}

void Heirarchy::build_draw_list(int& row, int depth, Scope* scope) {
    row++;

    draw_list.push_back(std::make_tuple(depth, scope, false));

    if (scope->child_scopes.size()){
        depth++;
    }

    for (auto c : scope->child_scopes){
        build_draw_list(row, depth, c.second);
    }
}

void WaveList::update(float t) {
    if (!scope) return;

    //TODO: don't need to clear this every frame
    int saved_selected = selected_row;
    draw_list.clear();    
    int row = 0;
    
    for (auto [name, var] : scope->identifier_to_var){
        draw_list.push_back(std::make_tuple(var, false));
    }

    if (saved_selected != -1 && saved_selected < draw_list.size()){
        std::get<1>(draw_list[saved_selected]) = true;
    }


    auto mpos = get_mpos();
    if (point_in_bb(mpos)){
        int hovered_row = mpos.y / (8*scale_factor);
        if (hovered_row < draw_list.size()){
            std::get<1>(draw_list[hovered_row]) = true;
            if(tv->GetPGE()->GetMouse(0).bPressed){
                manager->wave_list_select_var(std::get<0>(draw_list[hovered_row]));
                selected_row = hovered_row;
            }
        }
    }
}

void WaveList::draw() {
    draw_frame();
    if (!scope) return;

    int row = 0;
    for (auto& [var, highlight] : draw_list){
        olc::vf2d start_pos = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        if (highlight){
            tv->FillRectDecal(
                start_pos,
                {float(size.x), 8*scale_factor}, olc::WHITE
            );
            tv->DrawStringDecal(
                start_pos,
                var->identifier, olc::BLACK, {scale_factor, scale_factor}
            );
        } else {
            tv->DrawStringDecal(
                start_pos,
                var->identifier, olc::WHITE, {scale_factor, scale_factor}
            );
        }
        row++;
    }
}

void WaveList::set_scope(Scope* scope) {
    this->scope = scope;
}

void WavePane::update(float t) {
    wave_width = size.x - wave_x; 

    /* Default reset key */
    if (tv->GetPGE()->GetKey(olc::F).bPressed) reset_zoom();

    /* mouse zoom handling */
    //inputs
    bool mdown = tv->GetPGE()->GetMouse(0).bPressed;
    bool mup = tv->GetPGE()->GetMouse(0).bReleased;
    auto mpos = get_mpos();
    
    //state update
    if ((mdown || mup) && point_in_bb(mpos)){
        if (zoom_select_state == NONE && mdown){
            zoom_select_state = FIRST_SELECTED;
            grabbed_position_first = mpos.x;
        }

        if (zoom_select_state == FIRST_SELECTED && mup){
            zoom_select_state = SECOND_SELECTED;
        }
    }

    if (zoom_select_state != NONE){
        grabbed_position_second = mpos.x;
    }


    //state resolution
    if (zoom_select_state == SECOND_SELECTED){
        int provisional_min_time;
        int provisional_max_time;

        if (grabbed_position_first < grabbed_position_second) {
            provisional_min_time = pixel_to_time(grabbed_position_first-wave_x);
            provisional_max_time = pixel_to_time(grabbed_position_second-wave_x);
        } else {
            provisional_min_time = pixel_to_time(grabbed_position_second-wave_x);
            provisional_max_time = pixel_to_time(grabbed_position_first-wave_x);
        }

        if (
            //if the zoom range is very narrow then just cancel
            std::abs(grabbed_position_first - grabbed_position_second) > zoom_cancel_width ||
            //if the new time range is smaller than the minimum time range then just cancel
            provisional_max_time - provisional_min_time < minimum_time_width
        ){
            min_time = provisional_min_time;
            max_time = provisional_max_time;
        }

        zoom_select_state = NONE;
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

void WavePane::draw() {
    /* timeline */
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

    /* min/max time debug info */
    tv->DrawStringDecal({wave_x, size.y-16}, std::to_string(min_time));
    tv->DrawStringDecal({wave_x + 8*16, size.y-16}, std::to_string(max_time));

    /* waves */
    int row = 0;
    for (auto& w : waves){
        olc::vf2d row_start = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        row_start.y += gap*(row+1) + wave_y;
        tv->DrawStringDecal(
            row_start,
            w->identifier, olc::WHITE, {scale_factor, scale_factor}
        );
        row++;

        render_wave(w, row_start);
    }

    /* zoom lines */
    if (zoom_select_state != NONE){
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

    /* Name/Wave separator */
    tv->DrawLineDecal({wave_x, 0}, {wave_x, size.y});
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

#include "panes.h"
#include "manager.h"

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

}

void WavePane::draw() {
    draw_frame();

    int wave_x = 100;
    int min_time = 0;
    int max_time = 2300;
    int wave_width = size.x - wave_x;
    
    int row = 0;
    for (auto& w : waves){
        olc::vf2d row_start = olc::vf2d{0.0f,float(row)} * 8 * scale_factor;
        tv->DrawStringDecal(
            row_start,
            w->identifier, olc::WHITE, {scale_factor, scale_factor}
        );
        row++;

        if (w->size == 1){
            int last_time = min_time;
            BitVector* last_value = w->value_at(min_time);
            for (auto& [time, value] : w->values) {

                olc::Pixel colour;
                int voffset;

                //TODO: should use this
                switch((*last_value)[0]){
                    case BitVector::Bit::X:
                        colour = olc::RED;
                        voffset = 4*scale_factor;
                        break;
                    case BitVector::Bit::Z:
                        colour = olc::Pixel(0xFFA500);//orange
                        voffset = 4*scale_factor;
                        break;
                    case BitVector::Bit::_0:
                        colour = olc::GREEN;
                        voffset = 8*scale_factor;
                        break;
                    case BitVector::Bit::_1:
                        colour = olc::GREEN;
                        voffset = 0;
                        break;
                }

                //time to pix 
                int start_pos = (last_time - min_time)  * wave_width / (max_time - min_time);
                int end_pos = (time - min_time)  * wave_width / (max_time - min_time);

                olc::vf2d draw_start = row_start + olc::vi2d(wave_x + start_pos, voffset);
                olc::vf2d draw_stop = row_start + olc::vi2d(wave_x + end_pos, voffset);

                tv->DrawLineDecal(draw_start, draw_stop, colour);
                
                last_time = time;
                last_value = value;
            }
        }
    }




}

void WavePane::add_wave(Var* var) {
    waves.push_back(var);
}
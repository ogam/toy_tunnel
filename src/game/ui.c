#include "game/ui.h"

f64 joypad_dpad_timestamps[4];
typedef s32 Joypad_Button;
enum
{
    Joypad_Button_Up,
    Joypad_Button_Down,
    Joypad_Button_Left,
    Joypad_Button_Right
};

typedef struct UI_Deadzone
{
    CF_V2 deadzone_x;
    CF_V2 deadzone_y;
    CF_V2 value;
} UI_Deadzone;

void init_ui()
{
    UI* ui = &s_app->ui;
    cf_array_fit(ui->items, 1024);
    cf_array_fit(ui->interactable_items, 1024);
    
    CF_MEMSET(joypad_dpad_timestamps, 0, sizeof(joypad_dpad_timestamps));
}

b32 joypad_dpad_button_repeat(s32 player_index, Joypad_Button dpad_button)
{
    CF_JoypadButton button = CF_JOYPAD_BUTTON_COUNT;
    b32 pressed = false;
    switch (dpad_button)
    {
        case Joypad_Button_Up: button = CF_JOYPAD_BUTTON_DPAD_UP; break;
        case Joypad_Button_Down: button = CF_JOYPAD_BUTTON_DPAD_DOWN; break;
        case Joypad_Button_Left: button = CF_JOYPAD_BUTTON_DPAD_LEFT; break;
        case Joypad_Button_Right: button = CF_JOYPAD_BUTTON_DPAD_RIGHT; break;
    }
    
    if (button != CF_JOYPAD_BUTTON_COUNT)
    {
        if (cf_joypad_button_just_pressed(player_index, button))
        {
            joypad_dpad_timestamps[dpad_button] = CF_SECONDS;
            pressed = true;
        }
        else if (cf_joypad_button_down(player_index, button))
        {
            // cf_key_repeating()
            
            double repeat_delay = 0.5;
            double repeat_rate = 0.035;
            f64 t = joypad_dpad_timestamps[dpad_button] + repeat_delay;
            if (CF_SECONDS > t)
            {
                pressed = cf_on_interval((f32)repeat_rate, (f32)t);
            }
        }
    }
    
    return pressed;
}

void update_ui_input()
{
    UI_Input* input = &s_app->ui.input;
    CF_V2 direction = cf_v2(0, 0);
    b32 select = false;
    b32 menu = false;
    
    if (cf_joypad_count() > 0)
    {
        s32 player_index = 0;
        
        if (joypad_dpad_button_repeat(player_index, Joypad_Button_Up))
        {
            direction.y += 1.0f;
        }
        
        if (joypad_dpad_button_repeat(player_index, Joypad_Button_Down))
        {
            direction.y -= 1.0f;
        }
        
        if (joypad_dpad_button_repeat(player_index, Joypad_Button_Right))
        {
            direction.x += 1.0f;
        }
        
        if (joypad_dpad_button_repeat(player_index, Joypad_Button_Left))
        {
            direction.x -= 1.0f;
        }
        
        select = cf_joypad_button_just_pressed(player_index, CF_JOYPAD_BUTTON_A);
        menu = cf_joypad_button_just_pressed(player_index, CF_JOYPAD_BUTTON_BACK) || cf_joypad_button_just_pressed(player_index, CF_JOYPAD_BUTTON_START);
    }
    
    // keyboard
    {
        if (cf_key_just_pressed(CF_KEY_UP) || cf_key_repeating(CF_KEY_UP))
        {
            direction.y += 1.0f;
        }
        if (cf_key_just_pressed(CF_KEY_DOWN) || cf_key_repeating(CF_KEY_DOWN))
        {
            direction.y -= 1.0f;
        }
        if (cf_key_just_pressed(CF_KEY_RIGHT) || cf_key_repeating(CF_KEY_RIGHT))
        {
            direction.x += 1.0f;
        }
        if (cf_key_just_pressed(CF_KEY_LEFT) || cf_key_repeating(CF_KEY_LEFT))
        {
            direction.x -= 1.0f;
        }
        select = select || cf_key_just_pressed(CF_KEY_Z) || cf_key_just_pressed(CF_KEY_RETURN2) || cf_key_just_pressed(CF_KEY_RETURN);
        menu = menu || cf_key_just_pressed(CF_KEY_ESCAPE);
    }
    
    input->direction = direction;
    input->select = select;
    input->menu = menu;
}

void update_ui()
{
    UI* ui = &s_app->ui;
    
    cf_array_clear(ui->items);
    cf_array_clear(ui->interactable_items);
    
    ui->cursor = cf_v2(0.0f, 0.0f);
    ui->layout = UI_Layout_None;
    ui->interactable_item_counter = 0;
    
    update_ui_input();
    
    if (ui->options_state != UI_Options_State_None)
    {
        switch (ui->options_state)
        {
            case UI_Options_State_Options: ui_do_options(); break;
            case UI_Options_State_Keyboard: ui_do_options_keyboard(); break;
            case UI_Options_State_Controller: ui_do_options_controller(); break;
        }
    }
    else
    {
        switch (s_app->world.state)
        {
            case World_State_None:
            {
                ui_do_main_menu();
                break;
            }
            case World_State_Play:
            {
                ui_do_game();
                break;
            }
            case World_State_Pause:
            {
                ui_do_pause();
                break;
            }
            case World_State_Game_Over:
            {
                ui_do_game_over();
                break;
            }
        }
    }
    
    // validate selection
    UI_Item* next_item = NULL;
    {
        for (s32 index = 0; index < cf_array_count(ui->interactable_items); ++index)
        {
            UI_Item* item = ui->interactable_items + index;
            
            if (item->hash == ui->next_hash)
            {
                next_item = item;
                break;
            }
        }
        
        if (next_item == NULL)
        {
            ui->next_hash = 0;
        }
    }
    
    // navigation
    {
        UI_Item* navigation_items = ui->interactable_items;
        if (next_item == NULL)
        {
            if (cf_array_count(navigation_items) > 0)
            {
                ui->next_hash = navigation_items->hash;
                next_item = navigation_items;
            }
        }
        
        CF_V2 direction = ui->input.direction;
        
        CF_V2 origin = cf_v2(0, 0);
        
        f32 closest_up = F32_MAX;
        f32 closest_down = F32_MAX;
        f32 closest_left = F32_MAX;
        f32 closest_right = F32_MAX;
        
        u64 closest_hash_up = 0;
        u64 closest_hash_down = 0;
        u64 closest_hash_left = 0;
        u64 closest_hash_right = 0;
        
        if (next_item)
        {
            origin = next_item->origin;
        }
        
        for (s32 index = 0; index < cf_array_count(navigation_items); ++index)
        {
            UI_Item* item = navigation_items + index;
            if (item == next_item)
            {
                continue;
            }
            
            CF_V2 item_origin = item->origin;
            CF_V2 dp = cf_sub(item_origin, origin);
            f32 distance_sq = cf_len_sq(dp);
            
            if (cf_dot(dp, cf_v2(0, 1)) > 0)
            {
                if (closest_up > distance_sq)
                {
                    closest_up = distance_sq;
                    closest_hash_up = item->hash;
                }
            }
            
            if (cf_dot(dp, cf_v2(0, -1)) > 0)
            {
                if (closest_down > distance_sq)
                {
                    closest_down = distance_sq;
                    closest_hash_down = item->hash;
                }
            }
            
            if (cf_dot(dp, cf_v2(1, 0)) > 0)
            {
                if (closest_right > distance_sq)
                {
                    closest_right = distance_sq;
                    closest_hash_right = item->hash;
                }
            }
            
            if (cf_dot(dp, cf_v2(-1, 0)) > 0)
            {
                if (closest_left > distance_sq)
                {
                    closest_left = distance_sq;
                    closest_hash_left = item->hash;
                }
            }
        }
        
        if (direction.y > 0)
        {
            if (closest_hash_up)
            {
                ui->next_hash = closest_hash_up;
            }
        }
        else if (direction.y < 0)
        {
            if (closest_hash_down)
            {
                ui->next_hash = closest_hash_down;
            }
        }
        else if (direction.x > 0)
        {
            if (closest_hash_right)
            {
                ui->next_hash = closest_hash_right;
            }
        }
        else if (direction.x < 0)
        {
            if (closest_hash_left)
            {
                ui->next_hash = closest_hash_left;
            }
        }
    }
}

void draw_ui()
{
    UI* ui = &s_app->ui;
    
    cf_draw_push();
    cf_draw_scale_v2(cf_safe_invert(s_app->screen_scale));
    
    for (s32 index = 0; index < cf_array_count(ui->items); ++index)
    {
        UI_Item* item = ui->items + index;
        
        switch (item->type)
        {
            case UI_Item_Type_Text:
            {
                cf_push_font_size(item->font_size);
                cf_draw_push_color(item->text_color);
                
                cf_draw_push();
                cf_draw_TSR(cf_top_left(item->text_aabb), cf_v2(1.0f, 1.0f), 0.0f);
                cf_draw_text(item->text, cf_v2(0, 0), -1);
                cf_draw_pop();
                
                cf_draw_pop_color();
                cf_pop_font_size();
                break;
            }
            case UI_Item_Type_Button:
            {
                cf_draw_push_color(item->background_color);
                cf_draw_box_fill(item->aabb, 0.0f);
                cf_draw_pop_color();
                
                cf_draw_push_color(item->border_color);
                cf_draw_box(item->aabb, 1.0f, 0.0f);
                cf_draw_pop_color();
                
                cf_push_font_size(item->font_size);
                cf_draw_push_color(item->text_color);
                
                cf_draw_push();
                cf_draw_TSR(cf_top_left(item->text_aabb), cf_v2(1.0f, 1.0f), 0.0f);
                cf_draw_text(item->text, cf_v2(0, 0), -1);
                cf_draw_pop();
                
                cf_draw_pop_color();
                cf_pop_font_size();
                break;
            }
            case UI_Item_Type_Slider:
            {
                f32 spacing = 10.0f;
                
                CF_V2 extents = cf_extents(item->text_aabb);
                
                cf_push_font_size(item->font_size);
                cf_draw_push_color(item->text_color);
                
                cf_draw_push();
                cf_draw_TSR(cf_top_left(item->text_aabb), cf_v2(1.0f, 1.0f), 0.0f);
                cf_draw_text(item->text, cf_v2(0, 0), -1);
                
                if (ui->next_hash == item->hash)
                {
                    f32 arrow_width = cf_peek_font_size() * 0.25f;
                    // left arrow
                    {
                        CF_V2 p0 = cf_v2(-spacing, 0.0f);
                        CF_V2 p1 = cf_v2(-spacing - arrow_width, -extents.y * 0.5f);
                        CF_V2 p2 = cf_v2(-spacing, -extents.y);
                        cf_draw_tri_fill(p0, p1, p2, 0.0f);
                    }
                    
                    // right arrow
                    {
                        CF_V2 p0 = cf_v2(spacing + extents.x, 0.0f);
                        CF_V2 p1 = cf_v2(spacing + extents.x + arrow_width, -extents.y * 0.5f);
                        CF_V2 p2 = cf_v2(spacing + extents.x, -extents.y);
                        cf_draw_tri_fill(p0, p1, p2, 0.0f);
                    }
                }
                
                cf_draw_pop();
                
                cf_draw_pop_color();
                cf_pop_font_size();
                
                
                break;
            }
            case UI_Item_Type_Background:
            {
                CF_Aabb aabb = cf_make_aabb_center_half_extents(cf_v2(0, 0), cf_mul_v2_f(s_app->screen_size, 0.5f));
                
                cf_draw_push_color(item->background_color);
                cf_draw_box_fill(aabb, 0.0f);
                cf_draw_pop_color();
                break;
            }
            case UI_Item_Type_Custom:
            {
                item->custom_fn(item);
                break;
            }
        }
    }
    
    cf_draw_pop();
}

const char* ui_text_vfmt(const char* fmt, va_list args)
{
    char buffer[1024];
    u64 length = vsnprintf(buffer, sizeof(buffer), fmt, args);
    
    char* text = NULL;
    if (length > 0)
    {
        text = scratch_alloc(length);
        CF_MEMCPY(text, buffer, length);
        text[length] = '\0';
    }
    
    return text;
}

UI_Item* ui_make_item(UI_Item_Type type, const char* fmt, va_list args)
{
    UI* ui = &s_app->ui;
    
    f32 padding = 5.0f;
    f32 item_padding = 10.0f;
    
    UI_Item new_item = { 0 };
    cf_array_push(ui->items, new_item);
    
    UI_Item* item = &cf_array_last(ui->items);
    item->text = ui_text_vfmt(fmt, args);
    
    CF_V2 text_size = cf_text_size(item->text, -1);
    CF_V2 position = ui->cursor;
    CF_V2 origin = position;
    
    if (BIT_IS_SET_EX(ui->layout, UI_Layout_Left))
    {
        position.x += text_size.x * 0.5f + padding;
    }
    else if (BIT_IS_SET_EX(ui->layout, UI_Layout_Right))
    {
        position.x -= text_size.x * 0.5f + padding;
    }
    
    if (BIT_IS_SET_EX(ui->layout, UI_Layout_Top))
    {
        position.y -= text_size.y * 0.5f + padding;
    }
    else if (BIT_IS_SET_EX(ui->layout, UI_Layout_Bottom))
    {
        position.y += text_size.y * 0.5f + padding;
    }
    
    CF_Aabb text_aabb = cf_make_aabb_center_half_extents(position, cf_mul_v2_f(text_size, 0.5f));
    CF_Aabb aabb = cf_expand_aabb_f(text_aabb, padding);
    
    if (BIT_IS_SET_EX(ui->layout, UI_Layout_Bottom))
    {
        ui->cursor.y += cf_extents(aabb).y + item_padding;
    }
    else
    {
        ui->cursor.y -= cf_extents(aabb).y + item_padding;
    }
    
    if (BIT_IS_SET_EX(ui->layout, UI_Layout_Left))
    {
        origin.x = cf_left(aabb).x;
    }
    else if (BIT_IS_SET_EX(ui->layout, UI_Layout_Right))
    {
        origin.x = cf_right(aabb).x;
    }
    
    if (BIT_IS_SET_EX(ui->layout, UI_Layout_Top))
    {
        origin.y = cf_top(aabb).y;
    }
    else if (BIT_IS_SET_EX(ui->layout, UI_Layout_Bottom))
    {
        origin.y = cf_bottom(aabb).y;
    }
    
    item->text_aabb = text_aabb;
    item->aabb = aabb;
    item->type = type;
    item->origin = origin;
    item->font_size = cf_peek_font_size();
    
    char hash_buffer[4096];
    s32 hash_length = 0;
    switch (type)
    {
        case UI_Item_Type_Text:
        {
            hash_length = snprintf(hash_buffer, sizeof(hash_buffer), "TEXT::%s", item->text);
            break;
        }
        case UI_Item_Type_Button:
        {
            // to keep navigation and selection consistent for buttons use fmt instead of formated text
            hash_length = snprintf(hash_buffer, sizeof(hash_buffer), "BUTTON::%d::%s", ui->interactable_item_counter++, fmt);
            break;
        }
        case UI_Item_Type_Slider:
        {
            hash_length = snprintf(hash_buffer, sizeof(hash_buffer), "SLIDER::%d", ui->interactable_item_counter++);
            break;
        }
    }
    item->hash = cf_fnv1a(hash_buffer, hash_length);
    item->text_color = cf_color_white();
    item->background_color = cf_color_clear();
    item->border_color = cf_color_clear();
    
    if (type == UI_Item_Type_Button || type == UI_Item_Type_Slider)
    {
        cf_array_push(ui->interactable_items, *item);
        item->background_color = cf_color_clear();
        item->border_color = cf_color_clear();
        
        if (item->hash == ui->next_hash)
        {
            item->text_color = cf_color_yellow();
        }
    }
    
    return item;
}

UI_Item* ui_make_item_fmt(UI_Item_Type type, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    UI_Item* item = ui_make_item(type, fmt, args);
    va_end(args);
    
    return item;
}

void ui_set_layout(UI_Layout layout)
{
    s_app->ui.layout = layout;
}

void ui_set_cursor(CF_V2 position)
{
    s_app->ui.cursor = position;
}

void ui_do_text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    UI_Item* item = ui_make_item(UI_Item_Type_Text, fmt, args);
    va_end(args);
}

b32 ui_do_button(const char* fmt, ...)
{
    UI* ui = &s_app->ui;
    
    va_list args;
    va_start(args, fmt);
    UI_Item* item = ui_make_item(UI_Item_Type_Button, fmt, args);
    va_end(args);
    
    b32 click = false;
    
    if (item->hash == ui->next_hash && ui->input.select)
    {
        click = true;
    }
    
    return click;
}

b32 ui_do_slider(f32* value, f32 min, f32 max, f32 rate)
{
    UI* ui = &s_app->ui;
    CF_V2 direction = ui->input.direction;
    
    UI_Item* item = ui_make_item_fmt(UI_Item_Type_Slider, "%.2f", *value);
    
    b32 changed = false;
    
    if (item->hash == ui->next_hash)
    {
        f32 epsilon = 1e-7f;
        f32 next_value = cf_clamp(*value + direction.x * rate, min, max);
        changed = cf_abs(next_value - *value) > epsilon;
        *value = next_value;
    }
    
    return changed;
}

b32 ui_do_slider_button_text(s32 *index, const char** names, s32 count)
{
    UI* ui = &s_app->ui;
    CF_V2 direction = ui->input.direction;
    
    UI_Item* item = ui_make_item_fmt(UI_Item_Type_Slider, names[*index]);
    
    b32 click = false;
    
    if (item->hash == ui->next_hash)
    {
        s32 delta = 0;
        if (direction.x > 0)
        {
            ++delta;
        }
        else if (direction.x < 0)
        {
            --delta;
        }
        
        s32 next_index = *index + delta;
        if (next_index < 0)
        {
            next_index += count;
        }
        *index = next_index % count;
        
        if (ui->input.select)
        {
            click = true;
        }
    }
    
    return click;
}

void ui_do_background(CF_Color color)
{
    UI* ui = &s_app->ui;
    UI_Item new_item = { 0 };
    cf_array_push(ui->items, new_item);
    
    UI_Item* item = &cf_array_last(ui->items);
    item->type = UI_Item_Type_Background;
    item->background_color = color;
}

void ui_draw_deadzone(UI_Item* item)
{
    UI_Deadzone* deadzone_data = (UI_Deadzone*)item->udata;
    
    CF_V2 half_extents = cf_half_extents(item->aabb);
    
    CF_V2 knob_half_extents = cf_mul_v2_f(half_extents, 0.05f);
    CF_V2 knob_position = cf_center(item->aabb);
    knob_position.x += deadzone_data->value.x * half_extents.x;
    knob_position.y += deadzone_data->value.y * half_extents.y;
    
    CF_Aabb knob_aabb = cf_make_aabb_center_half_extents(knob_position, knob_half_extents);
    
    CF_V2 deadzone_min = cf_v2(deadzone_data->deadzone_x.x, deadzone_data->deadzone_y.x);
    CF_V2 deadzone_max = cf_v2(deadzone_data->deadzone_x.y, deadzone_data->deadzone_y.y);
    deadzone_min = cf_mul(deadzone_min, half_extents);
    deadzone_max = cf_mul(deadzone_max, half_extents);
    
    CF_Aabb deadzone_aabb = cf_make_aabb(deadzone_min, deadzone_max);
    
    // background
    {
        cf_draw_push_color(item->background_color);
        cf_draw_box_fill(item->aabb, 0.0f);
        cf_draw_pop_color();
    }
    
    // borders
    {
        cf_draw_push_color(item->border_color);
        
        cf_draw_line(cf_top(item->aabb), cf_bottom(item->aabb), 1.0f);
        cf_draw_line(cf_left(item->aabb), cf_right(item->aabb), 1.0f);
        
        cf_draw_box(item->aabb, 1.0f, 0.0f);
        
        cf_draw_pop_color();
    }
    
    // deadzone borders
    {
        cf_draw_push_color(item->border_color);
        cf_draw_box(deadzone_aabb, 1.0f, 0.0f);
        cf_draw_pop_color();
    }
    
    // knob
    {
        cf_draw_push_color(cf_color_red());
        cf_draw_box_fill(knob_aabb, 0.0f);
        cf_draw_pop_color();
    }
}

void ui_do_deadzone(CF_V2 deadzone_x, CF_V2 deadzone_y, CF_V2 value)
{
    UI* ui = &s_app->ui;
    UI_Item new_item = { 0 };
    cf_array_push(ui->items, new_item);
    
    f32 wh = 200.0f;
    CF_Aabb aabb = cf_make_aabb_center_half_extents(ui->cursor, cf_v2(wh, wh));
    
    UI_Item* item = &cf_array_last(ui->items);
    item->type = UI_Item_Type_Custom;
    item->aabb = aabb;
    item->text_color = cf_color_white();
    item->background_color = cf_color_grey();;
    item->border_color = cf_color_white();
    
    ui->cursor.y -= wh + 10.0f;
    
    UI_Deadzone* deadzone_data = (UI_Deadzone*)scratch_alloc(sizeof(UI_Deadzone));
    deadzone_data->deadzone_x = deadzone_x;
    deadzone_data->deadzone_y = deadzone_y;
    deadzone_data->value = value;
    
    item->custom_fn = ui_draw_deadzone;
    item->udata = deadzone_data;
}

void ui_do_game()
{
    UI* ui = &s_app->ui;
    World* world = &s_app->world;
    Tunnel* tunnel = &world->tunnel;
    
    ui_set_cursor(cf_v2(s_app->screen_size.x * 0.5f, s_app->screen_size.y * -0.5f));
    ui_set_layout(UI_Layout_Bottom | UI_Layout_Right);
    
    // due to the way the cursor advances when using `UI_Layout_Bottom` need to do this in reverse order
    // it's not ideal, should use a layout struct of all the items to reorder them properly but for a small
    // small ui it's fine..
    ui_do_text("%.0f%16.0f", tunnel->distance_traveled, tunnel->world_speed);
    ui_do_text("%d", world->score);
    ui_do_text("%d", world->player_lives);
    
    if (world->player_count <= 0 && world->player_lives <= 0)
    {
        world_set_state(World_State_Game_Over);
    }
    
    if (ui->input.menu)
    {
        world_set_state(World_State_Pause);
    }
}

void ui_do_pause()
{
    UI* ui = &s_app->ui;
    ui_set_cursor(cf_v2(0, 0));
    ui_set_layout(UI_Layout_None);
    
    cf_push_font_size(32.0f);
    if (ui_do_button("Continue"))
    {
        world_set_state(World_State_Play);
    }
    if (ui_do_button("Options"))
    {
        ui->options_state = UI_Options_State_Options;
    }
    if (ui_do_button("Main Menu"))
    {
        world_set_state(World_State_None);
    }
    cf_pop_font_size();
    
    if (ui->input.menu)
    {
        world_set_state(World_State_Play);
    }
}

void ui_do_main_menu()
{
    ui_set_cursor(cf_v2(0, 0));
    ui_set_layout(UI_Layout_None);
    
    const char* modes[] = 
    {
        "Easy",
        "Normal",
        "Hard",
    };
    
    static Stage_Difficulty mode_index = 0;
    
    cf_push_font_size(32.0f);
    if (ui_do_slider_button_text(&mode_index, modes, CF_ARRAY_SIZE(modes)))
    {
        world_load(mode_index, 0);
    }
    
    if (ui_do_button("Options"))
    {
        s_app->ui.options_state = UI_Options_State_Options;
    }
    if (ui_do_button("Quit"))
    {
        cf_app_signal_shutdown();
    }
    cf_pop_font_size();
}

void ui_do_game_over()
{
    World* world = &s_app->world;
    
    ui_set_cursor(cf_v2(0.0f, s_app->screen_size.y * 0.25f));
    ui_set_layout(UI_Layout_Top);
    
    ui_do_text("DISTANCE: %.0f", world->distance_traveled);
    ui_do_text("SCORE: %.0f", world->score + world->distance_traveled);
    cf_pop_font_size();
    
    ui_set_cursor(cf_v2(0, 0));
    ui_set_layout(UI_Layout_None);
    
    cf_push_font_size(32.0f);
    if (ui_do_button("Main Menu"))
    {
        world_set_state(World_State_None);
    }
    if (ui_do_button("Quit"))
    {
        cf_app_signal_shutdown();
    }
    cf_pop_font_size();
}

void ui_do_options()
{
    static s32 volume_index = 0;
    
    UI* ui = &s_app->ui;
    Audio* audio = &s_app->audio;
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * 0.5f));
    ui_set_layout(UI_Layout_Top);
    
    cf_push_font_size(32.0f);
    ui_do_text("Audio");
    ui_do_slider(&audio->volume, 0.0f, 1.0f, 0.25f);
    if (ui_do_button("Keyboard Bindings"))
    {
        ui->options_state = UI_Options_State_Keyboard;
    }
    if (ui_do_button("Controller Bindings"))
    {
        ui->options_state = UI_Options_State_Controller;
    }
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * -0.5f));
    ui_set_layout(UI_Layout_Bottom);
    
    if (ui_do_button("Back") || ui->input.menu)
    {
        ui->options_state = UI_Options_State_None;
    }
    
    cf_pop_font_size();
}

typedef struct Binding_Data
{
    const char* name;
    union
    {
        CF_KeyButton* key;
        CF_JoypadButton* button;
    };
    u8 is_joypad_button;
} Binding_Data;

void ui_setup_binding_coroutine(Binding_Data* binding);
void ui_binding_co(CF_Coroutine co);
void ui_deadzone_calibration_co(CF_Coroutine co);

void ui_setup_binding_coroutine(Binding_Data* binding)
{
    UI* ui = &s_app->ui;
    if (ui->binding_co.id)
    {
        if (cf_coroutine_state(ui->binding_co) == CF_COROUTINE_STATE_DEAD)
        {
            cf_destroy_coroutine(ui->binding_co);
            ui->binding_co = cf_make_coroutine(ui_binding_co, 0, binding);
        }
    }
    else
    {
        ui->binding_co = cf_make_coroutine(ui_binding_co, 0, binding);
    }
}

void ui_binding_co(CF_Coroutine co)
{
    Binding_Data binding = *(Binding_Data*)cf_coroutine_get_udata(co);
    b32 is_rebinding = true;
    
    cf_coroutine_yield(co);
    
    while (is_rebinding)
    {
        ui_set_cursor(cf_v2(0, 0));
        ui_set_layout(UI_Layout_None);
        
        ui_do_background(cf_color_black());
        
        cf_push_font_size(32.0f);
        ui_do_text("Rebinding %s %s", binding.is_joypad_button ? "Controller" : "Keyboard", binding.name);
        ui_do_text("%s", binding.is_joypad_button ? 
                   cf_joypad_button_to_string(*binding.button) : 
                   cf_key_button_to_string(*binding.key));
        cf_pop_font_size();
        
        if (binding.is_joypad_button)
        {
            if (cf_joypad_count() > 0)
            {
                s32 player_index = 0;
                for (s32 index = 0; index < CF_JOYPAD_BUTTON_COUNT; ++index)
                {
                    CF_JoypadButton button = (CF_JoypadButton)index;
                    if (button != CF_JOYPAD_BUTTON_BACK && button != CF_JOYPAD_BUTTON_START)
                    {
                        if (cf_joypad_button_just_pressed(player_index, button))
                        {
                            *binding.button = button;
                            is_rebinding = false;
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            for (s32 index = 0; index < CF_KEY_COUNT; ++index)
            {
                CF_KeyButton key = (CF_KeyButton)index;
                if (key != CF_KEY_ESCAPE && key != CF_KEY_ANY)
                {
                    if (cf_key_just_pressed(key))
                    {
                        *binding.key = key;
                        is_rebinding = false;
                        break;
                    }
                }
            }
        }
        
        cf_coroutine_yield(co);
    }
    
    if (binding.is_joypad_button)
    {
        if (cf_joypad_count() > 0)
        {
            s32 player_index = 0;
            while (cf_joypad_button_down(player_index, *binding.button))
            {
                cf_coroutine_yield(co);
            }
        }
    }
    else
    {
        while (cf_key_down(*binding.key))
        {
            cf_coroutine_yield(co);
        }
    }
}

void ui_deadzone_calibration_co(CF_Coroutine co)
{
    UI* ui = &s_app->ui;
    CF_V2 deadzone_x = s_app->input.deadzone_x;
    CF_V2 deadzone_y = s_app->input.deadzone_y;
    b32 cancel = false;
    
    enum
    {
        Calibration_State_Up,
        Calibration_State_Up_Release,
        Calibration_State_Down,
        Calibration_State_Down_Release,
        Calibration_State_Left,
        Calibration_State_Left_Release,
        Calibration_State_Right,
        Calibration_State_Right_Release,
    };
    
    const char* state_names[] = 
    {
        "Move Left Stick Up",
        "Release Left Stick",
        "Move Left Stick Down",
        "Release Left Stick",
        "Move Left Stick Left",
        "Release Left Stick",
        "Move Left Stick Right",
        "Release Left Stick",
    };
    
    s32 calibration_state = Calibration_State_Up;
    f32 epsilon = 0.1f;
    
    cf_coroutine_yield(co);
    
    while (cf_joypad_count() > 0)
    {
        ui_do_background(cf_color_black());
        
        ui_set_cursor(cf_v2(0.0f, 0.0f));
        ui_set_layout(UI_Layout_None);
        
        CF_V2 direction = get_left_stick(0);
        ui_do_deadzone(deadzone_x, deadzone_y, direction);
        
        if (calibration_state & 1)
        {
            if (cf_len_sq(direction) < epsilon)
            {
                switch (calibration_state)
                {
                    case Calibration_State_Up_Release: deadzone_y.y = cf_max(direction.y, 0.0f); break;
                    case Calibration_State_Down_Release: deadzone_y.x = cf_min(direction.y, 0.0f); break;
                    case Calibration_State_Left_Release: deadzone_x.x = cf_min(direction.x, 0.0f); break;
                    case Calibration_State_Right_Release: deadzone_x.y = cf_max(direction.x, 0.0f); break;
                }
                
                calibration_state = (calibration_state + 1) % CF_ARRAY_SIZE(state_names);
            }
        }
        else
        {
            f32 axis_abs_value = 0.0f;
            
            switch (calibration_state)
            {
                case Calibration_State_Up: 
                {
                    if (1.0f - direction.y < epsilon)
                    {
                        axis_abs_value = cf_abs(direction.y);
                    }
                    break;
                }
                case Calibration_State_Down: 
                {
                    if (direction.y + 1.0f < epsilon)
                    {
                        axis_abs_value = cf_abs(direction.y);
                    }
                    break;
                }
                case Calibration_State_Right: 
                {
                    if (1.0f - direction.x < epsilon)
                    {
                        axis_abs_value = cf_abs(direction.x);
                    }
                    break;
                }
                case Calibration_State_Left: 
                {
                    if (direction.x + 1.0f < epsilon)
                    {
                        axis_abs_value = cf_abs(direction.x);
                    }
                    break;
                }
            }
            
            if (1.0f - axis_abs_value < epsilon)
            {
                calibration_state = (calibration_state + 1) % CF_ARRAY_SIZE(state_names);
            }
        }
        
        ui_do_text("Press %s to cancel", cf_joypad_button_to_string(CF_JOYPAD_BUTTON_B));
        ui_do_text("Press %s to accept", cf_joypad_button_to_string(CF_JOYPAD_BUTTON_A));
        ui_do_text(state_names[calibration_state]);
        
        if (cf_joypad_button_just_pressed(0, CF_JOYPAD_BUTTON_A))
        {
            break;
        }
        
        if (cf_joypad_button_just_pressed(0, CF_JOYPAD_BUTTON_B))
        {
            cancel = true;
            break;
        }
        
        cf_coroutine_yield(co);
    }
    
    if (!cancel)
    {
        s_app->input.deadzone_x = deadzone_x;
        s_app->input.deadzone_y = deadzone_y;
    }
    
    cf_coroutine_yield(co);
}

void ui_do_options_keyboard()
{
    UI* ui = &s_app->ui;
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * 0.5f));
    ui_set_layout(UI_Layout_Top);
    
    cf_push_font_size(32.0f);
    if (ui_do_button("Fire: %s", cf_key_button_to_string(s_app->input.fire_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Fire";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.fire_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Bomb: %s", cf_key_button_to_string(s_app->input.secondary_fire_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Bomb";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.secondary_fire_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Up: %s", cf_key_button_to_string(s_app->input.up_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Up";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.up_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Down: %s", cf_key_button_to_string(s_app->input.down_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Down";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.down_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Left: %s", cf_key_button_to_string(s_app->input.left_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Left";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.left_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Right: %s", cf_key_button_to_string(s_app->input.right_key)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Right";
        binding->is_joypad_button = false;
        binding->key = &s_app->input.right_key;
        
        ui_setup_binding_coroutine(binding);
    }
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * -0.5f));
    ui_set_layout(UI_Layout_Bottom);
    
    if (ui_do_button("Back") || ui->input.menu)
    {
        ui->options_state = UI_Options_State_Options;
    }
    
    cf_pop_font_size();
    
    if (ui->binding_co.id)
    {
        if (cf_coroutine_state(ui->binding_co) != CF_COROUTINE_STATE_DEAD)
        {
            cf_coroutine_resume(ui->binding_co);
        }
    }
}

void ui_do_options_controller()
{
    UI* ui = &s_app->ui;
    Input* input = &s_app->input;
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * 0.5f));
    ui_set_layout(UI_Layout_Top);
    
    cf_push_font_size(32.0f);
    if (ui_do_button("Fire: %s", cf_joypad_button_to_string(s_app->input.fire_button)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Fire";
        binding->is_joypad_button = true;
        binding->button = &s_app->input.fire_button;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Bomb: %s", cf_joypad_button_to_string(s_app->input.secondary_fire_button)))
    {
        Binding_Data* binding = (Binding_Data*)scratch_alloc(sizeof(Binding_Data));
        binding->name = "Bomb";
        binding->is_joypad_button = true;
        binding->button = &s_app->input.secondary_fire_button;
        
        ui_setup_binding_coroutine(binding);
    }
    
    if (ui_do_button("Deadzone Calibrate"))
    {
        if (ui->deadzone_calibration_co.id)
        {
            if (cf_coroutine_state(ui->deadzone_calibration_co) == CF_COROUTINE_STATE_DEAD)
            {
                ui->deadzone_calibration_co = cf_make_coroutine(ui_deadzone_calibration_co, 0, NULL);
            }
        }
        else
        {
            ui->deadzone_calibration_co = cf_make_coroutine(ui_deadzone_calibration_co, 0, NULL);
        }
    }
    
    ui_do_text("Deadzone X");
    ui_do_slider(&input->deadzone_x.x, -1.0f, 0.0f, 0.01f);
    ui_do_slider(&input->deadzone_x.y, 0.0f, 1.0f, 0.01f);
    
    ui_do_text("Deadzone Y");
    ui_do_slider(&input->deadzone_y.x, -1.0f, 0.0f, 0.01f);
    ui_do_slider(&input->deadzone_y.y, 0.0f, 1.0f, 0.01f);
    
    ui_set_cursor(cf_v2(0, s_app->screen_size.y * -0.5f));
    ui_set_layout(UI_Layout_Bottom);
    
    if (ui_do_button("Back") || ui->input.menu)
    {
        ui->options_state = UI_Options_State_Options;
    }
    
    cf_pop_font_size();
    
    if (ui->binding_co.id)
    {
        if (cf_coroutine_state(ui->binding_co) != CF_COROUTINE_STATE_DEAD)
        {
            cf_coroutine_resume(ui->binding_co);
        }
    }
    else if (ui->deadzone_calibration_co.id)
    {
        if (cf_coroutine_state(ui->deadzone_calibration_co) != CF_COROUTINE_STATE_DEAD)
        {
            cf_coroutine_resume(ui->deadzone_calibration_co);
        }
    }
}

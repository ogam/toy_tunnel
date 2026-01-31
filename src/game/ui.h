// UI was made in a very short period of time and very bug prone
// not ideal to use in other projects

#ifndef UI_H
#define UI_H

typedef b8 UI_Item_Type;
enum
{
    UI_Item_Type_Text,
    UI_Item_Type_Button,
    UI_Item_Type_Slider,
    UI_Item_Type_Background,
    UI_Item_Type_Custom,
};

typedef struct UI_Item
{
    CF_Aabb aabb;
    CF_Aabb text_aabb;
    const char* text;
    u64 hash;
    
    CF_V2 origin;
    CF_Color text_color;
    CF_Color background_color;
    CF_Color border_color;
    
    f32 font_size;
    
    UI_Item_Type type;
    
    void (*custom_fn)(struct UI_Item* item);
    void* udata;
} UI_Item;

typedef b32 UI_Layout;
enum
{
    UI_Layout_None = 0,
    UI_Layout_Bottom = 1 << 0,
    UI_Layout_Top = 1 << 1,
    UI_Layout_Left = 1 << 2,
    UI_Layout_Right = 1 << 3,
};

typedef struct UI_Input
{
    CF_V2 direction;
    b8 select;
    b8 menu;
} UI_Input;

typedef u8 UI_Options_State;
enum
{
    UI_Options_State_None,
    UI_Options_State_Options,
    UI_Options_State_Keyboard,
    UI_Options_State_Controller,
};

typedef struct UI
{
    UI_Input input;
    dyna UI_Item* items;
    dyna UI_Item* interactable_items;
    
    UI_Layout layout;
    CF_V2 cursor;
    s32 interactable_item_counter;
    f32 scale;
    
    u64 next_hash;
    
    CF_Coroutine binding_co;
    CF_Coroutine deadzone_calibration_co;
    
    UI_Options_State options_state;
} UI;

void init_ui();
void update_ui_input();
void update_ui();
void draw_ui();

void ui_set_layout(UI_Layout layout);
void ui_set_cursor(CF_V2 position);

void ui_do_text(const char* fmt, ...);
b32 ui_do_button(const char* fmt, ...);
b32 ui_do_slider(f32* value, f32 min, f32 max, f32 rate);
b32 ui_do_slider_button_text(s32 *index, const char** names, s32 count);
void ui_do_background(CF_Color color);
void ui_do_deadzone(CF_V2 deadzone_x, CF_V2 deadzone_y, CF_V2 value);

void ui_do_game();
void ui_do_pause();
void ui_do_main_menu();
void ui_do_game_over();
void ui_do_options();
void ui_do_options_keyboard();
void ui_do_options_controller();

#endif //UI_H

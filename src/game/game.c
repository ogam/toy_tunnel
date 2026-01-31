#include "common/types.h"
#include "common/math.h"
#include "common/macros.h"
#include "common/memory.h"
#include "assets/assets.h"
#include "game/audio.h"
#include "game/game.h"
#include "game/world.h"
#include "game/ui.h"

#ifndef CF_RUNTIME_SHADER_COMPILATION
#include "game/tunnel_color_shd.h"
#endif

typedef struct Input
{
    CF_KeyButton up_key;
    CF_KeyButton down_key;
    CF_KeyButton left_key;
    CF_KeyButton right_key;
    CF_KeyButton fire_key;
    CF_KeyButton secondary_fire_key;
    
    CF_JoypadButton fire_button;
    CF_JoypadButton secondary_fire_button;
    
    CF_V2 deadzone_x;
    CF_V2 deadzone_y;
    
    CF_V2 direction;
    b8 fire;
    b8 secondary_fire;
} Input;

typedef b32 Debug_Mode;
enum
{
    Debug_Mode_Depth_Ring,
};

typedef struct App
{
    Input input;
    Memory memory;
    Audio audio;
    World world;
    UI ui;
    
    CF_Shader tunnel_shader;
    CF_Canvas scene_canvas;
    CF_V2 screen_size;
    CF_V2 screen_scale;
    
    b32 debug_mode;
    s32 debug_ring;
} App;

App* s_app;

void init()
{ 
    s_app = cf_alloc(sizeof(App));
    CF_MEMSET(s_app, 0, sizeof(App));
    
    init_memory();
    init_audio();
    
    init_world();
    init_ui();
    
#ifdef CF_RUNTIME_SHADER_COMPILATION
    mount_shaders_read_directory();
    cf_shader_directory("/");
    s_app->tunnel_shader = cf_make_draw_shader("tunnel_color.shd");
#else
    s_app->tunnel_shader = cf_make_draw_shader_from_bytecode(s_tunnel_color_shd_bytecode);
#endif
    mount_data_read_directory();
    
    s_app->debug_ring = -1;
    
    // inputs
    {
        Input* input = &s_app->input;
        input->up_key = CF_KEY_UP;
        input->down_key = CF_KEY_DOWN;
        input->left_key = CF_KEY_LEFT;
        input->right_key = CF_KEY_RIGHT;
        input->fire_key = CF_KEY_Z;
        input->secondary_fire_key = CF_KEY_X;
        
        input->fire_button = CF_JOYPAD_BUTTON_X;
        input->secondary_fire_button = CF_JOYPAD_BUTTON_A;
        
        input->deadzone_x = cf_v2(-0.1f, 0.1f);
        input->deadzone_y = cf_v2(-0.1f, 0.1f);
    }
    
    load_configs();
}

void handle_window_events()
{
    s32 w, h;
    cf_app_get_size(&w, &h);
    CF_V2 screen_size = cf_v2((f32)w, (f32)h);
    
    if (cf_len_sq(cf_sub(s_app->screen_size, screen_size)))
    {
        cf_app_set_canvas_size(w, h);
        
        CF_CanvasParams canvas_params = cf_canvas_defaults(w, h);
        if (s_app->scene_canvas.id)
        {
            cf_destroy_canvas(s_app->scene_canvas);
        }
        s_app->scene_canvas = cf_make_canvas(canvas_params);
    }
    
    s_app->screen_size = screen_size;
    s_app->screen_scale = cf_v2(screen_size.x / GAME_WIDTH, screen_size.y / GAME_HEIGHT);
    
    if (cf_app_has_focus())
    {
        cf_mouse_hide(true);
    }
    else
    {
        cf_mouse_hide(false);
    }
}

CF_V2 get_left_stick(s32 player_index)
{
    CF_V2 direction = cf_v2(0, 0);
    direction.x = (f32)cf_joypad_axis(player_index, CF_JOYPAD_AXIS_LEFTX) / (1 << 15);
    direction.y = -(f32)cf_joypad_axis(player_index, CF_JOYPAD_AXIS_LEFTY) / (1 << 15);
    
    return direction;
}

void update_input()
{
    Input* input = &s_app->input;
    
    CF_V2 direction = cf_v2(0, 0);
    b8 fire = false;
    b8 secondary_fire = false;
    
    // controller
    if (cf_joypad_count() > 0)
    {
        s32 player_index = 0;
        direction = get_left_stick(player_index);
        
        if (direction.x > input->deadzone_x.x && direction.x < input->deadzone_x.y)
        {
            direction.x = 0.0f;
        }
        else
        {
            if (direction.x < 0)
            {
                direction.x = cf_remap(direction.x, -1.0f, input->deadzone_x.x, -1.0f, 0.0f);
            }
            else
            {
                direction.x = cf_remap(direction.x, input->deadzone_x.y, 1.0f, 0.0f, 1.0f);
            }
        }
        if (direction.y > input->deadzone_y.x && direction.y < input->deadzone_y.y)
        {
            direction.y = 0.0f;
        }
        else
        {
            if (direction.y < 0)
            {
                direction.y = cf_remap(direction.y, -1.0f, input->deadzone_y.x, -1.0f, 0.0f);
            }
            else
            {
                direction.y = cf_remap(direction.y, input->deadzone_y.y, 1.0f, 0.0f, 1.0f);
            }
        }
        
        if (cf_joypad_button_down(player_index, CF_JOYPAD_BUTTON_DPAD_UP))
        {
            direction.y += 1.0f;
        }
        if (cf_joypad_button_down(player_index, CF_JOYPAD_BUTTON_DPAD_DOWN))
        {
            direction.y -= 1.0f;
        }
        if (cf_joypad_button_down(player_index, CF_JOYPAD_BUTTON_DPAD_RIGHT))
        {
            direction.x += 1.0f;
        }
        if (cf_joypad_button_down(player_index, CF_JOYPAD_BUTTON_DPAD_LEFT))
        {
            direction.x -= 1.0f;
        }
        
        fire = cf_joypad_button_down(player_index, input->fire_button);
        secondary_fire = cf_joypad_button_just_pressed(player_index, input->secondary_fire_button);
    }
    
    // keyboard
    {
        if (cf_key_down(input->up_key))
        {
            direction.y += 1.0f;
        }
        if (cf_key_down(input->down_key))
        {
            direction.y -= 1.0f;
        }
        if (cf_key_down(input->right_key))
        {
            direction.x += 1.0f;
        }
        if (cf_key_down(input->left_key))
        {
            direction.x -= 1.0f;
        }
        
        fire = fire || cf_key_down(input->fire_key);
        secondary_fire = secondary_fire || cf_key_just_pressed(input->secondary_fire_key);
    }
    
    if (cf_key_just_pressed(CF_KEY_F1))
    {
        BIT_TOGGLE(s_app->debug_mode, Debug_Mode_Depth_Ring);
    }
    if (cf_key_just_pressed(CF_KEY_LEFTBRACKET))
    {
        s_app->debug_ring = cf_max(s_app->debug_ring - 1, -1);
    }
    if (cf_key_just_pressed(CF_KEY_RIGHTBRACKET))
    {
        s_app->debug_ring = cf_min(s_app->debug_ring + 1, (s32)TUNNEL_MAX_DEPTH);
    }
    
    direction.x = cf_clamp(direction.x, -1.0f, 1.0f);
    direction.y = cf_clamp(direction.y, -1.0f, 1.0f);
    
    input->direction = direction;
    input->fire = fire;
    input->secondary_fire = secondary_fire;
}

void update(void* udata)
{
    scratch_reset();
    handle_window_events();
    
    //ImGui_PushFontFloat(NULL, 64.0f);
    
    update_input();
    update_world();
    update_ui();
    update_audio();
    
    draw_world();
    draw_ui();
    
    //ImGui_PopFont();
}

CF_KeyButton string_to_key(const char* name)
{
    CF_KeyButton key = CF_KEY_COUNT;
    
    if (name)
    {
        for (s32 index = 0; index < CF_KEY_COUNT; ++index)
        {
            CF_KeyButton current_key = (CF_KeyButton)index;
            if (cf_string_equ(name, cf_key_button_to_string(current_key)))
            {
                key = current_key;
                break;
            }
        }
    }
    
    return key;
}

CF_JoypadButton string_to_button(const char* name)
{
    CF_JoypadButton button = CF_JOYPAD_BUTTON_COUNT;
    
    if (name)
    {
        for (s32 index = 0; index < CF_JOYPAD_BUTTON_COUNT; ++index)
        {
            CF_JoypadButton current_button = (CF_JoypadButton)index;
            if (cf_string_equ(name, cf_joypad_button_to_string(current_button)))
            {
                button = current_button;
                break;
            }
        }
    }
    
    return button;
}


void load_configs()
{
    mount_root_read_directory();
    if (cf_fs_file_exists("settings.txt"))
    {
        size_t file_size = 0;
        char* file = (char*)cf_fs_read_entire_file_to_memory("settings.txt", &file_size);
        char* walker = file;
        char* line_end = NULL;
        char buffer[1024];
        
        Input* input = &s_app->input;
        
        line_end = cf_string_find(walker, "\n");
        CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
        buffer[line_end - walker] = '\0';
        s_app->audio.volume = cf_string_tofloat(buffer);
        walker = line_end + 1;
        
        // keyboard
        {
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->up_key = string_to_key(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->down_key = string_to_key(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->left_key = string_to_key(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->right_key = string_to_key(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->fire_key = string_to_key(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->secondary_fire_key = string_to_key(buffer);
        }
        
        // controller
        {
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->fire_button = string_to_button(buffer);
            
            line_end = cf_string_find(walker, "\n");
            CF_MEMCPY(buffer, walker, (s32)(line_end - walker));
            buffer[line_end - walker] = '\0';
            walker = line_end + 1;
            input->secondary_fire_button = string_to_button(buffer);
        }
        
        cf_free(file);
    }
    dismount_root_directory();
}

void save_configs()
{
    CF_File *file = NULL;
    mount_root_read_directory();
    mount_root_write_directory();
    if (cf_fs_file_exists("settings.txt"))
    {
        file = cf_fs_open_file_for_write("settings.txt");;
    }
    else
    {
        file = cf_fs_create_file("settings.txt");;
    }
    
    if (file)
    {
        char buffer[1024];
        s32 length;
        
        Input* input = &s_app->input;
        
        {
            length = snprintf(buffer, sizeof(buffer), "%.2f\n", s_app->audio.volume);
            cf_fs_write(file, buffer, length);
        }
        // keyboard
        {
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->up_key));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->down_key));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->left_key));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->right_key));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->fire_key));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_key_button_to_string(input->secondary_fire_key));
            cf_fs_write(file, buffer, length);
        }
        
        // controller
        {
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_joypad_button_to_string(input->fire_button));
            cf_fs_write(file, buffer, length);
            
            length = snprintf(buffer, sizeof(buffer), "%s\n", cf_joypad_button_to_string(input->secondary_fire_button));
            cf_fs_write(file, buffer, length);
        }
        
        cf_fs_close(file);
    }
}

#include "assets/assets.c"
#include "common/math.c"
#include "common/memory.c"
#include "game/audio.c"
#include "game/world.c"
#include "game/ui.c"

#include <cute.h>

#include "game/game.c"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    int display_index = 0;
    int options = CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT | CF_APP_OPTIONS_RESIZABLE_BIT;
    int width = GAME_WIDTH;
    int height = GAME_HEIGHT;
    
	CF_Result result = cf_make_app("toy tunnel", display_index, 0, 0, width, height, options, argv[0]);
    
    if (cf_is_error(result)) return -1;
    
    cf_set_target_framerate(TARGET_FRAMERATE);
    cf_app_set_vsync(true);
    
    init();
    
    while (cf_app_is_running())
    {
        cf_app_update(update);
        cf_app_draw_onto_screen(false);
    }
    
    cf_destroy_app();
    
    return 0;
}
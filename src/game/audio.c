#include "game/audio.h"

void init_audio()
{
    Audio* audio = &s_app->audio;
    
    cf_array_fit(audio->sounds, 512);
    audio->volume = 1.0f;
    
    audio->rnd = cf_rnd_seed(cf_get_tick_frequency());
    audio_load_tracks();
}

void update_audio()
{
    Audio* audio = &s_app->audio;
    CF_Rnd* rnd = &audio->rnd;
    f32 sfx_volume = audio->volume;
    
    for (s32 index = cf_array_count(audio->sounds) - 1; index >= 0; --index)
    {
        Sound* sound = audio->sounds + index;
        if (!cf_sound_get_is_paused(sound->sound) && !cf_sound_is_active(sound->sound))
        {
            cf_array_del(audio->sounds, index);
        }
    }
    
    for (s32 index = cf_array_count(audio->sounds) - 1; index >= 0; --index)
    {
        Sound* sound = audio->sounds + index;
        cf_sound_set_volume(sound->sound, sfx_volume);
    }
    
    {
        const char** names = (const char**)cf_hashtable_keys(audio->queue);
        Sound_Queue* queues = (Sound_Queue*)cf_hashtable_items(audio->queue);
        
        for (s32 index = 0; index < cf_hashtable_count(audio->queue); ++index)
        {
            // emscripten has a weird bug where this can be null and will cause a lookup failure
            if (names[index] == NULL)
            {
                continue;
            }
            
            CF_Audio track = cf_hashtable_get(audio->tracks, names[index]);
            
            CF_SoundParams params = cf_sound_params_defaults();
            params.pitch = cf_rnd_range_float(rnd, 0.8f, 1.0f);
            params.volume = sfx_volume;
            
            Sound sound = { 0 };
            sound.sound = cf_play_sound(track, params);
            sound.type = queues[index].type;
            
            cf_array_push(audio->sounds, sound);
        }
    }
    
    cf_hashtable_clear(audio->queue);
}

void audio_load_tracks()
{
    Audio* audio = &s_app->audio;
    
    mount_data_read_directory();
    cf_hashtable_set(audio->tracks, cf_sintern("player_die"), cf_audio_load_wav("sounds/player_die.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("enemy_hit"), cf_audio_load_wav("sounds/enemy_hit.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("enemy_die"), cf_audio_load_wav("sounds/enemy_die.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("boss_die"), cf_audio_load_wav("sounds/boss_die.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("player_bullet_fire"), cf_audio_load_wav("sounds/player_bullet_fire.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("enemy_bullet_fire"), cf_audio_load_wav("sounds/enemy_bullet_fire.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("bullet_graze"), cf_audio_load_wav("sounds/bullet_graze.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("bullet_eater_fire"), cf_audio_load_wav("sounds/bullet_eater_fire.wav"));
    cf_hashtable_set(audio->tracks, cf_sintern("bullet_eat"), cf_audio_load_wav("sounds/bullet_eat.wav"));
}

void audio_set_sfx_pause(b32 true_to_pause)
{
    Audio* audio = &s_app->audio;
    for (s32 index = cf_array_count(audio->sounds) - 1; index >= 0; --index)
    {
        Sound* sound = audio->sounds + index;
        if (sound->type == Audio_Type_SFX)
        {
            cf_sound_set_is_paused(sound->sound, true_to_pause);
        }
    }
}

void audio_play_sfx(const char* name)
{
    Audio* audio = &s_app->audio;
    name = cf_sintern(name);
    if (cf_hashtable_has(audio->tracks, name))
    {
        Sound_Queue queue = { 0 };
        if (cf_hashtable_has(audio->queue, name))
        {
            queue = cf_hashtable_get(audio->queue, name);
        }
        
        if (queue.count < 1)
        {
            ++queue.count;
            cf_hashtable_set(audio->queue, name, queue);
        }
    }
}
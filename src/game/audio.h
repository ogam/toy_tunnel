#ifndef AUDIO_H
#define AUDIO_H

typedef u8 Audio_Type;
enum
{
    Audio_Type_SFX,
    Audio_Type_Music,
};

typedef struct Sound
{
    CF_Sound sound;
    Audio_Type type;
} Sound;

typedef struct Sound_Queue
{
    s32 count;
    Audio_Type type;
} Sound_Queue;

typedef struct Audio
{
    dyna Sound* sounds;
    cf_htbl CF_Audio* tracks;
    cf_htbl Sound_Queue* queue;
    
    f32 volume;
    
    CF_Rnd rnd;
} Audio;

void init_audio();
void update_audio();
void audio_load_tracks();
void audio_set_sfx_pause(b32 true_to_pause);
void audio_play_sfx(const char* name);

#endif //AUDIO_H

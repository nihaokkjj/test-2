#include"add_music.h"

void Music::initializeAudio() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("initialfailed: %s", SDL_GetError());
		return ;
	}

}

void Music::callback(void* userdata, Uint8* stream, int len)
{
    Music* music = static_cast<Music*>(userdata);

    if (music->audio_buf == NULL || music->audio_len <= 0) {
        SDL_memset(stream, 0, len); // ��侲��
        return;
    }

    int remain = music->audio_len - music->audio_pos;
    if (remain <= 0)
    {
        music->audio_pos = 0;//���²���
        remain = music->audio_len;
    }
    if (remain > len) {
        SDL_memcpy(stream, music->audio_buf + music->audio_pos, len);
        music->audio_pos += len;
    }
    else {
        SDL_memcpy(stream, music->audio_buf + music->audio_pos, remain);
        SDL_memset(stream + remain, 0, len - remain); // ���ʣ�ಿ��Ϊ����
        music->audio_pos = 0; // ���¿�ʼ����
    }
   
}

void Music::play_wav() {

    SDL_AudioSpec audio_spec;

    //����WAV�ļ�
    if (SDL_LoadWAV("background.WAV", &audio_spec, &audio_buf, &audio_len) == NULL)
    {
        SDL_Log("SDL_LoadWAV failed: %s", SDL_GetError());
        return;
    }
    //�ص�����
    audio_spec.callback = callback;

    audio_spec.userdata = this;
    
    //����Ƶ�豸
    device_id = SDL_OpenAudioDevice(NULL,0, &audio_spec, NULL, 0);
    if (device_id == 0) {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
        SDL_FreeWAV(audio_buf);
        return;
    }

    
    //��ʼ����
    SDL_PauseAudioDevice(device_id, 0);
}


void Music::deinit() {
    if (device_id != 0) {
        SDL_PauseAudioDevice(device_id, 1); // ֹͣ��Ƶ�豸
        SDL_CloseAudioDevice(device_id);
        device_id = 0;
    }
    if (audio_buf != NULL) {
        SDL_FreeWAV(audio_buf);
        audio_buf = NULL;
    }
}
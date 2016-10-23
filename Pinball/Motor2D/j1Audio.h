#ifndef __j1Audio_H__
#define __j1Audio_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class j1Audio : public j1Module
{
public:

	j1Audio();
	~j1Audio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
};

#endif // __j1Audio_H__
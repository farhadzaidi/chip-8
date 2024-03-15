#include <math.h>
#include <SDL2/SDL.h>

#include "sound.h"

#define PI 3.14159265
#define SAMPLE_RATE 44100
#define FREQUENCY 300.0
#define SAMPLE_SIZE 1024

void audio_callback(void* userdata, Uint8* stream, int len) {
	// Keep track of the phase for sine wave generation.
    static double phase = 0.0;

    int sample_count = len / sizeof(short);
    short* samples = (short*) stream;

    for (int i = 0; i < sample_count; ++i) {
        double sample = sin(phase * 2 * PI) * 32767; 
        samples[i] = (short) sample;

        // Update phase for next sample
        phase += FREQUENCY / SAMPLE_RATE;
        if (phase >= 1.0) {
            phase -= 1.0; // Wrap phase within [0, 1)
        }
    }
}

void init_sound() {
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec desired_spec;
	desired_spec.freq = SAMPLE_RATE; 
	desired_spec.format = AUDIO_S16SYS; 
	desired_spec.channels = 1; 
	desired_spec.samples = SAMPLE_SIZE; 
	desired_spec.callback = audio_callback;
	desired_spec.userdata = NULL;

	SDL_AudioSpec obtained_spec;
	SDL_OpenAudio(&desired_spec, &obtained_spec);
}

void play_sound() {
	SDL_PauseAudio(0);
}

void pause_sound() {
	SDL_PauseAudio(1);
}

void close_sound() {
	SDL_CloseAudio();
}
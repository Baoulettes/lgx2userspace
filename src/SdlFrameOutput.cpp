#include "SdlFrameOutput.h"

#include <SDL.h>

namespace sdl {

    SdlFrameOutput::SdlFrameOutput() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

        SDL_AudioSpec want, have;

        SDL_memset(&want, 0, sizeof(want));
        want.freq = 48000;
        want.format = AUDIO_S16LSB;
        want.channels = 2;
        want.samples = 1024;
        want.callback = nullptr;
        _audio = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        SDL_PauseAudioDevice(_audio, 0);

        SDL_Window *screen = SDL_CreateWindow("lgx2userspace", 1920, 100, 1920, 1080, 0);

        _renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

        _texture = SDL_CreateTexture(
                _renderer,
                SDL_PIXELFORMAT_YUY2,
                SDL_TEXTUREACCESS_STREAMING,
                1920,
                1080);
    }

    void SdlFrameOutput::videoFrameAvailable(uint32_t *image) {
        uint16_t *pixels;
        int32_t pitch;
        SDL_LockTexture(_texture, nullptr, (void **) &pixels, &pitch);
        memcpy(pixels, image, 1920 * 1080 * 2);
        SDL_UnlockTexture(_texture);
    }

    void SdlFrameOutput::audioFrameAvailable(uint32_t *audio) {
        SDL_QueueAudio(_audio, audio, 800*4);
    }

    void SdlFrameOutput::render() {
        SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
        SDL_RenderPresent(_renderer);
    }

}
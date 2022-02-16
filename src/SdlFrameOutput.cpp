#include "GlobalVar.h"
#include "SdlFrameOutput.h"

#include <SDL.h>

namespace sdl {

    SdlFrameOutput::SdlFrameOutput() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    }

    void SdlFrameOutput::initialiseVideo() {
        _window = SDL_CreateWindow(WindowName, WindowX, WindowY, WindowW, WindowH, 0);

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

        _texture = SDL_CreateTexture(
                _renderer,
                SDL_PIXELFORMAT_YUY2,
                SDL_TEXTUREACCESS_STREAMING,
                FrameW,
                FrameH);
    }

    void SdlFrameOutput::initialiseAudio() {
        SDL_AudioSpec want, have;

        SDL_memset(&want, 0, sizeof(want));
        want.freq = AudioFrequency;
        want.format = AUDIO_S16LSB;
        want.channels = AudioChannel;
        want.samples = AudioSample;
        want.callback = nullptr;
        _audio = SDL_OpenAudioDevice(nullptr, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        SDL_PauseAudioDevice(_audio, 0);
    }

    void SdlFrameOutput::videoFrameAvailable(uint32_t *image) {
        uint16_t *pixels;
        int32_t pitch;
        SDL_LockTexture(_texture, nullptr, (void **) &pixels, &pitch);
        memcpy(pixels, image, FrameW * FrameH * 2);
        SDL_UnlockTexture(_texture);
    }

    void SdlFrameOutput::audioFrameAvailable(uint32_t *audio) {
        SDL_QueueAudio(_audio, audio, 800*4);
    }

    void SdlFrameOutput::display() {

        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);

        if (keyboardState[SDL_SCANCODE_F]) {
            SDL_SetWindowFullscreen(_window, 1);
        } else if (keyboardState[SDL_SCANCODE_G]) {
            SDL_SetWindowFullscreen(_window, 0);
        }
        
        SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
        SDL_RenderPresent(_renderer);
    }

    void SdlFrameOutput::render() {
        // Should be where audio data is written to the audio pipe
    }

    void SdlFrameOutput::shutdownVideo() {
        SDL_DestroyTexture(_texture);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void SdlFrameOutput::shutdownAudio() {
        SDL_PauseAudioDevice(_audio, 1);
        SDL_CloseAudioDevice(_audio);
    }

}

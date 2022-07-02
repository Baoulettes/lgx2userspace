#include <iostream>
#include "../Lib/SDL/SDL2/SDL.h"
#include <signal.h>
#include "GlobalVar.h"
#include "UsbStream.h"
//#include "SdlFrameOutput.h"
//#include "ImGuiImpl.h"
#include "NOOPLogger.h"
#include "OptionParser.h"
#include "version.h"
#include "lgxdevice.h"
#include "../Lib/ImGui/imgui.h"
#include "../Lib/ImGui/backends/imgui_impl_sdl.h"
#include "../Lib/ImGui/backends/imgui_impl_opengl3.h"
#include "../Lib/SDL/SDL2/SDL.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include "../Lib/SDL/SDL2/SDL_opengles2.h"
#else
#include "../Lib/SDL/SDL2/SDL_opengl.h"
#endif
bool do_exit = false;
//SDL Init variables 
SDL_Window *_window;
SDL_Renderer *_renderer;
SDL_Texture *_texture;
SDL_AudioDeviceID _audio;
SDL_GLContext _gl_context;
int L_FrameW = FrameW;
int L_FrameH = FrameH;
#define TOGGLE_BTN ImGuiKey_F1
bool ShowToolBar = false;
const float toolbarSize = 25;
namespace sdl {
    class SdlFrameOutput : public lgx2::VideoOutput, public lgx2::AudioOutput {
        public:
        SdlFrameOutput::SdlFrameOutput() {
            SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        }
        void SdlFrameOutput::initialiseVideo() {
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
    };
}
namespace ImGui {
    void DockSpaceUI() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        SetNextWindowPos(viewport->Pos);
        SetNextWindowSize(viewport->Size);
        SetNextWindowViewport(viewport->ID);
        ImGuiWindowClass window_class1;
        window_class1.DockNodeFlagsOverrideSet = ImGuiViewportFlags_TopMost;
        ImGui::SetNextWindowClass(&window_class1);
        ImGuiWindowFlags window_flags = 0 | ImGuiWindowFlags_NoBackground | ImGuiViewportFlags_TopMost | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        Begin("Master DockSpace", NULL, window_flags);
        ImGuiID dockMain = GetID("MyDockspace");

        DockSpace(dockMain);
        End();
        PopStyleVar(3);
    }
    void ToolbarUI() {
        if (IsKeyPressed(TOGGLE_BTN)) {
            ShowToolBar = !ShowToolBar;
        }
        if (BeginMainMenuBar()) {
       //if (ShowToolBar && BeginMainMenuBar()) { 
            if (ImGui::BeginMenu("Run"))  {
                if(Button("LGX - GC550 1080p")){
                    L_FrameW = 1920;
                    L_FrameH = 1080;
                }
                if(Button("LGX - GC550 720p")){
                    L_FrameW = 1280;
                    L_FrameH = 720;
                }
                if(Button("LGX - GC550 Gamecube mode")){
                    L_FrameW = 704;
                    L_FrameH = 480;
                }
                ImGui::EndMenu();
            }
            EndMainMenuBar();
        }
    }
}
int main(int argc, char **argv) {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    _window = SDL_CreateWindow(WindowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FrameW, FrameH, window_flags);
    _renderer = SDL_CreateRenderer(_window, -1, 1);
    _texture = SDL_CreateTexture(_renderer,SDL_PIXELFORMAT_YUY2,SDL_TEXTUREACCESS_STREAMING,FrameW,FrameH);
    _gl_context = SDL_GL_CreateContext(_window);
    SDL_GL_MakeCurrent(_window, _gl_context);
    SDL_GL_SetSwapInterval(0.0007); // Enable vsync

    app::OptionParser optionParser{};

    if (!optionParser.process(argc, argv)) {
        return 0;
    }

    lgx2::Logger *logger{optionParser.logger()};
    lgx2::VideoOutput *videoOutput{optionParser.videoOutput()};
    lgx2::AudioOutput *audioOutput{optionParser.audioOutput()};

    libusb::UsbStream stream{optionParser.deviceType()};
    sdl::SdlFrameOutput sdlOutput = sdl::SdlFrameOutput();
    NOOPLogger noopLogger{};

    if (videoOutput == nullptr) {
        videoOutput = &sdlOutput;
    }

    if (audioOutput == nullptr) {
        audioOutput = &sdlOutput;
    }

    if (logger == nullptr) {
        logger = &noopLogger;
    }

    lgx2::Device device{&stream, videoOutput, audioOutput, logger};

    device.initialise();

   // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(_window, _gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    SDL_Event event;

    signal(SIGKILL, [](int) {
        do_exit = true;
    });

    signal(SIGTERM, [](int) {
        do_exit = true;
    });

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!do_exit) {

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                do_exit = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window))
                do_exit = true;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceUI();
        ImGui::ToolbarUI();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(_window);
        device.run();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(_renderer);
    SDL_GL_DeleteContext(_gl_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();

    device.shutdown();

    logger->summarise();
    return 0;
}

#include <stdexcept>
#include <utility>
#include "GlobalVar.h"
#include "PulseAudioOutput.h"

pulse::PulseAudioOutput::PulseAudioOutput(std::string pulseAudioSink) : _pulseAudioSink{std::move(pulseAudioSink)}, _pulseAudioHandle{nullptr} {

}

void pulse::PulseAudioOutput::initialiseAudio() {

    static const pa_sample_spec sampleSpec = {
            .format = PA_SAMPLE_S16LE,
            .rate = AudioFrequency,
            .channels = AudioChannel
    };

    _pulseAudioHandle = pa_simple_new(nullptr, PA_Title, PA_STREAM_PLAYBACK, _pulseAudioSink.c_str(), PA_type, &sampleSpec, nullptr, nullptr, nullptr);
}

void pulse::PulseAudioOutput::audioFrameAvailable(uint32_t *audio) {
    if (pa_simple_write(_pulseAudioHandle, audio, 800*4, nullptr) < 0) {
        throw std::runtime_error(PA_Failed);
    }
}

void pulse::PulseAudioOutput::render() {

}

void pulse::PulseAudioOutput::shutdownAudio() {
    pa_simple_drain(_pulseAudioHandle, nullptr);
    pa_simple_free(_pulseAudioHandle);
}

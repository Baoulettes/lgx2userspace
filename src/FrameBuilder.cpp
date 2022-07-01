#include "FrameBuilder.h"
#include "GlobalVar.h"
#include <cstring>
namespace utils {

    FrameBuilder::FrameBuilder() : _videoFrame{new uint32_t[FrameW * FrameH]}, _audioFrame{new uint32_t[800]},
                                   _videoOffset{0}, _audioOffset{0} {

    }

    uint8_t * FrameBuilder::buildVideo(uint8_t *block, uint32_t len) {
        uint32_t blocksUntilCompleteFrame = FrameW * FrameH - _videoOffset;
        uint32_t blocksCopied = (len > blocksUntilCompleteFrame) ? blocksUntilCompleteFrame : len;

        std::memcpy(_videoFrame + _videoOffset, block, blocksCopied * 4);
        _videoOffset += blocksCopied;
        return block + blocksCopied*4;
    }

    void FrameBuilder::buildAudio(uint8_t *block, uint32_t len) {
        uint32_t blocksUntilCompleteFrame = 800 - _audioOffset;
        uint32_t blocksCopied = (len > blocksUntilCompleteFrame) ? blocksUntilCompleteFrame : len;

        std::memcpy(_audioFrame + _audioOffset, block, blocksCopied * 4);
        _audioOffset += blocksCopied;
    }

    uint32_t *FrameBuilder::completeVideoFrame() {
        if (_videoOffset == FrameW * FrameH) {
            _videoOffset = 0;
            return _videoFrame;
        } else {
            _videoOffset = 0;
        }
        return _videoFrame;
    }

    uint32_t *FrameBuilder::completeAudioFrame() {
        if (_audioOffset == 800) {
            _audioOffset = 0;
            return _audioFrame;
        } else {
            _audioOffset = 0;
        }
        return _audioFrame;
    }

    bool FrameBuilder::buildingVideoFrame() {
        return _videoOffset != 0;
    }

    uint32_t FrameBuilder::blocksUntilVideoComplete() {
        return FrameW * FrameH - _videoOffset;
    }

}
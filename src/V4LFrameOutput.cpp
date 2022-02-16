#include "GlobalVar.h"
#include "V4LFrameOutput.h"

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <cstring>

v4l::V4LFrameOutput::V4LFrameOutput(const std::string &deviceName) {
    _frameBuffer = new uint8_t[FrameW*FrameH*2];
    _v4l2fd = open(deviceName.c_str(), O_RDWR);
}

void v4l::V4LFrameOutput::initialiseVideo() {
    if (_v4l2fd == -1) {
        throw std::runtime_error(V4LFO_FailOpen);
    }

    struct v4l2_format v{};
    v.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if (ioctl(_v4l2fd, VIDIOC_G_FMT, &v) == -1) {
        throw std::runtime_error(V4LFO_FailSetupV);
    }
    v.fmt.pix.width = FrameW;
    v.fmt.pix.height = FrameH;
    v.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    v.fmt.pix.sizeimage = FrameW * FrameH * 4;
    v.fmt.pix.field = V4L2_FIELD_NONE;
    if (ioctl(_v4l2fd, VIDIOC_S_FMT, &v) == -1) {
        throw std::runtime_error(V4LFO_FailSetup);
    }
}

void v4l::V4LFrameOutput::videoFrameAvailable(uint32_t *image) {
    memcpy(_frameBuffer, image, FrameW*FrameH*2);
}

void v4l::V4LFrameOutput::display() {
    ssize_t bytesWritten = write(_v4l2fd, _frameBuffer, FrameW * FrameH * 2);
    if (bytesWritten < FrameW * FrameH * 2) {
        printf(V4LFO_FailBytes, bytesWritten, FrameW*FrameH*2);
    }
}

void v4l::V4LFrameOutput::shutdownVideo() {
    close(_v4l2fd);
}

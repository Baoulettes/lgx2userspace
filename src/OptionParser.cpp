#include <getopt.h>
#include <iostream>
#include "GlobalVar.h"
#include "OptionParser.h"
#include "PulseAudioOutput.h"
#include "ChronoLogger.h"
#include "V4LFrameOutput.h"

lgx2::VideoOutput *app::OptionParser::videoOutput() {
    return _videoOutput;
}

lgx2::AudioOutput *app::OptionParser::audioOutput() {
    return _audioOutput;
}

lgx2::Logger *app::OptionParser::logger() {
    return _logger;
}

bool app::OptionParser::process(int argc, char **argv) {
    for(;;)
    {
        switch(getopt(argc, argv, "vVa:d:hx"))
        {
            case 'a':
                std::cout << case_a << optarg << std::endl;
                _audioOutput = new pulse::PulseAudioOutput(optarg);
                continue;
            case 'v':
                std::cout << case_v1 << std::endl;
                _logger = new ChronoLogger(true);
                continue;
            case 'V':
                std::cout << case_v2 << std::endl;
                _logger = new ChronoLogger(false);
                continue;

            case 'd':
                std::cout << case_d << optarg << std::endl;
                _videoOutput = new v4l::V4LFrameOutput(optarg);
                continue;
            case 'x':
                std::cout << case_x << std::endl;
                _deviceType = libusb::LGXDeviceType::LGX;
                continue;
            case 'h':
            default :
                std::cout << argv[0] << case_none;
                return false;
            case -1:
                break;
        }

        break;
    }

    return true;
}

libusb::LGXDeviceType app::OptionParser::deviceType() {
    return _deviceType;
}

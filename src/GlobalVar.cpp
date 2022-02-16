#include "GlobalVar.h"
#include <string>

//Hold various variable to change change frame size easier.
const char* WindowName   =   "lgx2userspace";
const int FrameW  =   704;
const int FrameH  =   480;
const int WindowW =   1920;
const int WindowH =   1080;
const int WindowX =   1920;
const int WindowY =   100;

const int AudioChannel    =   2;
const int AudioFrequency  =   48000;
const int AudioSample     =   1024;

//Texts
//ChronoLogger.cpp
const std::string lte_avgns   =   " - avg. ns.: ";
const std::string summ_incl   =   "Summary (Include this in bug reports)";
const std::string summ_totl   =   "Total run time: ";
const std::string summ_ns     =   "ns (";
const std::string summ_s      =   "s)";
const std::string summ_avg    =   " - average ns: ";

//lgxdevice.cpp
const std::string run_Stream_TS   =   "streamUpdate";
const std::string run_Stream_TE   =   "streamUpdate";
const std::string run_Stream_TE2  =   "Stream update";
const std::string run_Video_TS    =   "videoDisplay";
const std::string run_Video_TE    =   "videoDisplay";
const std::string run_Video_TE2   =   "Video display update";
const std::string run_Audio_TS    =   "audioOutput";
const std::string run_Audio_TE    =   "audioOutput";
const std::string run_Audio_TE2   =   "Audio output render";

//OptionParser.cpp
const std::string case_a      =   "Attempting to output to Pulseaudio sink: ";
const std::string case_v1     =   "Logging diagnostics information at end of execution";
const std::string case_v2     =   "Logging diagnostics information - with output during execution ";
const std::string case_d      =   "Attempting to output to V4L2Loopback device: ";
const std::string case_x      =   "Using the LGX GC550 support";
const std::string case_none   =   " usage:\n\t-v\tPrint diagnostics information summary at end of execution, useful when submitting bugs\n\t-V\tPrint diagnostic information during execution\n\t-d V4L2LoopbackDevice\tSpecify the V4L2Loopback device to output video to (e.g. /dev/video99)\n\n";

//PulseAudioOutput.cpp
const char* PA_Title   =   "LGX2 Userspace driver";
const char* PA_type    =   "capture audio";
const std::string PA_Failed  =   "Failed to write audio data to pulseaudio\n";

//UsbStream.cpp
const std::string US_FailOpen    =   "Failed to open lgx2 - is it connected? Run lsusb to check";
const std::string US_FailConf    =   "Failed to set configuration\n";
const std::string US_FailClaim   =   "Could not claim interface for lgx2 - is something else using the device?\n";
const std::string US_UnderrunW   =   "Underrun when attempting to write command data for setup";
const std::string US_UnderrunR   =   "Underrun when attempting to read data during setup";

//V4LFrameOutput.cpp
const std::string V4LFO_FailOpen    =   "Failed to open V4L2 Loopback device";
const std::string V4LFO_FailSetupV  =   "Cannot setup V4L2Loopback device for video output";
const std::string V4LFO_FailSetup   =   "Cannot setup V4L2Loopback device";
const char* V4LFO_FailBytes   =   "Only wrote %ld bytes (expected %d)\n";


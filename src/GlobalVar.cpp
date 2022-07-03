#include "GlobalVar.h"
#include "ConfigFile.h"
#include "../Lib/mINI/src/mini/ini.h"
#include <string>
using namespace std;
//Hold various variable to change frame size easier.
const char* WindowName   =   "lgx2userspace";
const char* SettingFile   =   "settings.ini";
int GetFrameW(){
	int VideoMode = GetIntValue("VideoMode",0);
    if (VideoMode == 0){
        return 1920;
    } else if (VideoMode == 1){
        return 1280;
    } else if (VideoMode == 2){
        return 720;
    } else {
        mINI::INIFile file(SettingFile);
        mINI::INIStructure ini;
        file.read(ini);
        int result = std::stoi(ini["Settings"]["FrameW"]);
        if (result == NULL || static_cast<int>(result) != result){
            return 1920;
        }
    }
    return 1920;
}
int GetFrameH(){
	int VideoMode = GetIntValue("VideoMode",0);
    if (VideoMode == 0){
        return 1080;
    } else if (VideoMode == 1){
        return 720;
    } else if (VideoMode == 2){
        return 480;
    } else {
        mINI::INIFile file(SettingFile);
        mINI::INIStructure ini;
        file.read(ini);
        int result = std::stoi(ini["Settings"]["FrameH"]);
        if (result == NULL || static_cast<int>(result) != result){
            return 1080;
        }
    }
    return 1080;
}
int GetWinW(){
	mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    file.read(ini);
	int result = std::stoi(ini["Settings"]["WindowW"]);
    if (result == NULL || static_cast<int>(result) != result){
        return 1920;
    }
    return result;
}
int GetWinH(){
	mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    file.read(ini);
	int result = std::stoi(ini["Settings"]["WindowH"]);
    if (result == NULL || static_cast<int>(result) != result){
        return 1080;
    }
    return result;
}
const int FrameW  =   GetFrameW();
const int FrameH  =   GetFrameH();

const int WindowW =   GetWinW();
const int WindowH =   GetWinH();

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
const std::string case_lgx2   =   "Using the LGX GC551 support";
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

//ConfigFile.cpp
const std::string CF_FileOpenIssue   =   "There is some issues reading the file";
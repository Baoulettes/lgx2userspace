#pragma once

#include <string>

//Hold various variable to change change frame size easier.
extern const char* WindowName;
extern const char* SettingFile;
extern const int FrameW;
extern const int FrameH;
extern const int WindowW;
extern const int WindowH;
extern const int WindowX;
extern const int WindowY;

extern const int AudioChannel;
extern const int AudioFrequency;
extern const int AudioSample;

//Texts
//ChronoLogger.cpp
extern const std::string lte_avgns;
extern const std::string summ_incl;
extern const std::string summ_totl;
extern const std::string summ_ns;
extern const std::string summ_s;
extern const std::string summ_avg;

//lgxdevice.cpp
extern const std::string run_Stream_TS;
extern const std::string run_Stream_TE;
extern const std::string run_Stream_TE2;
extern const std::string run_Video_TS;
extern const std::string run_Video_TE;
extern const std::string run_Video_TE2;
extern const std::string run_Audio_TS;
extern const std::string run_Audio_TE;
extern const std::string run_Audio_TE2;

//OptionParser.cpp
extern const std::string case_a;
extern const std::string case_v1;
extern const std::string case_v2;
extern const std::string case_d;
extern const std::string case_x;
extern const std::string case_lgx2;
extern const std::string case_none;

//PulseAudioOutput.cpp
extern const char* PA_Title;
extern const char* PA_type;
extern const std::string PA_Failed;

//UsbStream.cpp
extern const std::string US_FailOpen;
extern const std::string US_FailConf;
extern const std::string US_FailClaim;
extern const std::string US_UnderrunW;
extern const std::string US_UnderrunR;

//ConfigFile.cpp
extern const std::string CF_FileOpenIssue;

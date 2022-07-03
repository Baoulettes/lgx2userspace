#include "../Lib/mINI/src/mini/ini.h"
#include "GlobalVar.h"
#include <iostream>
#include <fstream>
#include <string>
void SetNewFrameSize(uint16_t NewFrameW, uint16_t NewFrameH) {
	mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    if(file.read(ini) == true) {
        std::string NewFrame_H = std::to_string(NewFrameH);
        std::string NewFrame_W = std::to_string(NewFrameW);
        ini["Settings"]["FrameH"] = NewFrame_H;
        ini["Settings"]["FrameW"] = NewFrame_W;
        file.write(ini);
    } else {
        std::cout << CF_FileOpenIssue << std::endl;
    }
}
int GetLGXMode() {
	mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    if(file.read(ini) == true) {
        int LGXMode = atoi(ini["Settings"]["LGX2Mode"].c_str());
        if (LGXMode == 0 || LGXMode == 1) {
            return LGXMode;
        } else {
            return 0;
        }
    }
}
int GetIntValue(const char* VarName, int DefaultValue){
    mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    if(file.read(ini) == true) {
        int Value = atoi(ini["Settings"][VarName].c_str());
        if (Value != NULL) {
            return Value;
        } else {
            return DefaultValue;
        }
    }
}
void SetIntValue(const char* VarName, int NewValue) {
	mINI::INIFile file(SettingFile);
	mINI::INIStructure ini;
    if(file.read(ini) == true) {
        std::string NewFrame_H = std::to_string(NewValue);
        ini["Settings"][VarName] = NewFrame_H;
        file.write(ini);
    } else {
        std::cout << CF_FileOpenIssue << std::endl;
    }
}
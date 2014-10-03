#pragma once

#include <iostream>
#include <vector>
#include "include/INIReader.h"

struct Console {
    std::string Name;
    std::string Path;
    std::string RomPath;
    std::string RomExt;
    std::string Args;
};

class Settings {

public:
    Settings(const std::string& file);

    std::string get(const std::string& section, const std::string& name, const std::string& defaultValue);
    int get(const std::string& section, const std::string& name, int defaultValue);
    bool getBool(const std::string& section, const std::string& name, bool defaultValue);
    Console getConsole(const std::string& emulator);

private:
    INIReader _reader;

};

// Create a Settings
static Settings config("settings.ini");
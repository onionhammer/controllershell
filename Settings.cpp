#include "Settings.h"
#include "StringUtilities.h"

using namespace std;

#pragma region Settings

Settings::Settings(const string& file) : _reader(file) {
    // Load configuration
    if (_reader.ParseError())
        cerr << "Failed to parse settings" << endl;
}
int Settings::get(const string& section, const string& name, int defaultValue) {
    return (int)_reader.GetInteger(section, name, defaultValue);
}

string Settings::get(const string& section, const string& name, const string& defaultValue) {
    return _reader.Get(section, name, defaultValue);
}

bool Settings::getBool(const string& section, const string& name, bool defaultValue) {
    return _reader.GetBoolean(section, name, defaultValue);
}

Console Settings::getConsole(const string& emulator) {
    return {
        emulator,
        _reader.Get(emulator, "Path", "NA"),
        _reader.Get(emulator, "RomPath", ""),
        _reader.Get(emulator, "RomExt", ""),
        _reader.Get(emulator, "Args", "%s")
    };
}

#pragma endregion
#pragma once

#include <iostream>
#include <vector>

class Rom {

public:
    static std::vector<std::string> Find(const Console& console);
    static std::string CleanName(const std::string filename);
    static void Run(const Console& console, const std::string rom);
    static void Exit();
    
private:
    static std::vector<std::string> SplitArgs(const std::string command);

};
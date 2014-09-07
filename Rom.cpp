#include <cstdio>
#include <algorithm>

#if defined(_WIN32)

    #include "dirent.h"
    #include "windows.h"

    #define snprintf _snprintf

    static HANDLE _runningGame;

#else

    #include <dirent.h>
    #include <unistd.h>
    #include <signal.h>

    static pid_t _runningGame;

#endif

#include "Settings.h"
#include "Rom.h"
#include "StringUtilities.h"

using namespace std;

// Fill the input console with ROM filenames
vector<string> Rom::Find(const Console& console) {
    // Find ROM items in path
    vector<string> roms;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(console.RomPath.c_str())) != NULL) {
        // Find all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            string file(ent->d_name);

            // Filter by valid extension
            auto fileExtension = file.substr(file.find_last_of('.'));
            if (console.RomExt.size() == 0 || fileExtension == console.RomExt)
                roms.push_back(file);
        }

        closedir(dir);
    }

    else // Could not open directory
        switch (errno) {
            case ENOTDIR: cout << "Not a directory" << endl; break;
            case EACCES:  cout << "Permission denied" << endl; break;
            case ENOENT:  cout << "Directory not found" << endl; break;
            default:	  cout << "unknown error: " << errno << endl; break;
        }

    return roms;
}

// Generate a display name for the input filename
// - Remove extension
string Rom::CleanName(const string filename) {
    return filename.substr(0, filename.find_last_of("."));
}

// Run the emulator
void Rom::Run(const Console& console, const string rom) {

    char buffer[255];
    snprintf(buffer, 255, console.Args.c_str(), ("\"" + rom + "\"").c_str());

    // Display path used
    cout << console.Path << " " << buffer << endl;

#ifdef _WIN32

    auto workingDir = console.Path.substr(0, console.Path.find_last_of("\\"));

    SHELLEXECUTEINFO shExecInfo;

    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExecInfo.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = "runas";
    shExecInfo.lpFile = console.Path.c_str();
    shExecInfo.lpParameters = buffer;
    shExecInfo.lpDirectory = workingDir.c_str();
    shExecInfo.nShow = SW_HIDE;
    shExecInfo.hInstApp = NULL;

    if (ShellExecuteEx(&shExecInfo))
        _runningGame = shExecInfo.hProcess;

#else

    auto args = Rom::SplitArgs(console.Path + " " + buffer);

    vector<const char *> cstr_args;
    for (int i = 0; i < args.size(); ++i)
        cstr_args.push_back(args[i].c_str());
    cstr_args.push_back(NULL);

    _runningGame = fork();

    if (_runningGame == 0) {
        // Child process
        execv(cstr_args[0], (char**)&cstr_args[0]);

        // If exec fails then exit forked process.
        _exit(0);
    }

#endif
}

// Exit the emulator
void Rom::Exit() {
#if defined(_WIN32)

    TerminateProcess(_runningGame, 0);

#else

    kill(_runningGame, SIGKILL);

#endif
}

vector<string> Rom::SplitArgs(const string command) {
    vector<string> arguments;

    for (int i = 0; i < command.length();) {
        bool quote(false);
        stringstream stream;

        while (i < command.length()) {
            auto c = command[i++];

            if (!quote && c == ' ')
                break;
            else if (c == '"') {
                quote = !quote;
                continue;
            }

            stream.put(c);
        }

        arguments.push_back(stream.str());
    }

    return arguments;
}
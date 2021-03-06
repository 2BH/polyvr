#ifndef VRLOGGER_H_INCLUDED
#define VRLOGGER_H_INCLUDED

#include <string.h>
#include <map>

using namespace std;

class VRLog {
    private:
        static map<string, bool> tags;
        static void print(string tag, string s);

    public:
        static void log(string tag, string s);
        static void wrn(string tag, string s);
        static void err(string tag, string s);
        static void setTag(string tag, bool b);
        static bool tag(string tag);
};

#endif // VRLOGGER_H_INCLUDED

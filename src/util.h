#pragma once

#include <string>
#include <fstream>

using namespace std;

class Util {
    public:
        static ifstream getStream(string path);
};

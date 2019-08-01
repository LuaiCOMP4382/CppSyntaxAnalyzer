#include "util.h"
#include <string>
#include <fstream>

using namespace std;

ifstream Util::getStream(string path) {
    ifstream stream;
    stream.open(path, ifstream::in);

    if (!stream && !stream.is_open()) {
        stream.close();
        throw runtime_error("Non-existing source code");
    }
    
    return stream;
}

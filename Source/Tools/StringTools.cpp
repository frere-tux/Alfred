#include "StringTools.h"

#include <sstream>

void longToString(const long _long, std::string& _string)
{
    std::stringstream stream;
    stream << _long;
    _string = stream.str();
}

const char* longToString(const long _long)
{
    std::stringstream stream;
    stream << _long;
    return stream.str().c_str();
}


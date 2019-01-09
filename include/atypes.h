#ifndef ATYPES_H
#define ATYPES_H

#include <deque>
#include <string>
#include <sstream>

typedef std::deque<std::string> AStringList;

template<class T>
std::string join(const T &vec, const char *delim)
{
    switch (vec.size()) {
    case 0:
        return std::string();
    case 1:
        return *vec.begin();
    }
    std::stringstream result;
    auto i = vec.begin();
    auto j = vec.end();
    while (true) {
        result << *i;
        i++;
        if (i == j)
            return result.str();
        result << delim;
    }
}


#endif // ATYPES_H

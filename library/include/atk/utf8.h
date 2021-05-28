#ifndef UTF8_H
#define UTF8_H

#include <string>

std::wstring widen(const std::string &in);
std::string narrow(const std::wstring &in);

#endif // UTF8_H

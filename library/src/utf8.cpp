#include <windows.h>
#include <atk/utf8.h>

std::wstring widen(const std::string &in) {
    if (in.empty())
        return std::wstring();

    int len = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), -1, nullptr, 0);
    std::wstring out(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, &in[0], -1, &out[0], len);
    return out;
}


std::string narrow(const std::wstring &in)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string out(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, &in[0], -1, &out[0], len, nullptr, nullptr);
    return out;
}

#include "pch.h"
#include "Win32File.h"
#if __has_include("Win32File.g.cpp")
#include "Win32File.g.cpp"
#endif

namespace winrt::NfqLib::implementation
{
    int32_t Win32File::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Win32File::MyProperty(int32_t /*value*/)
    {
        throw hresult_not_implemented();
    }
}

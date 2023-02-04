#pragma once

#include "Win32File.g.h"

namespace winrt::NfqLib::implementation
{
    struct Win32File : Win32FileT<Win32File>
    {
        Win32File() = default;
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32File : Win32FileT<Win32File, implementation::Win32File>
    {
    };
}

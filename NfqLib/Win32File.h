#pragma once

#include "Win32File.g.h"

namespace winrt::NfqLib::implementation
{
    struct Win32File : Win32FileT<Win32File>
    {
        Win32File() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32File : Win32FileT<Win32File, implementation::Win32File>
    {
    };
}

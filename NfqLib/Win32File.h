#pragma once

#include "Win32File.g.h"

namespace winrt::NfqLib::implementation
{
    struct Win32File : Win32FileT<Win32File>
    {
        Win32File() = default;

        Win32File(const winrt::hstring& folderPath, WIN32_FIND_DATA findData);

        winrt::hstring Name();
        winrt::hstring Path();
        winrt::hstring Extension();
        winrt::hstring ItemTypeText();

        Windows::Foundation::DateTime DateCreated();
        Windows::Foundation::DateTime DateModified();
        Windows::Foundation::DateTime DateTaken();

        uint64_t Size();

    private:
        winrt::hstring m_name;
        winrt::hstring m_path;
        winrt::hstring m_extension;
        winrt::hstring m_itemTypeText;
            
        Windows::Foundation::DateTime m_dateCreated;
        Windows::Foundation::DateTime m_dateModified;
        Windows::Foundation::DateTime m_dateTaken;

        uint64_t m_size{ 0 };
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32File : Win32FileT<Win32File, implementation::Win32File>
    {
    };
}

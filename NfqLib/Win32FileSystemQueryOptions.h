#pragma once

#include "Win32FileSystemQueryOptions.g.h"

namespace winrt::NfqLib::implementation
{
    struct Win32FileSystemQueryOptions : Win32FileSystemQueryOptionsT<Win32FileSystemQueryOptions>
    {
        Win32FileSystemQueryOptions(
            const winrt::hstring& folderPath,
            const SortOrder& sortOrder,
            const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter,
            bool isRecursive);

        winrt::hstring FolderPath();
        SortOrder Sort();
        Windows::Foundation::Collections::IVector<winrt::hstring> FileTypeFilter();
        bool IsRecursive();

    private:
        winrt::hstring m_folderPath;
        SortOrder m_sort;
        Windows::Foundation::Collections::IVector<winrt::hstring> m_fileTypeFilter;
        bool m_isRecursive{ false };
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32FileSystemQueryOptions : Win32FileSystemQueryOptionsT<Win32FileSystemQueryOptions, implementation::Win32FileSystemQueryOptions>
    {
    };
}

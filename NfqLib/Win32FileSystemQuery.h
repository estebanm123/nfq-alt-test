#pragma once

#include "Win32FileSystemQuery.g.h"

namespace winrt::NfqLib::implementation
{
    struct Win32FileSystemQuery : Win32FileSystemQueryT<Win32FileSystemQuery>
    {
        Win32FileSystemQuery(const Win32FileSystemQueryOptions& queryOptions);

        Win32FileSystemQueryOptions QueryOptions();

        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<winrt::hstring>> EnumerateAsync();

        winrt::event_token FileSystemChanged(FileSystemChangedEventHandler const& handler) { return m_fileSystemChangede.add(handler); }
        void FileSystemChanged(winrt::event_token const& token) { m_fileSystemChangede.remove(token); }

    private:
        std::vector<winrt::hstring> FindNextFileEnumerate(const winrt::hstring folderPath, const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter);
        bool IsSupportedFileType(const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter, const winrt::hstring& fileType);

        Win32FileSystemQueryOptions m_queryOptions;
        winrt::event<FileSystemChangedEventHandler> m_fileSystemChangede;
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32FileSystemQuery : Win32FileSystemQueryT<Win32FileSystemQuery, implementation::Win32FileSystemQuery>
    {
    };
}

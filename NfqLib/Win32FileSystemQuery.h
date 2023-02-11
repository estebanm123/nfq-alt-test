#pragma once

#include "Win32FileSystemQuery.g.h"

#include <filesystem>

namespace winrt::NfqLib::implementation
{
    struct Win32FileSystemQuery : Win32FileSystemQueryT<Win32FileSystemQuery>
    {
        Win32FileSystemQuery(const Win32FileSystemQueryOptions& queryOptions);

        Win32FileSystemQueryOptions QueryOptions();

        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<NfqLib::Win32File>> EnumerateAsync();
        Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<NfqLib::Win32File>> SortAsync(NfqLib::SortOrder sortOrder);

        winrt::event_token FileSystemChanged(FileSystemChangedEventHandler const& handler) { return m_fileSystemChangede.add(handler); }
        void FileSystemChanged(winrt::event_token const& token) { m_fileSystemChangede.remove(token); }

    private:
        std::vector<NfqLib::Win32File> FindNextFileEnumerate(const winrt::hstring folderPath, const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter);
        bool IsSupportedFileType(const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter, const std::filesystem::path& fileTypeExtension);

        static bool CompareByNameAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByNameDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateCreatedAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateCreateDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateModifiedAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateModifiedDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateTakenAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDateTakenDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByTypeAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByTypeDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareBySizeAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareBySizeDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByTagsAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByTagsDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDimensionsAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByDimensionsDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByRatingAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool CompareByRatingDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2);
        static bool Win32FileSystemQuery::CompareNameAsWCharAscending(const wchar_t* item1Name, const wchar_t* item2Name);
        static bool Win32FileSystemQuery::CompareNameAsWCharDescending(const wchar_t* item1Name, const wchar_t* item2Name);

        Win32FileSystemQueryOptions m_queryOptions;
        winrt::event<FileSystemChangedEventHandler> m_fileSystemChangede;
        std::vector<NfqLib::Win32File> m_files;
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct Win32FileSystemQuery : Win32FileSystemQueryT<Win32FileSystemQuery, implementation::Win32FileSystemQuery>
    {
    };
}

#include "pch.h"
#include "Win32FileSystemQueryOptions.h"
#if __has_include("Win32FileSystemQueryOptions.g.cpp")
#include "Win32FileSystemQueryOptions.g.cpp"
#endif

namespace winrt::NfqLib::implementation
{
    Win32FileSystemQueryOptions::Win32FileSystemQueryOptions(
        const winrt::hstring& folderPath,
        const SortOrder& sort,
        const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter,
        bool isRecursive): 
        m_folderPath(folderPath), m_sort(sort), m_fileTypeFilter(fileTypeFilter), m_isRecursive(isRecursive)
    {
    }

    winrt::hstring Win32FileSystemQueryOptions::FolderPath()
    {
        return m_folderPath;
    }

    SortOrder Win32FileSystemQueryOptions::Sort()
    {
        return m_sort;
    }

    Windows::Foundation::Collections::IVector<winrt::hstring> Win32FileSystemQueryOptions::FileTypeFilter()
    {
        return m_fileTypeFilter;
    }

    bool Win32FileSystemQueryOptions::IsRecursive()
    {
        return m_isRecursive;
    }
}

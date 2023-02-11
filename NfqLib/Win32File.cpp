#include "pch.h"
#include "Win32File.h"
#if __has_include("Win32File.g.cpp")
#include "Win32File.g.cpp"
#endif

#include <filesystem>

namespace winrt::NfqLib::implementation
{
    Win32File::Win32File(const winrt::hstring& folderPath, WIN32_FIND_DATA findData)
    {
        m_name = findData.cFileName;

        std::filesystem::path fullPath = folderPath.c_str();
        fullPath /= m_name.c_str();
        m_path = fullPath.c_str();

        std::filesystem::path fileName = findData.cFileName;
        m_extension = fileName.extension().c_str();

        // TODO: setup property store
        m_itemTypeText = L"";

        m_dateCreated = winrt::clock::from_FILETIME(findData.ftCreationTime);

        m_dateModified = winrt::clock::from_FILETIME(findData.ftLastWriteTime);

        // TODO: setup property store
        m_dateTaken = winrt::clock::now();

        m_size = (static_cast<uint64_t>(findData.nFileSizeHigh) << 32) | findData.nFileSizeLow;
    }

    winrt::hstring Win32File::Name()
    {
        return m_name;
    }

    winrt::hstring Win32File::Path()
    {
        return m_path;

    }

    winrt::hstring Win32File::Extension()
    {
        return m_extension;

    }

    winrt::hstring Win32File::ItemTypeText()
    {
        return m_itemTypeText;

    }

    Windows::Foundation::DateTime Win32File::DateCreated()
    {
        return m_dateCreated;
    }

    Windows::Foundation::DateTime Win32File::DateModified()
    {
        return m_dateModified;
    }

    Windows::Foundation::DateTime Win32File::DateTaken()
    {
        return m_dateTaken;
    }

    uint64_t Win32File::Size()
    {
        return m_size;
    }
}

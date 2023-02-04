#include "pch.h"
#include "Win32FileSystemQuery.h"
#if __has_include("Win32FileSystemQuery.g.cpp")
#include "Win32FileSystemQuery.g.cpp"
#endif

#include "shlwapi.h"

namespace winrt::NfqLib::implementation
{
    Win32FileSystemQuery::Win32FileSystemQuery(const Win32FileSystemQueryOptions& queryOptions): 
        m_queryOptions(queryOptions)
    {
    }

    Win32FileSystemQueryOptions Win32FileSystemQuery::QueryOptions()
    {
        return m_queryOptions;
    }

    Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<winrt::hstring>> Win32FileSystemQuery::EnumerateAsync()
    {
		co_await winrt::resume_background();

		auto files =  FindNextFileEnumerate(m_queryOptions.FolderPath(), m_queryOptions.FileTypeFilter());

        co_return single_threaded_vector<winrt::hstring>(std::move(files));
    }

	std::vector<winrt::hstring> Win32FileSystemQuery::FindNextFileEnumerate(const winrt::hstring folderPath, const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter)
	{
		std::vector<winrt::hstring> files;
		winrt::hstring formattedFolderPath = folderPath + L"/*";

		WIN32_FIND_DATA findData;
		HANDLE handle = FindFirstFile(formattedFolderPath.c_str(), &findData);

		if (handle == INVALID_HANDLE_VALUE || handle == NULL)
		{
			return files;
		}

		do
		{
			winrt::hstring cFileName = findData.cFileName;
			auto extension = PathFindExtension(cFileName.c_str());

			if (extension != NULL && extension[0] != 0)
			{
				if (IsSupportedFileType(fileTypeFilter, extension))
				{
					files.push_back(cFileName);
				}
			}
		} 
		while (FindNextFile(handle, &findData));

		FindClose(handle);

		return files;
	}

	bool Win32FileSystemQuery::IsSupportedFileType(const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter, const winrt::hstring& fileTypeExtension)
	{
		for (auto filterExtension : fileTypeFilter)
		{
			if (filterExtension == fileTypeExtension)
			{
				return true;
			}
		}

		return false;
	}
}

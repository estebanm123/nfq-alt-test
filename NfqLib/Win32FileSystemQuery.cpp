#include "pch.h"
#include "Win32FileSystemQuery.h"
#if __has_include("Win32FileSystemQuery.g.cpp")
#include "Win32FileSystemQuery.g.cpp"
#endif

#include <cwctype>
#include <execution>
#include <unordered_set>
#include <Win32File.h>

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

    Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<NfqLib::Win32File>> Win32FileSystemQuery::EnumerateAsync()
    {
		co_await winrt::resume_background();

		auto files =  FindNextFileEnumerate(m_queryOptions.FolderPath(), m_queryOptions.FileTypeFilter());

        co_return single_threaded_vector<NfqLib::Win32File>(std::move(files));
    }

	Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<NfqLib::Win32File>> Win32FileSystemQuery::SortAsync(NfqLib::SortOrder sortOrder)
	{
		co_await resume_background();

		if (sortOrder == nullptr)
		{
			// Default is sorting by name
			std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByNameAscending : &Win32FileSystemQuery::CompareByNameDescending);
			co_return single_threaded_vector<NfqLib::Win32File>(std::move(m_files));
		}

		switch (sortOrder.Property())
		{
			case SortProperty::Name:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByNameAscending : &Win32FileSystemQuery::CompareByNameDescending);
				break;
			case SortProperty::Date:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByDateAscending : &Win32FileSystemQuery::CompareByDateDescending);
				break;
			case SortProperty::DateCreated:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByDateCreatedAscending : &Win32FileSystemQuery::CompareByDateCreateDescending);
				break;
			case SortProperty::DateModified:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByDateModifiedAscending : &Win32FileSystemQuery::CompareByDateModifiedDescending);
				break;
			case SortProperty::DateTaken:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByDateTakenAscending : &Win32FileSystemQuery::CompareByDateTakenDescending);
				break;
			case SortProperty::Type:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByTypeAscending : &Win32FileSystemQuery::CompareByTypeDescending);
				break;
			case SortProperty::Size:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareBySizeAscending : &Win32FileSystemQuery::CompareBySizeDescending);
				break;
			case SortProperty::Tags:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByTagsAscending : &Win32FileSystemQuery::CompareByTagsDescending);
				break;
			case SortProperty::Dimensions:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByDimensionsAscending : &Win32FileSystemQuery::CompareByDimensionsDescending);
				break;
			case SortProperty::Rating:
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByRatingAscending : &Win32FileSystemQuery::CompareByRatingDescending);
				break;
			default:
				// Default is sorting by name
				std::sort(std::execution::par_unseq, m_files.begin(), m_files.end(), sortOrder.IsAscending() ? &Win32FileSystemQuery::CompareByNameAscending : &Win32FileSystemQuery::CompareByNameDescending);
				break;
		}

		co_return single_threaded_vector<NfqLib::Win32File>(std::move(m_files));
	}

	std::vector<NfqLib::Win32File> Win32FileSystemQuery::FindNextFileEnumerate(const winrt::hstring folderPath, const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter)
	{
		m_files.clear();
		winrt::hstring formattedFolderPath = folderPath + L"/*";

		WIN32_FIND_DATA findData;
		HANDLE handle = FindFirstFile(formattedFolderPath.c_str(), &findData);

		if (handle == INVALID_HANDLE_VALUE || handle == NULL)
		{
			return m_files;
		}

		std::unordered_set<std::wstring> fileTypeFilterSet;
		for (auto typeExtension : fileTypeFilter)
		{
			fileTypeFilterSet.insert(typeExtension.c_str());
		}

		do
		{
			winrt::hstring cFileName = findData.cFileName;
			winrt::com_ptr<NfqLib::implementation::Win32File> win32File = winrt::make_self<NfqLib::implementation::Win32File>(folderPath, findData);

			if (fileTypeFilter != nullptr)
			{
				std::filesystem::path path = cFileName.c_str();

				if (path.has_extension())
				{
					auto extension = path.extension();
					std::wstring lowerExtension(extension.c_str());
					std::transform(lowerExtension.begin(), lowerExtension.end(), lowerExtension.begin(), std::towlower);

					if (fileTypeFilterSet.find(lowerExtension) != fileTypeFilterSet.end())
					{
						m_files.push_back(*win32File);
					}
				}
			}
			else
			{
				m_files.push_back(*win32File);
			}
		}
		while (FindNextFile(handle, &findData));

		FindClose(handle);

		return m_files;
	}

	bool Win32FileSystemQuery::IsSupportedFileType(const Windows::Foundation::Collections::IVector<winrt::hstring>& fileTypeFilter, const std::filesystem::path& fileTypeExtension)
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

	bool Win32FileSystemQuery::CompareByNameDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return CompareNameAsWCharAscending(item1.Name().c_str(), item2.Name().c_str());
	}

	bool Win32FileSystemQuery::CompareByNameAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return CompareNameAsWCharDescending(item1.Name().c_str(), item2.Name().c_str());
	}

	bool Win32FileSystemQuery::CompareByDateAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByDateDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByDateCreatedAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateCreated());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateCreated());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 < seconds2;
	}

	bool Win32FileSystemQuery::CompareByDateCreateDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateCreated());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateCreated());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 > seconds2;
	}

	bool Win32FileSystemQuery::CompareByDateModifiedAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateModified());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateModified());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 < seconds2;
	}

	bool Win32FileSystemQuery::CompareByDateModifiedDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateModified());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateModified());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 > seconds2;
	}

	bool Win32FileSystemQuery::CompareByDateTakenAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateTaken());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateTaken());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 < seconds2;
	}

	bool Win32FileSystemQuery::CompareByDateTakenDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		auto seconds1 = std::chrono::time_point_cast<std::chrono::seconds>(item1.DateTaken());
		auto seconds2 = std::chrono::time_point_cast<std::chrono::seconds>(item2.DateTaken());

		return seconds1 == seconds2 ? CompareByNameAscending(item1, item2) : seconds1 > seconds2;
	}

	bool Win32FileSystemQuery::CompareByTypeAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByTypeDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareBySizeAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return item1.Size() < item2.Size();
	}

	bool Win32FileSystemQuery::CompareBySizeDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return item1.Size() > item2.Size();
	}

	bool Win32FileSystemQuery::CompareByTagsAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByTagsDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByDimensionsAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByDimensionsDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByRatingAscending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareByRatingDescending(const NfqLib::Win32File& item1, const NfqLib::Win32File& item2)
	{
		return true;
	}

	bool Win32FileSystemQuery::CompareNameAsWCharAscending(const wchar_t* item1Name, const wchar_t* item2Name)
	{
		return (CSTR_GREATER_THAN == CompareStringW(LOCALE_NAME_USER_DEFAULT, NORM_IGNORECASE | SORT_DIGITSASNUMBERS, item1Name, -1, item2Name, -1));
	}

	bool Win32FileSystemQuery::CompareNameAsWCharDescending(const wchar_t* item1Name, const wchar_t* item2Name)
	{
		return (CSTR_LESS_THAN == CompareStringW(LOCALE_NAME_USER_DEFAULT, NORM_IGNORECASE | SORT_DIGITSASNUMBERS, item1Name, -1, item2Name, -1));
	}
}

#include "pch.h"
#include "FindNextFileTest.h"
#include <windows.h>
#include <iostream>
#include "Utils.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Foundation.h"
#include <shobjidl.h>
#include <propvarutil.h>
#include <propkey.h>
#include <shobjidl.h>
#include <winrt/Windows.Storage.FileProperties.h>

namespace WS = winrt::Windows::Storage;
namespace WF = winrt::Windows::Foundation;

struct TestImageProps
{
	uint32_t width;
	uint32_t height;
	uint64_t dateTaken;
};

void fetchWin32PropsTest(
	uint32_t activatedFileIdx, 
	std::wstring const& activatedFilePath, 
	std::wstring const& searchPath,
	std::vector<WIN32_FIND_DATA> const& files)
{
	// TODO: test on dehyrdated files
	auto prePropStoreFetchT = Clock::now();
	winrt::com_ptr<IPropertyStore> propStore;
	SHGetPropertyStoreFromParsingName(activatedFilePath.c_str(), NULL, GPS_DEFAULT, IID_PPV_ARGS(&propStore));
	auto postPropStoreFetchT = Clock::now();

	std::vector < winrt::com_ptr<IPropertyStore>> propStores;
	auto prePropGroupStoreFetchT = Clock::now();
	for (auto i = activatedFileIdx - FirstLoadStorageFileCount / 2.f; i < activatedFileIdx + FirstLoadStorageFileCount / 2.f; i++)
	{
		auto filePath = searchPath + L"\\" + files[i].cFileName;
		winrt::com_ptr<IPropertyStore> pStore;
		SHGetPropertyStoreFromParsingName(filePath.c_str(), NULL, GPS_DEFAULT, IID_PPV_ARGS(&pStore));
		propStores.push_back(pStore);
	}
	auto postPropGroupStoreFetchT = Clock::now();

	auto preDimPropFetchT = Clock::now();
	PROPVARIANT propvarValue = { 0 };
	TestImageProps props;
	auto hrGetValue = propStore->GetValue(PKEY_Image_HorizontalSize, &propvarValue);
	if (SUCCEEDED(hrGetValue))
	{
		props.width = propvarValue.uintVal;
		PropVariantClear(&propvarValue);
	}

	hrGetValue = propStore->GetValue(PKEY_Image_VerticalSize, &propvarValue);
	if (SUCCEEDED(hrGetValue))
	{
		props.height = propvarValue.uintVal;
		PropVariantClear(&propvarValue);
	}
	auto postDimPropFetchT = Clock::now();

	auto preDateTakenPropFetchT = Clock::now();
	hrGetValue = propStore->GetValue(PKEY_Photo_DateTaken, &propvarValue);
	if (SUCCEEDED(hrGetValue))
	{
		props.dateTaken = fileTimeTo100Ns(propvarValue.filetime);
		PropVariantClear(&propvarValue);
	}
	auto postDateTakenPropFetchT = Clock::now();

	printf("single prop store fetch time: %lu ms\n", toMs(postPropStoreFetchT - prePropStoreFetchT));
	printf("prop store group fetch time: %lu ms\n", toMs(postPropGroupStoreFetchT - prePropGroupStoreFetchT));
	printf("size: %lu ms\n", propStores.size());
	printf("dimensions fetch time %lu us\n", toUs(postDimPropFetchT - preDimPropFetchT));
	printf("datetaken fetch time: %lu us\n", toUs(postDateTakenPropFetchT - preDateTakenPropFetchT));
}

WF::IAsyncAction fetchStorageFileAndPropsTest(
	uint32_t activatedFileIdx, 
	std::wstring const& activatedFilePath, 
	std::wstring const& searchPath,
	std::vector<WIN32_FIND_DATA> const& files)
{
	auto preStorageFileFetchT = Clock::now();
	auto file = co_await WS::StorageFile::GetFileFromPathAsync(activatedFilePath);
	auto postStorageFileFetchT = Clock::now();

	auto preGroupStorageFileFetchT = Clock::now();
	// Simulation of fetching StorageFile and Properties from property store
	std::vector<WS::StorageFile> storageFiles;
	for (auto i = activatedFileIdx - FirstLoadStorageFileCount / 2.f; i < activatedFileIdx + FirstLoadStorageFileCount / 2.f; i++)
	{
		auto filePath = searchPath + L"\\" + files[i].cFileName;
		auto file = co_await WS::StorageFile::GetFileFromPathAsync(filePath);
		storageFiles.push_back(file);
	}
	auto postGroupStorageFileFetchT = Clock::now();
	printf("Single storageFile fetch: %lu\n", toMs(postStorageFileFetchT - preStorageFileFetchT));
	printf("FirstLoad group StorageFile fetch: %lu\n", toMs(postGroupStorageFileFetchT - preGroupStorageFileFetchT));
	printf("Num StorageFiles fetched: %lu\n", storageFiles.size());

	auto prePropsFetchT = Clock::now();
	auto imageProps = co_await file.Properties().GetImagePropertiesAsync();
	auto postPropsFetchT = Clock::now();

	auto preGroupPropsFetchT = Clock::now();
	std::vector<WS::FileProperties::ImageProperties> propsList;
	for (auto const& file : storageFiles)
	{
		auto imageProps = co_await file.Properties().GetImagePropertiesAsync();
		propsList.push_back(imageProps);
	}
	auto postGroupPropsFetchT = Clock::now();

	printf("Image prop fetch: %lu\n", toMs(postPropsFetchT - prePropsFetchT));
	printf("Group image prop fetch: %lu\n", toMs(postGroupPropsFetchT - preGroupPropsFetchT));
}

std::vector<WIN32_FIND_DATA> executeQueryTest(std::wstring searchPath)
{
	auto formattedPath = searchPath + L"/*";
	LPCWSTR parentPath = formattedPath.c_str();

	WIN32_FIND_DATA findData;

	auto preFindFirstFileT = Clock::now();

	// TODO: would we ever need to support ANSI alternative? - in/out str types are different.
	HANDLE handle = FindFirstFile(parentPath, &findData);

	// Tried this alternative API that provides more options: ~5x slower for some reason
	//HANDLE handle = FindFirstFileEx(
	//	parentPath, 
	//	FINDEX_INFO_LEVELS::FindExInfoBasic, 
	//	&findData, 
	//	FINDEX_SEARCH_OPS::FindExSearchMaxSearchOp, 
	//	nullptr, // required to be null
	//	0);
	if (handle == INVALID_HANDLE_VALUE || handle == NULL)
	{
		printf("Invalid handle from FindFirstFile\n");
		return {};
	}

	bool hasNext = false;

	std::wstring dummyFileName = L"h329i0abmlw12!I(O)#";

	auto preloopT = Clock::now();
	std::vector<WIN32_FIND_DATA> files;
	do
	{
		// TODO: can maybe optimize by avoid creating a new std::wstring and just working with the wchar* name
		auto name = findData.cFileName;
		auto tmp = std::wstring(name);
		auto pos = tmp.find_last_of(L".");

		if (pos != std::string::npos)
		{
			auto ext = tmp.substr(pos); 

			if (dummyFileName == tmp)
			{
				printf("This conditional block is just to simulate a check we may have to do");
			}

			if (supportedExtensions.find(ext) != supportedExtensions.end())
			{
				files.push_back(findData);
			}
		}

		hasNext = FindNextFile(handle, &findData);
	} while (hasNext);
	auto doneT = Clock::now();
	
	// Sample sorting code
	auto preloop2T = Clock::now();
	std::sort(files.begin(), files.end(),
		[](auto const& a, auto const& b) { return fileTimeTo100Ns(a.ftCreationTime) < fileTimeTo100Ns(b.ftCreationTime); });
	auto loop2DoneT = Clock::now();

	printf("Loop: %lu\n", toMs(doneT - preloopT));
	printf("Loop w/ initial find: %lu\n", toMs(doneT - preFindFirstFileT));
	printf("Sorting time: %lu\n", toMs(loop2DoneT - preloop2T));
	printf("FileCount: %lu\n", files.size());

	auto lastError = GetLastError();
	if (lastError != ERROR_NO_MORE_FILES)
	{
		printf("error: %u\n", lastError);
	}
	return files;
}

void executeFindNextFileTest(std::wstring searchPath)
{
	auto files = executeQueryTest(searchPath);

	std::replace(searchPath.begin(), searchPath.end(), '/', '\\');
	// TODO: put in loop choosing more random idxs
	auto randActivatedFileIdx = files.size() / 2;
	auto activatedFileData = files[randActivatedFileIdx]; // use a 'random' file to simulate the activated file
	auto activatedFilePath = searchPath + L"\\" + activatedFileData.cFileName;
	printf("\n");
	printf("Test activatd file: %ls\n", activatedFilePath.c_str());
	fetchStorageFileAndPropsTest(randActivatedFileIdx, activatedFilePath, searchPath, files).get();
	printf("\n");
	fetchWin32PropsTest(randActivatedFileIdx, activatedFilePath, searchPath, files);
}

#include "pch.h"
#include "FindNextFileTest.h"
#include <windows.h>
#include <iostream>
#include <shobjidl.h>
#include <propvarutil.h>
#include <propkey.h>
#include <shobjidl.h>
#include <thumbnailstreamcache.h>
#include <shcore.h>
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Foundation.h"
#include <winrt/Windows.Storage.FileProperties.h>
#include "Utils.h"

constexpr uint32_t RequestedThumbnailSize = 1280;

namespace WS = winrt::Windows::Storage;
namespace WF = winrt::Windows::Foundation;

struct TestImageProps
{
	uint32_t width;
	uint32_t height;
	uint64_t dateTaken;
	uint64_t thumbCacheId;
};


WF::IAsyncOperation<WS::StorageFile> loadStorageFile(std::wstring path)
{
	co_await winrt::resume_background();


}

void executeWin32MediaLoadingAltTest(
	uint32_t activatedFileIdx,
	std::wstring const& activatedFilePath,
	std::wstring const& searchPath,
	std::vector<WIN32_FIND_DATA> const& files)
{
	auto preShellItemInitT = Clock::now();
	winrt::com_ptr<IShellItem> shellItem;
	auto hr = SHCreateItemFromParsingName(activatedFilePath.c_str(), NULL, IID_PPV_ARGS(&shellItem));
	if (FAILED(hr))
	{
		printf("Error creating shell item");
	}

	auto postShellItemInitT = Clock::now();
	printf("Creating shell item: %lu us", toUs(postShellItemInitT - preShellItemInitT));
}



void executeWin32MediaLoadingTest(
	uint32_t activatedFileIdx, 
	std::wstring const& activatedFilePath, 
	std::wstring const& searchPath,
	std::vector<WIN32_FIND_DATA> const& files)
{
	auto prePropStoreFetchT = Clock::now();
	winrt::com_ptr<IPropertyStore> propStore;
	auto hr = SHGetPropertyStoreFromParsingName(activatedFilePath.c_str(), NULL, GPS_DEFAULT, IID_PPV_ARGS(&propStore));
	if (FAILED(hr))
	{
		printf("Error fetching propertystore");
	}
	auto postPropStoreFetchT = Clock::now();

	std::vector < winrt::com_ptr<IPropertyStore>> propStores;
	auto prePropGroupStoreFetchT = Clock::now();
	for (auto i = activatedFileIdx - FirstLoadStorageFileCount / 2.f; i < activatedFileIdx + FirstLoadStorageFileCount / 2.f; i++)
	{
		auto filePath = searchPath + L"\\" + files[i].cFileName;
		winrt::com_ptr<IPropertyStore> pStore;
		auto hr = SHGetPropertyStoreFromParsingName(activatedFilePath.c_str(), NULL, GPS_DEFAULT, IID_PPV_ARGS(&propStore));
		if (FAILED(hr))
		{
			printf("Error fetching propertystore");
		}
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

	auto preThumbCacheIdPropFetchT = Clock::now();
	hrGetValue = propStore->GetValue(PKEY_ThumbnailCacheId, &propvarValue);
	if (SUCCEEDED(hrGetValue))
	{
		props.thumbCacheId = propvarValue.uintVal;
		PropVariantClear(&propvarValue);
	}
	auto postThumbCacheIdPropFetchT = Clock::now();

	auto preStorageProviderPropFetchT = Clock::now();
	hrGetValue = propStore->GetValue(PKEY_StorageProviderId, &propvarValue);
	if (SUCCEEDED(hrGetValue))
	{
		props.thumbCacheId = propvarValue.uintVal;
		PropVariantClear(&propvarValue);
	}
	auto postStorageProviderPropFetchT = Clock::now();

	printf("single prop store fetch time: %lu ms\n", toMs(postPropStoreFetchT - prePropStoreFetchT));
	printf("prop store group fetch time: %lu ms\n", toMs(postPropGroupStoreFetchT - prePropGroupStoreFetchT));
	printf("size: %lu\n", propStores.size());
	printf("dimensions fetch time %lu us\n", toUs(postDimPropFetchT - preDimPropFetchT));
	printf("datetaken fetch time: %lu us\n", toUs(postDateTakenPropFetchT - preDateTakenPropFetchT));
	printf("provider id fetch time: %lu us\n", toUs(postStorageProviderPropFetchT - preStorageProviderPropFetchT));
	printf("thumbCacheId fetch time: %lu us\n", toUs(postThumbCacheIdPropFetchT - preThumbCacheIdPropFetchT));

	printf("\n");
	auto preThumbCacheFetchT = Clock::now();
	winrt::com_ptr<IThumbnailStreamCache> cache;
	auto hrCreateCache = CoCreateInstance(__uuidof(ThumbnailStreamCache), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&cache));
	if (FAILED(hrCreateCache))
	{
		printf("Error fetching thumb cache: %d\n", hrCreateCache);
		return;
	}
	auto postThumbCacheFetchT = Clock::now();

	auto preThumbFetchT = Clock::now();
	winrt::com_ptr<IStream> cacheThumbnailIStream;
	// TODO: try extractifnotcached - this is used for filmstrip small icon thumbs? vs placeholder uses allowsmallersize
	ThumbnailStreamCacheOptions thumbnailOptions = ThumbnailStreamCacheOptions::AllowSmallerSize; // ThumbnailStreamCacheOptions::ExtractIfNotCached;
	SIZE actualSize{ 0,0 };
	auto hrGetStream = cache->GetThumbnailStream(activatedFilePath.c_str(), props.thumbCacheId, thumbnailOptions, RequestedThumbnailSize, &actualSize, cacheThumbnailIStream.put());
	if (FAILED(hrGetStream))
	{
		printf("Error fetching thumb from cache: %d\n", hrGetStream);
		return;
	}
	auto postThumbFetchT = Clock::now();

	auto preThumbRasConvertT = Clock::now();
	winrt::com_ptr<WS::Streams::IRandomAccessStream> randomAccessStream;
	auto streamIID{ winrt::guid_of<WS::Streams::IRandomAccessStream>() };
	auto hrCreateStream = CreateRandomAccessStreamOverStream(cacheThumbnailIStream.get(), BSOS_DEFAULT, streamIID, randomAccessStream.put_void());
	if (FAILED(hrCreateStream))
	{
		printf("Error fetching thumb from cache: %d\n", hrCreateStream);
	}
	auto postThumbRasConvertT = Clock::now();

	printf("thumb cache fetch time: %lu ms\n", toMs(postThumbCacheFetchT - preThumbCacheFetchT));
	printf("thumb fetch time: %lu ms\n", toMs(postThumbFetchT - preThumbFetchT));
	printf("stream creation: %lu ms\n", toMs(postThumbRasConvertT - preThumbRasConvertT));


	auto preReadT = Clock::now();
	IStream *stream = 0; // Todo: try using com_ptr
	if (FAILED(SHCreateStreamOnFileEx(activatedFilePath.c_str(), STGM_READ | STGM_SHARE_DENY_WRITE, 0, false, 0, &stream)))
	{
		printf("Failure reading stream\n");
	}
	auto postReadT = Clock::now();
	// TODO: release stream
	printf("SHCreateStreamOnFileEx: %lu ms", toMs(postReadT - preReadT));
}

WF::IAsyncAction executeStorageFileMediaLoadingTest(
	uint32_t activatedFileIdx, 
	std::wstring const& activatedFilePath, 
	std::wstring const& searchPath,
	std::vector<WIN32_FIND_DATA> const& files)
{
	auto preStorageFileFetchT = Clock::now();
	auto activatedFile = co_await WS::StorageFile::GetFileFromPathAsync(activatedFilePath);
	auto postStorageFileFetchT = Clock::now();

	auto preFirstLoadStorageFileFetchT = Clock::now();
	std::vector<WS::StorageFile> firstLoadStorageFiles;
	std::vector<WF::IAsyncOperation<WS::StorageFile>> promises;
	for (auto i = activatedFileIdx - FirstLoadStorageFileCount / 2.f; i < activatedFileIdx + FirstLoadStorageFileCount / 2.f; i++)
	{
		auto filePath = searchPath + L"\\" + files[i].cFileName;
		promises.push_back(WS::StorageFile::GetFileFromPathAsync(filePath));
	}
	for (auto const& promise : promises)
	{
		auto file = co_await promise;
		firstLoadStorageFiles.push_back(file);
	}
	auto postFirstLoadStorageFileFetchT = Clock::now();

	promises.clear();

	auto preBulkLoadStorageFileFetchT = Clock::now();
	std::vector<WS::StorageFile> bulkLoadStorageFiles;
	for (auto i = activatedFileIdx - BulkLoadStorageFileCount / 2.f; i < activatedFileIdx + BulkLoadStorageFileCount / 2.f; i++)
	{
		auto filePath = searchPath + L"\\" + files[i].cFileName;
		promises.push_back(WS::StorageFile::GetFileFromPathAsync(filePath));
	}
	for (auto const& promise : promises)
	{
		auto file = co_await promise;
		bulkLoadStorageFiles.push_back(file);
	}
	auto postBulkStorageFileFetchT = Clock::now();

	printf("Single storageFile fetch: %lu\n", toMs(postStorageFileFetchT - preStorageFileFetchT));
	printf("FirstLoad %u group StorageFile fetch: %lu\n", firstLoadStorageFiles.size(), toMs(postFirstLoadStorageFileFetchT - preFirstLoadStorageFileFetchT));
	printf("BulkLoad %u group StorageFile fetch: %lu\n", bulkLoadStorageFiles.size(), toMs(postBulkStorageFileFetchT - preBulkLoadStorageFileFetchT));

	auto prePropsFetchT = Clock::now();
	auto imageProps = co_await activatedFile.Properties().GetImagePropertiesAsync();
	auto postPropsFetchT = Clock::now();

	auto preGroupPropsFetchT = Clock::now();
	std::vector<WS::FileProperties::ImageProperties> propsList;
	for (auto const& file : firstLoadStorageFiles)
	{
		auto imageProps = co_await file.Properties().GetImagePropertiesAsync();
		propsList.push_back(imageProps);
	}
	auto postGroupPropsFetchT = Clock::now();

	printf("Image prop fetch: %lu\n", toMs(postPropsFetchT - prePropsFetchT));
	printf("Group image prop fetch: %lu\n", toMs(postGroupPropsFetchT - preGroupPropsFetchT));


	auto preThumbFetchT = Clock::now();
	auto thumb = co_await activatedFile.GetThumbnailAsync(WS::FileProperties::ThumbnailMode::SingleItem, RequestedThumbnailSize, WS::FileProperties::ThumbnailOptions::None);
	auto postThumbFetchT = Clock::now();

	printf("Thumb fetch: %lu\n", toMs(postThumbFetchT - preThumbFetchT));

	//auto preOpenT = Clock::now();
	//auto stream = co_await activatedFile.OpenReadAsync();
	//auto postOpenT = Clock::now();

	//printf("OpenReadAsync: %lu\n", toMs(postOpenT - preOpenT));
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
	executeStorageFileMediaLoadingTest(randActivatedFileIdx, activatedFilePath, searchPath, files).get();
	printf("\n");
	executeWin32MediaLoadingTest(randActivatedFileIdx, activatedFilePath, searchPath, files);
	//executeWin32MediaLoadingAltTest(randActivatedFileIdx, activatedFilePath, searchPath, files);
}

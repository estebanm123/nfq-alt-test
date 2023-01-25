#include "WindowsStorageQueryTest.h"

#include "pch.h"
#include "Utils.h"
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Search.h"

namespace WF = winrt::Windows::Foundation;
namespace WS = winrt::Windows::Storage;

WF::IAsyncAction executeAsync(std::wstring const& searchPath, WS::Search::IndexerOption indexerOption)
{
	try
	{
		auto supportedExtList = winrt::single_threaded_vector<winrt::hstring>();
		for (auto const& ext : supportedExtensions)
		{
			supportedExtList.Append(ext);
		}

		auto preFolderFetchT = Clock::now();
		auto folder = co_await WS::StorageFolder::GetFolderFromPathAsync(searchPath);
		auto postFolderFetchT = Clock::now();
		
		auto preQueryCreationT = Clock::now();
		WS::Search::QueryOptions options(WS::Search::CommonFileQuery::DefaultQuery, {});
		options.IndexerOption(indexerOption);
		auto query = folder.CreateFileQueryWithOptions(options);
		auto postQueryCreationT = Clock::now();

		auto preItemCountT = Clock::now();
		auto  itemCount = co_await query.GetItemCountAsync();
		auto postItemCountT = Clock::now();

		auto preFindIdxT = Clock::now();
		auto idx = co_await query.FindStartIndexAsync(winrt::box_value(L"236897881.jpg"));
		auto postFindIdxT = Clock::now();

		auto preQueryExecutionT = Clock::now();
		auto files = co_await query.GetFilesAsync();
		auto postQueryExecutionT = Clock::now();
;
		printf("items queried: %d\n", files.Size());
		printf("GetFolderFromPathAsync %lu \n", toMs(postFolderFetchT - preFolderFetchT));
		printf("Query initialization: %lu \n", toMs(postQueryCreationT - preQueryCreationT));
		printf("GetFilesAsync: %lu \n", toMs(postQueryExecutionT - preQueryExecutionT));
		printf("FindStartIdx: %lu \n", toMs(postFindIdxT - preFindIdxT));
		printf("GetItemCount: %lu \n", toMs(postItemCountT - preItemCountT));
	}
	catch (winrt::hresult_error e)
	{
		printf("Error executing WinStorage query test: %d\n", e.code());
	}
}

void executeWinStorageFolderQueryTest(std::wstring searchPath)
{
	std::replace(searchPath.begin(), searchPath.end(), '/', '\\');

	printf("executing WinStorageFolderQueryTest - without indexer usage\n");
	auto promise = executeAsync(searchPath, WS::Search::IndexerOption::DoNotUseIndexer);
	promise.get();
	printf("executing WinStorageFolderQueryTest - with forced indexer usage\n");
	promise = executeAsync(searchPath, WS::Search::IndexerOption::OnlyUseIndexer);
	promise.get();
}

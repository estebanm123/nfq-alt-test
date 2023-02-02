#include "pch.h"
#include "StorageFileQueryBenchmark.h"
#if __has_include("StorageFileQueryBenchmark.g.cpp")
#include "StorageFileQueryBenchmark.g.cpp"
#endif

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Search.h>

#include "QueryBenchmarkUtils.h"

namespace WS = winrt::Windows::Storage;
namespace WSS = winrt::Windows::Storage::Search;

namespace winrt::NfqLib::implementation
{
	winrt::hstring toString(WSS::IndexerOption indexerOption)
	{
		switch (indexerOption)
		{
		case WSS::IndexerOption::DoNotUseIndexer: return L"No indexer";
		case WSS::IndexerOption::OnlyUseIndexer: return L"Indexer";
		default:
		assert(!L"Not implemented");
		}
	}

	void ExecuteWithIndexerOption(WSS::IndexerOption indexerOption, WS::StorageFolder folder, WFC::IVector<BenchmarkResult> & benchmarkResults)
	{
		auto query = RunBenchmark<WSS::StorageFileQueryResult>(
			L"CreateFileQuery - " + toString(indexerOption),
			[=]() 
			{ 
				WS::Search::QueryOptions options(WS::Search::CommonFileQuery::DefaultQuery, {});
				options.IndexerOption(indexerOption);
				return folder.CreateFileQueryWithOptions(options);
			},
			benchmarkResults
		);

		auto itemCount = RunBenchmark<uint32_t>(
			L"GetItemCountAsync - " + toString(indexerOption),
			[=]()
			{
				return query.GetItemCountAsync().get();
			},
			benchmarkResults
		);

		if (itemCount > 100)
		{
			auto files = RunBenchmark<WFC::IVectorView<WS::StorageFile>>(
				L"GetFilesAsync (first 100 files) - " + toString(indexerOption),
				[=]()
				{
					return query.GetFilesAsync(0, 100).get();
				},
				benchmarkResults
			);
		}

		auto files = RunBenchmark<WFC::IVectorView<WS::StorageFile>>(
			L"GetFilesAsync (all files) - " + toString(indexerOption),
			[=]()
			{
				return query.GetFilesAsync().get();
			},
			benchmarkResults
		);

	}

	WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> StorageFileQueryBenchmark::Execute(winrt::hstring queryFolderPath)
	{
		auto strongThis{ get_strong() };
		apartment_context callingThread;
		co_await resume_background();

		WFC::IVector<BenchmarkResult> benchmarkResults = winrt::single_threaded_vector<BenchmarkResult>();

		auto folderPath = std::wstring(queryFolderPath.c_str());
		std::replace(folderPath.begin(), folderPath.end(), '/', '\\');

		auto storageFolder = RunBenchmark<WS::StorageFolder>(
			L"GetFolderFromPathAsync",
			[=]() 
			{ 
				return WS::StorageFolder::GetFolderFromPathAsync(folderPath).get(); 
			},
			benchmarkResults
		);

		ExecuteWithIndexerOption(WSS::IndexerOption::DoNotUseIndexer, storageFolder, benchmarkResults);
		ExecuteWithIndexerOption(WSS::IndexerOption::OnlyUseIndexer, storageFolder, benchmarkResults);

		co_await callingThread;
		co_return benchmarkResults;
	}

}

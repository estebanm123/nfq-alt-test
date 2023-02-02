#include "pch.h"
#include "StorageFileQueryBenchmark.h"
#if __has_include("StorageFileQueryBenchmark.g.cpp")
#include "StorageFileQueryBenchmark.g.cpp"
#endif

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Search.h>

#include "QueryBenchmarkUtils.h"

namespace WS = winrt::Windows::Storage;
namespace WSS = winrt::Windows::Storage::Streams;

namespace winrt::NfqLib::implementation
{
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

		// TODO: force indexer usage vs not

		co_await callingThread;
		co_return benchmarkResults;
	}
}

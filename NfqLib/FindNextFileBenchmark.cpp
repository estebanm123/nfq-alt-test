#include "pch.h"
#include "FindNextFileBenchmark.h"
#if __has_include("FindNextFileBenchmark.g.cpp")
#include "FindNextFileBenchmark.g.cpp"
#endif
#include "QueryBenchmarkUtils.h"

#include <exception>

namespace winrt::NfqLib::implementation
{
	std::vector<WIN32_FIND_DATA> FindNextFileMainQueryHelper(LPCWSTR parentPath)
	{
		WIN32_FIND_DATA findData;
		HANDLE handle = FindFirstFile(parentPath, &findData);

		if (handle == INVALID_HANDLE_VALUE || handle == NULL)
		{
			throw std::runtime_error("Invalid FindFirstFile handle");
		}

		std::vector<WIN32_FIND_DATA> files;
		std::wstring dummyFileName = L"h329i0abmlw12!I(O)#";
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

				if (SupportedExtensions.find(ext) != SupportedExtensions.end())
				{
					files.push_back(findData);
				}
			}
		} while (FindNextFile(handle, &findData));
		auto lastError = GetLastError();
		if (lastError != ERROR_NO_MORE_FILES)
		{
			throw std::runtime_error("Error enumerating with FindNextFile: " + lastError);
		}
		return files;
	}

    WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> FindNextFileBenchmark::Execute(winrt::hstring queryFolderPath)
    {
		auto strongThis{ get_strong() };
		apartment_context callingThread;
		co_await resume_background();

		auto formattedPath = queryFolderPath + L"/*";
		LPCWSTR parentPath = formattedPath.c_str();

		WFC::IVector<BenchmarkResult> benchmarkResults = winrt::single_threaded_vector<BenchmarkResult>();
		std::vector<WIN32_FIND_DATA> files = RunBenchmark<std::vector<WIN32_FIND_DATA>>(
			L"Entire directory enumeration",
			[=]() { return FindNextFileMainQueryHelper(parentPath); },
			benchmarkResults
		);

		RunBenchmark(
			L"Sorting",
			[&]() 
			{ 
				std::sort(files.begin(), files.end(),
				// TODO: pass in a custom sort property
				[](auto const& a, auto const& b) { return FileTimeTo100Ns(a.ftCreationTime) < FileTimeTo100Ns(b.ftCreationTime); });
			},
			benchmarkResults
		);

		co_await callingThread;
		co_return benchmarkResults;
    }
}

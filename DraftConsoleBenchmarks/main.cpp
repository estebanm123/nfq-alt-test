#include "pch.h" 
#include "FindNextFileTest.h"
#include "WinSearchIndexerTest.h"
#include "WindowsStorageQueryTest.h"
#include <thread>
#include <chrono>

using namespace winrt;
using namespace Windows::Foundation;
using namespace std::chrono_literals;

int main()
{
    init_apartment();

    // ------> TODO: Insert your test folders here <--------------
    auto searchPaths = std::vector<std::wstring>{
        L"E:/bigFolders/20k", 
        L"E:/bigFolders/1k", 
    };

    for (auto const& searchPath : searchPaths)
    {
        printf("-----------------Executing tests: %ls\n", searchPath.c_str());
        executeFindNextFileTest(searchPath);
        printf("\n");
    }
}

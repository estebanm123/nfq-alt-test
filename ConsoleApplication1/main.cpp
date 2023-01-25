﻿#include "pch.h"

#include "FindNextFileTest.h"
#include "WinSearchIndexerTest.h"
#include "WindowsStorageQueryTest.h"

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();

    // Replace this with a test folder
    auto searchPaths = std::vector<std::wstring>{ L"E:/bigFolders/1k", L"E:/bigFolders/20k", L"E:/bigFolders/100k"};

    for (auto const& searchPath : searchPaths)
    {
        printf("Executing tests: %ws\n", searchPath);

		executeFindNextFileTest(searchPath);
        printf("\n");
		executeWinSearchIndexerTest(searchPath);
        printf("\n");
		executeWinStorageFolderQueryTest(searchPath);
        printf("\n");
    }
}

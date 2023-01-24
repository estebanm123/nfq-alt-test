#include "pch.h"

#include "FindNextFileTest.h"
#include "WinSearchIndexerTest.h"

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();


    // Replace this with a test folder
    std::wstring searchPath = L"E:/bigFolders/20k";

    executeFindNextFileTest(searchPath);
    //executeWinSearchIndexerTest(searchPath);
}

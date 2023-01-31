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

    // Replace this with a test folder
    auto searchPaths = std::vector<std::wstring>{
        L"E:/bigFolders/1k", 
        L"E:/bigFolders/20k",
        L"E:/bigFolders/100k",
        L"C:/Users/emargaron/OneDrive - Microsoft/Pictures",
        L"C:/Users/emargaron/OneDrive - Microsoft/Pictures/small photo",
        //L"C:/Users/emargaron/OneDrive/small photos back up" ,
        //L"C:/Users/emargaron/OneDrive/small photos back up 2" ,
        //L"C:/Users/emargaron/OneDrive/small photos back up 3" ,
        //L"C:/Users/emargaron/OneDrive/small photos varied" ,
        //L"E:/backup/small photo",
        //L"E:/backup/Photo bank",
        //L"E:/backup/Photo bank/small photo"
    };
    //auto searchPaths = std::vector<std::wstring>{ L"C:/Users/emargaron/OneDrive - Microsoft/Pictures" };
    //auto searchPaths = std::vector<std::wstring>{ L"C:/Users/emargaron/OneDrive/small photos back up" };
    //auto searchPaths = std::vector<std::wstring>{ L"C:/Users/emargaron/OneDrive/QueryTest" };
    for (auto const& searchPath : searchPaths)
    {
        printf("-----------------Executing tests: %ls\n", searchPath.c_str());

		executeFindNextFileTest(searchPath);
        printf("\n");

		//executeWinSearchIndexerTest(searchPath);
	  //      printf("\n");
		//executeWinStorageFolderQueryTest(searchPath);
	 //       printf("\n");
    }
}

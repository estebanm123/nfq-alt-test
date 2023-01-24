#include "pch.h"
#include "FindNextFileTest.h"
#include <windows.h>
#include <iostream>
#include "Utils.h"

void executeFindNextFileTest()
{
	// Replace this with a test folder
	LPCWSTR parentPath = L"E:\\bigFolders\\20k\\*";

	WIN32_FIND_DATAW findData;

	auto preFindFirstFileT = Clock::now();
	// TODO: would we ever need to support ANSI alternative? - in/out types are different.

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
		return;
	}

	bool hasNext = false;

	std::wstring dummyFileName = L"h329i0abmlw12!I(O)#";

	auto preloopT = Clock::now();
	std::vector<WIN32_FIND_DATAW> files;
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
				// This test may not be an effective simulation due to branch prediction maybe skipping it
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
	printf("Loop: %lu\n", toMs(doneT - preloopT));
	printf("Loop w/ initial find: %lu\n", toMs(doneT - preFindFirstFileT));
	printf("num files: %zu\n", files.size());
	
	auto preloop2T = Clock::now();
	for (int i = 0; i < files.size(); i++)
	{
		if (dummyFileName == files[i].cFileName)
		{
			printf("This conditional block is just to simulate a check we may have to do");
		}
	}
	auto loop2DoneT = Clock::now();
	printf("Loop 2: %lu\n", toMs(loop2DoneT - preloop2T));

	auto lastError = GetLastError();
	if (lastError == ERROR_NO_MORE_FILES)
	{
		return;
	}
	else
	{
		printf("error: %u\n", lastError);
	}
}

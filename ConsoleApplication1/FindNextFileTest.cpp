#include "pch.h"
#include "FindNextFileTest.h"
#include <windows.h>
#include <chrono>
#include <iostream>
#include <unordered_set>

using Clock = std::chrono::high_resolution_clock;

auto supportedExtensions = std::unordered_set<std::wstring>{
		L".arw", L".cr2", L".crw", L".erf", L".kdc", L".mrw", L".nef", L".nrw", L".orf", L".pef", L".raf", L".raw", L".rw2", L".rwl", L".sr2", L".srw",
		// Raw formats supported by Raw Codec Extension Pack downloadable from the Microsoft Store
		L".ori", L".cr3", L".crw", L".srf", L".dcs", L".dcr", L".drf", L".k25", L".dng", L".3fr", L".ari", L".bay", L".cap", L".iiq", L".eip", L".fff", L".mef", L".mdc", L".mos", L".ptx", L".pxn", L".r3d", L".rwz", L".x3f"
			L".3g2", L".3gp", L".3gp2", L".3gpp", L".asf", L".avi", L".m2t", L".m2ts",
		L".m4v", L".mkv", L".mov", L".mp4", L".mp4v", L".mts", L".wm", L".wmv",
			L".jpg", L".jpeg", L".thumb", L".png", L".tif", L".tiff", L".dng", L".bmp", L".dib", L".gif", L".jfif", L".jpe", L".jxr", L".wdp", L".ico", L".heic", L".heif", L".hif", L".avif"
};

long toMs(std::chrono::nanoseconds t)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

void executeFindNextFileTest()
{
	LPCWSTR parentPath = L"E:\\bigFolders\\20k\\*";

	auto parentPathHstring = winrt::hstring(parentPath);

	WIN32_FIND_DATAW findData;


	auto preFindFirstFileT = Clock::now();
	// TODO: would we ever need to support ANSI alternative? - in/out types are different
	HANDLE handle = FindFirstFile(parentPath, &findData);
	if (handle == INVALID_HANDLE_VALUE || handle == NULL)
	{
		printf("Invalid handle from FindFirstFile\n");
		return;
	}

	bool hasNext = false;

	auto preloopT = Clock::now();
	// dummy vector - in reality store metadata to enable sorting
	std::vector<winrt::hstring> files;
	do
	{
		// TODO: store metadata
		auto name = findData.cFileName;
		auto tmp = std::wstring(name);
		auto pos = tmp.find_last_of(L".");
		auto ext = tmp.substr(pos);
		if (supportedExtensions.find(ext) != supportedExtensions.end())
		{
			files.push_back(name);
		}
		hasNext = FindNextFile(handle, &findData);
	} while (hasNext);
	auto doneT = Clock::now();
	printf("Loop: %lu\n", toMs(doneT - preloopT));
	printf("Loop w/ initial find: %lu\n", toMs(doneT - preFindFirstFileT));
	printf("num files: %u\n", files.size());


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

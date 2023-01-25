#pragma once

#include <chrono>
#include <unordered_set>
#include <oledb.h>

using Clock = std::chrono::high_resolution_clock;

inline const auto supportedExtensions = std::unordered_set<std::wstring>{
		L".arw", L".cr2", L".crw", L".erf", L".kdc", L".mrw", L".nef", L".nrw", L".orf", L".pef", L".raf", L".raw", L".rw2", L".rwl", L".sr2", L".srw",
		// Raw formats supported by Raw Codec Extension Pack downloadable from the Microsoft Store
		L".ori", L".cr3", L".crw", L".srf", L".dcs", L".dcr", L".drf", L".k25", L".dng", L".3fr", L".ari", L".bay", L".cap", L".iiq", L".eip", L".fff", L".mef", L".mdc", L".mos", L".ptx", L".pxn", L".r3d", L".rwz", L".x3f"
			L".3g2", L".3gp", L".3gp2", L".3gpp", L".asf", L".avi", L".m2t", L".m2ts",
		L".m4v", L".mkv", L".mov", L".mp4", L".mp4v", L".mts", L".wm", L".wmv",
			L".jpg", L".jpeg", L".thumb", L".png", L".tif", L".tiff", L".dng", L".bmp", L".dib", L".gif", L".jfif", L".jpe", L".jxr", L".wdp", L".ico", L".heic", L".heif", L".hif", L".avif", L".webp",
};

inline long toMs(std::chrono::nanoseconds t)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

inline unsigned long fileTimeTo100Ns(const FILETIME& ac_FileTime)
{
	ULARGE_INTEGER lv_Large;

	lv_Large.LowPart = ac_FileTime.dwLowDateTime;
	lv_Large.HighPart = ac_FileTime.dwHighDateTime;

	return lv_Large.QuadPart;
}

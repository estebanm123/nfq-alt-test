#pragma once

#include "FileExplorerHelper.g.h"

#include <shlobj.h>

namespace winrt::NfqLib::implementation
{
    struct FileExplorerHelper : FileExplorerHelperT<FileExplorerHelper>
    {
        static Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<SortOrder>> GetSortColumnsAsync(winrt::hstring folderPath);

    private:
        static winrt::com_ptr<IFolderView2> GetFolderView(const std::vector<winrt::com_ptr<IWebBrowserApp>>& webBrowserApps, const winrt::hstring& folderPath);
        static std::vector<winrt::com_ptr<IWebBrowserApp>> FileExplorerHelper::GetWebBrowserAppsOrderedByZOrder();
        static uint32_t GetZOrder(HWND hWnd);
        static HWND GetActiveTab(HWND hwnd);

        winrt::apartment_context m_uiThread;
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct FileExplorerHelper : FileExplorerHelperT<FileExplorerHelper, implementation::FileExplorerHelper>
    {
    };
}

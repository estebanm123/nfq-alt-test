#include "pch.h"
#include "FileExplorerHelper.h"
#include "FileExplorerHelper.g.cpp"

namespace winrt::NfqLib::implementation
{
    Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<SortOrder>> FileExplorerHelper::GetSortColumnsAsync(winrt::hstring folderPath)
    {
        Windows::Foundation::Collections::IVector<SortOrder> sortColumns = winrt::single_threaded_vector<SortOrder>();

        try
        {
            std::vector<winrt::com_ptr<IWebBrowserApp>> webBrowserApps = GetWebBrowserAppsOrderedByZOrder();
            winrt::com_ptr<IFolderView2> folderView2 = GetFolderView(webBrowserApps, folderPath);

            int columnCount;
            folderView2->GetSortColumnCount(&columnCount);
            if (columnCount > 64)
            {
                columnCount = 64;
            }

            SORTCOLUMN columns[64];
            folderView2->GetSortColumns(columns, columnCount);

            for (int i = 0; i < columnCount; i++)
            {
                bool ascending = columns[0].direction > 0 ? true : false;

                PWSTR sortProperty;
                PSGetNameFromPropertyKey(columns[0].propkey, &sortProperty);
                winrt::hstring propertyKey{ sortProperty };

                auto sortOrder = SortOrder(propertyKey, ascending);
                sortColumns.Append(sortOrder);
            }
        }
        catch (winrt::hresult_error const& exception)
        {
            sortColumns.Clear();
        }

        co_return sortColumns;
    }

    winrt::com_ptr<IFolderView2> FileExplorerHelper::GetFolderView(const std::vector<winrt::com_ptr<IWebBrowserApp>>& webBrowserApps, const winrt::hstring& folderPath)
    {
        winrt::com_ptr<IFolderView2> folderView2;

        for (auto& webBrowserApp : webBrowserApps)
        {
            HWND rootHwnd;
            winrt::check_hresult(webBrowserApp->get_HWND((LONG_PTR*)&rootHwnd));
            winrt::com_ptr<IServiceProvider> serviceProvider;
            winrt::check_hresult(webBrowserApp->QueryInterface(IID_IServiceProvider, serviceProvider.put_void()));
            winrt::com_ptr<IShellBrowser> shellBrowser;
            winrt::check_hresult(serviceProvider->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, shellBrowser.put_void()));
            HWND shellBrowserHandle;

            winrt::check_hresult(shellBrowser->GetWindow(&shellBrowserHandle));
            auto activeTab = GetActiveTab(rootHwnd);
            if (activeTab == shellBrowserHandle)
            {
                winrt::com_ptr<IShellView> shellView;
                winrt::check_hresult(shellBrowser->QueryActiveShellView(shellView.put()));
                winrt::com_ptr<IFolderView> folderView;
                winrt::check_hresult(shellView->QueryInterface(IID_IFolderView, folderView.put_void()));
                winrt::com_ptr<IPersistFolder2> persistFolder;
                winrt::check_hresult(folderView->GetFolder(IID_IPersistFolder2, persistFolder.put_void()));
                LPITEMIDLIST pidlFolder;
                winrt::check_hresult(persistFolder->GetCurFolder(&pidlFolder));
                TCHAR g_szPath[MAX_PATH];
                g_szPath[0] = TEXT('\0');
                if (!SHGetPathFromIDList(pidlFolder, g_szPath))
                {
                    lstrcpyn(g_szPath, TEXT("<not a directory>"), MAX_PATH);
                }
                winrt::hstring fileExplorerFolderPath = g_szPath;

                if (folderPath == fileExplorerFolderPath)
                {
                    winrt::check_hresult(shellView->QueryInterface(IID_IFolderView2, folderView2.put_void()));
                    return folderView2;
                }
            }
        }

        return folderView2;
    }

    std::vector<winrt::com_ptr<IWebBrowserApp>> FileExplorerHelper::GetWebBrowserAppsOrderedByZOrder()
    {
        VARIANT v;
        V_VT(&v) = VT_I4;
        winrt::com_ptr<IDispatch> dispatch;
        BOOL fFound = FALSE;
        std::vector<std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>> webBrowserAppZOrder;
        winrt::com_ptr<IShellWindows> shellWindows;
        winrt::check_hresult(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_PPV_ARGS(&shellWindows)));
        for (V_I4(&v) = 0; !fFound && shellWindows->Item(v, dispatch.put()) == S_OK; V_I4(&v)++)
        {
            winrt::com_ptr<IDispatch> disp;
            shellWindows->Item(v, disp.put());
            winrt::com_ptr<IWebBrowserApp> webBrowserApp;
            winrt::check_hresult(disp->QueryInterface(IID_IWebBrowserApp, webBrowserApp.put_void()));
            HWND hwnd;
            webBrowserApp->get_HWND((LONG_PTR*)&hwnd);
            uint32_t zOrder = GetZOrder(hwnd);
            webBrowserAppZOrder.push_back({ webBrowserApp, zOrder });
        }

        std::sort(webBrowserAppZOrder.begin(), webBrowserAppZOrder.end(),
            [](const std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>& a, const std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>& b) -> bool
            {
                return a.second < b.second;
            });

        std::vector<winrt::com_ptr<IWebBrowserApp>> webBrowserAppsOrdered(webBrowserAppZOrder.size());
        std::transform(webBrowserAppZOrder.begin(), webBrowserAppZOrder.end(), webBrowserAppsOrdered.begin(), 
            [](std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t> pair) { return pair.first; });

        return webBrowserAppsOrdered;
    }

    uint32_t FileExplorerHelper::GetZOrder(HWND hWnd)
    {
        uint32_t zOrder = 0;
        for (HWND h = hWnd; h != NULL; h = GetWindow(h, GW_HWNDPREV))
        {
            zOrder++;
        }

        return zOrder;
    }

    HWND FileExplorerHelper::GetActiveTab(HWND hwnd)
    {
        auto activeTab = FindWindowEx(hwnd, NULL, L"ShellTabWindowClass", NULL);
        
        if (activeTab == NULL)
        {
            activeTab = FindWindowEx(hwnd, NULL, L"TabWindowClass", NULL);
        }

        return activeTab;
    }

}

#include "pch.h"
#include "FileExplorerHelper.h"
#include "FileExplorerHelper.g.cpp"
#include <Windows.h>
#include <shlobj.h>
#include <exdisp.h>


namespace winrt::NfqLib::implementation
{
    Windows::Foundation::Collections::IVector<winrt::hstring> FileExplorerHelper::GetSelectedFiles(HWND foregroundWindowHandle)
    {
        HWND activeTab = GetActiveTab(foregroundWindowHandle);

        winrt::com_ptr<IShellWindows> shellWindows = winrt::make_self<ShellWindows>();
        VARIANT v;
        V_VT(&v) = VT_I4;
        IDispatch* pdisp;
        BOOL fFound = FALSE;
        for (V_I4(&v) = 0; !fFound && shellWindows->Item(v, &pdisp) == S_OK; V_I4(&v)++) {
            IDispatch* disp;
            shellWindows->Item(v, &disp);
            IWebBrowserApp* webBrowserApp;
            disp->QueryInterface(IID_IWebBrowserApp, (void**)&webBrowserApp);
            HWND hwnd;
            webBrowserApp->get_HWND((LONG_PTR*)&hwnd);
            if (hwnd == foregroundWindowHandle) {
                IServiceProvider* serviceProvider;
                webBrowserApp->QueryInterface(IID_IServiceProvider, (void**)&serviceProvider);
                IShellBrowser* shellBrowser;
                serviceProvider->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, (void**)&shellBrowser);
                HWND shellBrowserHandle;
                shellBrowser->GetWindow(&shellBrowserHandle);
                if (activeTab == shellBrowserHandle) {
                    IShellFolderViewDual2* shellFolderView;
                    disp->QueryInterface(IID_IShellFolderViewDual2, (void**)&shellFolderView);
                    FolderItems** folderItems;
                    shellFolderView->SelectedItems(folderItems);
                }
            }
        }
        return winrt::single_threaded_vector<winrt::hstring>();
    }
}

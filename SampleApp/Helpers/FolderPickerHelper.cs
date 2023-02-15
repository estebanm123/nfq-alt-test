using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.UI.Xaml;
using Windows.Storage.Pickers;
using Windows.Storage;
using WinRT.Interop;

namespace SampleApp.Helpers;
public static class FolderPickerHelper
{
    public static async Task<StorageFolder> PickFolderAsync()
    {
        var window = new Window();
        var hwnd = WindowNative.GetWindowHandle(window);
        var folderPicker = new FolderPicker();
        folderPicker.FileTypeFilter.Add("*");
        InitializeWithWindow.Initialize(folderPicker, hwnd);
        return await folderPicker.PickSingleFolderAsync();
    }
}

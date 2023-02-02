// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using CommunityToolkit.Mvvm.ComponentModel;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using NfqLib;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.Storage.Pickers;
using WinRT.Interop;

#nullable enable

namespace NfqApp
{
    [INotifyPropertyChanged]
    public sealed partial class MainWindow : Window
    {
        [ObservableProperty]
        IStorageItem? activatedItem;

        [ObservableProperty]
        IList<FileExplorerSort> fileExplorerSortOrder = new List<FileExplorerSort>();

        [ObservableProperty]
        TimeSpan timeToRetrieveSort;

        public MainWindow()
        {
            InitializeComponent();
        }

        public static Visibility HasSortOrder(IList<FileExplorerSort> fileExplorerSortOrder) => fileExplorerSortOrder.Count > 0 ? Visibility.Visible : Visibility.Collapsed;

        public static string AscendingToString(bool isAscending) => isAscending ? "Ascending" : "Descending";

        public static string TimeToRetrieveSortToString(TimeSpan time) => $"Time to retrieve sort order : {time.TotalMilliseconds} (ms)";

        public async Task InitializeAsync(IStorageItem? item)
        {
            ActivatedItem = item;

            if (ActivatedItem is IStorageItem2 storageItem2)
            {
                var parentFolder = await storageItem2.GetParentAsync();
                DisplaySortColumns(parentFolder);
            }


            var benchmark = new StorageFileQueryBenchmark();
            var benchmarkResults = await benchmark.Execute("E:/bigFolders/1k");
            foreach (var result in benchmarkResults)
            {
                Trace.WriteLine("!~: " + result.testName + ": " + result.duration.TotalMilliseconds);
            }
        }

        private async void OpenFolderButton_Click(object sender, RoutedEventArgs e)
        {
            StorageFolder folder = await PickFolderAsync();
            if (folder != null)
            {
                DisplaySortColumns(folder);
            }
        }

        private void DisplaySortColumns(StorageFolder? folder)
        {
            if (folder != null)
            {
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                FileExplorerSortOrder = FileExplorerHelper.GetSortColumns(folder.Path);
                stopwatch.Stop();
                TimeToRetrieveSort = stopwatch.Elapsed;
            }
        }

        private async Task<StorageFolder> PickFolderAsync()
        {
            var window = new Window();
            var hwnd = WindowNative.GetWindowHandle(window);
            var folderPicker = new FolderPicker();
            folderPicker.FileTypeFilter.Add("*");
            InitializeWithWindow.Initialize(folderPicker, hwnd);
            return await folderPicker.PickSingleFolderAsync();
        }
    }
}

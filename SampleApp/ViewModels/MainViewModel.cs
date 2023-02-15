using System.Diagnostics;
using System.Linq;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.UI.Xaml;
using NfqLib;
using SampleApp.Contracts.ViewModels;
using SampleApp.Helpers;
using SampleApp.Services;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Storage;

namespace SampleApp.ViewModels;

public partial class MainViewModel : ObservableRecipient, INavigationAware
{
    [NotifyCanExecuteChangedFor(nameof(QueryFilesAsyncCommand))]
    [ObservableProperty]
    private StorageFolder? folder;

    [ObservableProperty]
    private IList<Win32File> files = new List<Win32File>();

    [ObservableProperty]
    private IList<SortOrder> fileExplorerSortOrder = new List<SortOrder>();

    [ObservableProperty]
    private TimeSpan timeToRetrieveSort;

    public static Visibility HasSortOrder(IList<SortOrder> fileExplorerSortOrder) => fileExplorerSortOrder.Count > 0 ? Visibility.Visible : Visibility.Collapsed;

    public static string AscendingToString(bool isAscending) => isAscending ? "Ascending" : "Descending";

    public static string TimeToRetrieveSortToString(TimeSpan time) => $"Time to retrieve sort order : {time.TotalMilliseconds} (ms)";

    public string GetFolderName(StorageFolder? folder) => folder?.Name ?? string.Empty;

    public string GetFilesTitleText(IList<Win32File> files) => $"Showing {GetFilesToShowCount(files)} of {files.Count} files queried.";

    public IList<Win32File> GetFilesToShow(IList<Win32File> files) => files.Take(GetFilesToShowCount(files)).ToList();

    public int GetFilesToShowCount(IList<Win32File> files) => files.Count > 1000 ? 1000 : files.Count;

    public LoggerService Logger { get; }

    public MainViewModel(LoggerService loggerService)
    {
        Logger = loggerService;
    }

    public void OnNavigatedFrom() => Debug.WriteLine("Navigated from MainViewModel.");

    public async void OnNavigatedTo(object parameter)
    {
        var instanceArgs = AppInstance.GetActivatedEventArgs();
        if (instanceArgs.Kind == ActivationKind.File)
        {
            if (instanceArgs is IFileActivatedEventArgs fileActivatedEventArgs)
            {
                var activatedItem = fileActivatedEventArgs.Files.FirstOrDefault();
                if (activatedItem is StorageFile storageFile)
                {
                    Folder = await storageFile.GetParentAsync();
                }
            }
        }
    }

    [RelayCommand]
    private async void OpenFolderAsync()
    {
        Folder = await FolderPickerHelper.PickFolderAsync();
    }

    [RelayCommand(CanExecute = nameof(CanQueryFiles))]
    private async void QueryFilesAsync()
    {
        await QueryFiles();
    }

    private bool CanQueryFiles()
    {
        return Folder != null;
    }

    private async Task QueryFiles()
    {
        if (Folder == null)
        {
            return;
        }

        Logger.StartLogFileEntry(Folder.Path, "NfqPerfTestLogs");

        var (sortOrderList, feSortOrderElapsedTime) = await Logger.StopwatchLog(
            nameof(FileExplorerHelper.GetSortColumnsAsync), 
            FileExplorerHelper.GetSortColumnsAsync(Folder.Path).AsTask());

        var queryOptions = new Win32FileSystemQueryOptions(
            folderPath: Folder.Path,
            sort: sortOrderList.FirstOrDefault(),
            fileTypeFilter: NfqHelper.SupportedExtensions,
            isRecursive: false);

        var query = new Win32FileSystemQuery(queryOptions);
        var (_, enumeratingElapsed) = await Logger.StopwatchLog(
            nameof(query.EnumerateAsync),
            query.EnumerateAsync().AsTask());

        var (files, sortingElapsed) = await Logger.StopwatchLog(
            nameof(query.SortAsync),
            query.SortAsync(queryOptions.Sort).AsTask());

        Files = files;

        Logger.Log($"Total Elapsed Time {feSortOrderElapsedTime + enumeratingElapsed + sortingElapsed}ms.");
        Logger.CompleteLogFileEntry();
    }
}

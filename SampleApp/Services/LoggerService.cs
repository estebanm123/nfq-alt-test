using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Text;
using CommunityToolkit.Mvvm.ComponentModel;

namespace SampleApp.Services;

public partial class LoggerService : ObservableObject
{
    [ObservableProperty]
    private ObservableCollection<string> logs = new();

    private Stopwatch Stopwatch { get; }

    private string? LogFilePath { get; set; }

    private StringBuilder LogBuilder { get; set; }

    public LoggerService()
    {
        Stopwatch = new Stopwatch();
        LogBuilder = new StringBuilder();
    }

    public void StartLogFileEntry(string folderPath, string logFileName)
    {
        LogFilePath = $"{folderPath}\\{logFileName}.txt";
    }

    public void CompleteLogFileEntry()
    {
        if (string.IsNullOrEmpty(LogFilePath))
        {
            return;
        }

        try
        {
            using var streamWriter = File.AppendText(LogFilePath);
            streamWriter.WriteLine($"\r\nLog Entry");
            streamWriter.WriteLine($"{DateTime.Now.ToLongTimeString()} {DateTime.Now.ToLongDateString()}");
            streamWriter.WriteLine($"{LogBuilder}");
            streamWriter.WriteLine("----------------------------------------------------");
        }
        catch (Exception)
        {
        }
    }

    public void Log(string logMessage)
    {
        Logs.Insert(0, $"{DateTime.Now.ToLongTimeString()} - {logMessage}");
        LogBuilder.AppendLine($" - {logMessage}");
    }

    public async Task<StopwatchLogReturn<T>> StopwatchLog<T>(string taskName, Task<T> taskToWatch)
    {
        Stopwatch.Reset();
        Stopwatch.Start();
        var returnedObject = await taskToWatch;
        Stopwatch.Stop();

        Log($"Execution time for {taskName}: {Stopwatch.ElapsedMilliseconds}ms.");

        return new StopwatchLogReturn<T>(returnedObject, Stopwatch.ElapsedMilliseconds);
    }

    public record StopwatchLogReturn<T>(T ReturnedObject, long ElapsedTime);
}

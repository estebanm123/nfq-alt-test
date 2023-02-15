using Microsoft.UI.Xaml.Controls;
using SampleApp.ViewModels;

namespace SampleApp.Views;

public sealed partial class MainPage : Page
{
    public MainViewModel ViewModel { get; }

    public MainPage()
    {
        ViewModel = App.GetService<MainViewModel>();
        InitializeComponent();
    }
}

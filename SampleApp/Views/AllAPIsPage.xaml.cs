using Microsoft.UI.Xaml.Controls;

using SampleApp.ViewModels;

namespace SampleApp.Views;

public sealed partial class AllAPIsPage : Page
{
    public AllAPIsViewModel ViewModel
    {
        get;
    }

    public AllAPIsPage()
    {
        ViewModel = App.GetService<AllAPIsViewModel>();
        InitializeComponent();
    }
}

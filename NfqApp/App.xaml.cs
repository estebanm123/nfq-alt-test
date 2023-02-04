// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using System.Linq;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Storage;

#nullable enable

namespace NfqApp
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
        }

        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            var instanceArgs = AppInstance.GetActivatedEventArgs();
            IStorageItem? item = null;
            if (instanceArgs.Kind == ActivationKind.File)
            {
                if (instanceArgs is IFileActivatedEventArgs fileActivatedEventArgs)
                {
                    item = fileActivatedEventArgs.Files.FirstOrDefault();
                }
            }

            m_window = new MainWindow();
            _ = m_window.InitializeAsync(item);
            m_window.Activate();
        }

        private MainWindow? m_window;
    }
}

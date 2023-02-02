#pragma once

#include "StorageFileQueryBenchmark.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace WF = winrt::Windows::Foundation;
namespace WFC = WF::Collections;

namespace winrt::NfqLib::implementation
{
    struct StorageFileQueryBenchmark : StorageFileQueryBenchmarkT<StorageFileQueryBenchmark>
    {
        StorageFileQueryBenchmark() = default;

        WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> Execute(winrt::hstring queryFolderPath); 
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct StorageFileQueryBenchmark : StorageFileQueryBenchmarkT<StorageFileQueryBenchmark, implementation::StorageFileQueryBenchmark>
    {
    };
}

#pragma once

#include "FindNextFileBenchmark.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace WF = winrt::Windows::Foundation;
namespace WFC = WF::Collections;

namespace winrt::NfqLib::implementation
{
    struct FindNextFileBenchmark : FindNextFileBenchmarkT<FindNextFileBenchmark>
    {
        FindNextFileBenchmark() = default;

        WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> Execute(winrt::hstring queryFolderPath);
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct FindNextFileBenchmark : FindNextFileBenchmarkT<FindNextFileBenchmark, implementation::FindNextFileBenchmark>
    {
    };
}

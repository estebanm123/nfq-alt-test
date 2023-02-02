#pragma once

#include "OleDbQueryBenchmark.g.h"

namespace WF = winrt::Windows::Foundation;
namespace WFC = WF::Collections;

namespace winrt::NfqLib::implementation
{
    struct OleDbQueryBenchmark : OleDbQueryBenchmarkT<OleDbQueryBenchmark>
    {
        OleDbQueryBenchmark() = default;

        WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> Execute(winrt::hstring queryFolderPath);
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct OleDbQueryBenchmark : OleDbQueryBenchmarkT<OleDbQueryBenchmark, implementation::OleDbQueryBenchmark>
    {
    };
}

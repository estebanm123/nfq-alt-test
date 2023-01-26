#pragma once

#include "FileExplorerHelper.g.h"

namespace winrt::NfqLib::implementation
{
    struct FileExplorerHelper : FileExplorerHelperT<FileExplorerHelper>
    {
        static Windows::Foundation::Collections::IVector<winrt::hstring> FileExplorerHelper::GetSelectedFiles();
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct FileExplorerHelper : FileExplorerHelperT<FileExplorerHelper, implementation::FileExplorerHelper>
    {
    };
}

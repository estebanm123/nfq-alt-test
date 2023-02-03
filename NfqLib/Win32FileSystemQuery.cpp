#include "pch.h"
#include "Win32FileSystemQuery.h"
#if __has_include("Win32FileSystemQuery.g.cpp")
#include "Win32FileSystemQuery.g.cpp"
#endif

namespace winrt::NfqLib::implementation
{
    Win32FileSystemQuery::Win32FileSystemQuery(const Win32FileSystemQueryOptions& queryOptions): m_queryOptions(queryOptions)
    {
    }

    Win32FileSystemQueryOptions Win32FileSystemQuery::QueryOptions()
    {
        return m_queryOptions;
    }

    Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVector<winrt::hstring>> Win32FileSystemQuery::EnumerateAsync()
    {
        co_return single_threaded_vector<winrt::hstring>();
    }
}

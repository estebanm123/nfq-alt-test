#pragma once

#include "SortOrder.g.h"

namespace winrt::NfqLib::implementation
{
    struct SortOrder : SortOrderT<SortOrder>
    {
        SortOrder(const winrt::hstring& propertyKey, bool ascending);

        winrt::hstring PropertyKey();
        void PropertyKey(const winrt::hstring& propertyKey);

        bool Ascending();
        void Ascending(bool ascending);

    private:
        winrt::hstring m_propertyKey;
        bool m_ascending{ true };
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct SortOrder : SortOrderT<SortOrder, implementation::SortOrder>
    {
    };
}

#pragma once

#include "SortOrder.g.h"

namespace winrt::NfqLib::implementation
{
    struct SortOrder : SortOrderT<SortOrder>
    {
        SortOrder(const winrt::hstring& propertyKey, bool isAscending);

        winrt::hstring PropertyKey();
        SortProperty Property();
        SortDirection Direction();
        bool IsAscending();

    private:
        winrt::hstring m_propertyKey;
        SortProperty m_property;
        SortDirection m_direction;
        bool m_isAscending;
    };
}

namespace winrt::NfqLib::factory_implementation
{
    struct SortOrder : SortOrderT<SortOrder, implementation::SortOrder>
    {
    };
}

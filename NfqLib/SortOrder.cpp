#include "pch.h"
#include "SortOrder.h"
#if __has_include("SortOrder.g.cpp")
#include "SortOrder.g.cpp"
#endif

namespace winrt::NfqLib::implementation
{
    static const std::map<winrt::hstring, SortProperty> s_propertyKeyToSortProperty = 
    {
        { L"System.ItemNameDisplay", SortProperty::Name },
        { L"System.ItemDate", SortProperty::Date },
        { L"System.DateCreated", SortProperty::DateCreated },
        { L"System.DateModified", SortProperty::DateModified },
        { L"System.ItemDate Or System.Photo.DateTaken", SortProperty::DateTaken },
        { L"System.ItemTypeText", SortProperty::Type },
        { L"System.Size", SortProperty::Size },
        { L"System.Keywords", SortProperty::Tags },
        { L"System.Image.Dimensions", SortProperty::Dimensions },
        { L"System.Rating", SortProperty::Rating }
    };

    SortOrder::SortOrder(const winrt::hstring& propertyKey, bool isAscending)
        : m_propertyKey(propertyKey), m_isAscending(isAscending)
    {
        m_property = s_propertyKeyToSortProperty.at(propertyKey);

        m_direction = isAscending ? SortDirection::Ascending : SortDirection::Descending;
    }

    winrt::hstring SortOrder::PropertyKey()
    {
        return m_propertyKey;
    }

    SortProperty SortOrder::Property()
    {
        return m_property;
    }

    SortDirection SortOrder::Direction()
    {
        return m_direction;
    }

    bool SortOrder::IsAscending()
    {
        return m_isAscending;
    }
}

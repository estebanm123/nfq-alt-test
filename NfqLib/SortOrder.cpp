#include "pch.h"
#include "SortOrder.h"
#if __has_include("SortOrder.g.cpp")
#include "SortOrder.g.cpp"
#endif

namespace winrt::NfqLib::implementation
{
    SortOrder::SortOrder(const winrt::hstring& propertyKey, bool ascending):
        m_propertyKey(propertyKey), m_ascending(ascending)
    {
    }

    winrt::hstring SortOrder::PropertyKey()
    {
        return m_propertyKey;
    }

    void SortOrder::PropertyKey(const winrt::hstring& propertyKey)
    {
        m_propertyKey = propertyKey;
    }

    bool SortOrder::Ascending()
    {
        return m_ascending;
    }

    void SortOrder::Ascending(bool ascending)
    {
        m_ascending = ascending;
    }
}

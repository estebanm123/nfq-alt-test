#include "WindowsSearchQueryTest.h"

#include <string>
#include <vector>

#include <oledb.h>
#include <SearchAPI.h>
#include <PropVarUtil.h>

const PCWSTR c_select = L"SELECT";
const PCWSTR c_fromIndex = L"FROM SystemIndex WHERE";
const PCWSTR c_scopeFileConditions = L" DIRECTORY='file://E:/bigfolders/20k'";
const PCWSTR c_scopeEmailConditions = L" OR SCOPE='mapi:' OR SCOPE='mapi16:'";
const PCWSTR c_orderConditions = L" ORDER BY System.Search.Rank, System.DateModified, System.ItemNameDisplay DESC";
std::wstring m_usersScope;
std::vector<std::wstring> m_properties{ L"System.ItemUrl", L"System.ItemNameDisplay", L"path", L"System.Search.EntryID",
    L"System.Kind", L"System.KindText", L"System.Search.GatherTime", L"System.Search.QueryPropertyHits" };
std::wstring m_scopeStr; // can be overriden to provide a custom scope

void windowsSearchQueryTest()
{

}

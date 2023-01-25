#include "pch.h"

#include "WinSearchIndexerTest.h"
#include <windows.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>
#include <processenv.h>
#include "Utils.h"
#include <oledb.h>
#include <NTQuery.h>
#include <intsafe.h>
#include "shellapi.h"
#include <wil\resource.h>
#include <wil\result.h>
#include <propsys.h>

const PCWSTR c_select = L"SELECT";
const PCWSTR c_fromIndex = L"FROM SystemIndex WHERE";
const PCWSTR c_scopeFileConditions = L" DIRECTORY='file:";
// TODO: add conditional section to filter based on file type
const PCWSTR c_orderConditions = L" ORDER BY System.ItemNameDisplay DESC";
std::vector<std::wstring> m_properties{ L"System.ItemNameDisplay", L"path", L"System.Kind", L"System.Size", L"System.DateModified", L"System.DateCreated"};

__declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

std::wstring generateSingleUserScope()
{
    wchar_t myProfileDir[MAX_PATH]{};
    GetEnvironmentVariable(L"%USERPROFILE%", myProfileDir, ARRAYSIZE(myProfileDir));
    std::wstring myProfileDirStr(myProfileDir);

    // find the trailing slash
//        _tracelog(L"CurrentUserProfilePath: %s\n", userProfilesDir.c_str());

        // Get all the users, and filter out the one that is not us
    std::wstring scopeStr;
    auto users = winrt::Windows::System::User::FindAllAsync().get();
    for (const auto& user : users)
    {
        std::wstring foundUserProfileDir(winrt::Windows::Storage::UserDataPaths::GetForUser(user).Profile());
        scopeStr += L" AND SCOPE <> ";
        scopeStr += foundUserProfileDir.c_str();
    }

    return scopeStr;
}

std::wstring generateProperties()
{
    std::wstring propertyStr;
    for (auto prop : m_properties)
    {
        propertyStr += L' ';
        propertyStr += prop.c_str();
        propertyStr += L',';
    }
    // remove the last comma
    propertyStr.pop_back();

    // add a space
    propertyStr += L' ';

    return propertyStr;
}

std::wstring generateSelectQueryWithScope(std::wstring const& searchPath)
{
    std::wstring queryStr(c_select);
    queryStr += generateProperties();
    queryStr += c_fromIndex;
    queryStr += L' ';
    queryStr += L"(";

    queryStr += c_scopeFileConditions;
    queryStr +=  searchPath + L"'";

	//queryStr += generateSingleUserScope();

    queryStr += L")";
    return queryStr;
}

void getCommandText(winrt::com_ptr<ICommandText>& cmdText)
{
    // Query CommandText
    winrt::com_ptr<IDBInitialize> dataSource;
    THROW_IF_FAILED(CoCreateInstance(CLSID_CollatorDataSource, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dataSource.put())));
    THROW_IF_FAILED(dataSource->Initialize());

    winrt::com_ptr<IDBCreateSession> session = dataSource.as<IDBCreateSession>();
    winrt::com_ptr<IUnknown> unkSessionPtr;
    THROW_IF_FAILED(session->CreateSession(0, IID_IDBCreateCommand, unkSessionPtr.put()));

    winrt::com_ptr<IDBCreateCommand> createCommand = unkSessionPtr.as<IDBCreateCommand>();
    winrt::com_ptr<IUnknown> unkCmdPtr;
    THROW_IF_FAILED(createCommand->CreateCommand(0, IID_ICommandText, unkCmdPtr.put()));

    cmdText = unkCmdPtr.as<ICommandText>();
}
std::wstring generateQuery(std::wstring const& searchPath)
{
    auto queryStr = generateSelectQueryWithScope(searchPath);

    // optimization for future queries
    // queryStr += L" AND ReuseWhere(";
    // queryStr += std::to_wstring(whereId);
    // queryStr += L")";

    queryStr += c_orderConditions;
    return queryStr;
}

void executeWinSearchIndexerTest(std::wstring const& searchPath)
{
    auto query = generateQuery(searchPath);
	//printf("Query: %ls \n", query.c_str());

    auto preQueryExecutionT = Clock::now();
    winrt::com_ptr<ICommandText> cmdTxt;
    getCommandText(cmdTxt);
    THROW_IF_FAILED(cmdTxt->SetCommandText(DBGUID_DEFAULT, query.c_str()));

    DBROWCOUNT rowCount = 0;
    winrt::com_ptr<IUnknown> unkRowsetPtr;
    THROW_IF_FAILED(cmdTxt->Execute(nullptr, IID_IRowset, nullptr, &rowCount, unkRowsetPtr.put()));
    auto postQueryExecutionT = Clock::now();

    auto resultRows = unkRowsetPtr.as<IRowset>();
    winrt::com_ptr<IGetRow> getRow = resultRows.as<IGetRow>();

    // Directory enumeration
    DBCOUNTITEM rowCountReturned;
    ULONGLONG fetched = 0;

    auto preFetchT = Clock::now();
    do
    {
		HROW rowBuffer[5000] = {}; // Request enough large batch to increase efficiency
        HROW* rowReturned = rowBuffer;

        THROW_IF_FAILED(resultRows->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned));
        THROW_IF_FAILED(ULongLongAdd(fetched, rowCountReturned, &fetched));

        for (unsigned int i = 0; (i < rowCountReturned); i++)
        {
            winrt::com_ptr<IUnknown> unknown;
            THROW_IF_FAILED(getRow->GetRowFromHROW(nullptr, rowBuffer[i], IID_IPropertyStore, unknown.put()));
            winrt::com_ptr<IPropertyStore> propStore = unknown.as<IPropertyStore>();
            // TODO: do something with props
        }

        THROW_IF_FAILED(resultRows->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr));
    } while (rowCountReturned > 0);

    auto postFetchT = Clock::now();

    printf("Fetched %llu entries\n", fetched);
    printf("Query execution time: %lu\n", toMs(postQueryExecutionT - preQueryExecutionT));
    printf("Row enumeration time: %lu \n", toMs(postFetchT - preFetchT));
}

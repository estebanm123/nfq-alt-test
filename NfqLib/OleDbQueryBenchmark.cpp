#include "pch.h"
#include "OleDbQueryBenchmark.h"
#if __has_include("OleDbQueryBenchmark.g.cpp")
#include "OleDbQueryBenchmark.g.cpp"
#endif

#include <windows.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>
#include <processenv.h>
#include <string>
#include <oledb.h>
#include <NTQuery.h>
#include <intsafe.h>
#include "shellapi.h"
#include <wil\resource.h>
#include <wil\result.h>
#include <propsys.h>

#include "QueryBenchmarkUtils.h"

namespace winrt::NfqLib::implementation
{

    struct QueryResult
    {
        winrt::com_ptr<IGetRow> getRow; 
        winrt::impl::com_ref<IRowset> resultRows;
    };

    const PCWSTR c_select = L"SELECT";
    const PCWSTR c_fromIndex = L"FROM SystemIndex WHERE";
    const PCWSTR c_scopeFileConditions = L" DIRECTORY='file:";
    // TODO: add conditional section to filter based on file type
    const PCWSTR c_orderConditions = L" ORDER BY System.ItemNameDisplay DESC";
    std::vector<std::wstring> m_properties{ L"System.ItemNameDisplay", L"path", L"System.Kind", L"System.Size", L"System.DateModified", L"System.DateCreated" };

    __declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

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
        queryStr += searchPath + L"'";

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

        queryStr += c_orderConditions;
        return queryStr;
    }

    void enumerateRows(QueryResult const& queryResult)
    {
        // Directory enumeration
        DBCOUNTITEM rowCountReturned;
        ULONGLONG numResults = 0;

        do
        {
            HROW rowBuffer[5000] = {}; // Request enough large batch to increase efficiency
            HROW* rowReturned = rowBuffer;

            THROW_IF_FAILED(queryResult.resultRows->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned));
            THROW_IF_FAILED(ULongLongAdd(numResults, rowCountReturned, &numResults));

            for (unsigned int i = 0; (i < rowCountReturned); i++)
            {
                winrt::com_ptr<IUnknown> unknown;
                THROW_IF_FAILED(queryResult.getRow->GetRowFromHROW(nullptr, rowBuffer[i], IID_IPropertyStore, unknown.put()));
                winrt::com_ptr<IPropertyStore> propStore = unknown.as<IPropertyStore>();
                // TODO: experiment with props
            }

            THROW_IF_FAILED(queryResult.resultRows->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr));
        } while (rowCountReturned > 0);
    }

    QueryResult executeQuery(std::wstring query)
    {
        winrt::com_ptr<ICommandText> cmdTxt;
        getCommandText(cmdTxt);
        THROW_IF_FAILED(cmdTxt->SetCommandText(DBGUID_DEFAULT, query.c_str()));

        DBROWCOUNT rowCount = 0;
        winrt::com_ptr<IUnknown> unkRowsetPtr;
        THROW_IF_FAILED(cmdTxt->Execute(nullptr, IID_IRowset, nullptr, &rowCount, unkRowsetPtr.put()));

        auto resultRows = unkRowsetPtr.as<IRowset>();
        winrt::com_ptr<IGetRow> getRow = resultRows.as<IGetRow>();
        return { getRow, resultRows };
    }

    WF::IAsyncOperation<WFC::IVector<BenchmarkResult>> OleDbQueryBenchmark::Execute(winrt::hstring queryFolderPath)
    {
		auto strongThis{ get_strong() };
		apartment_context callingThread;
		co_await resume_background();

		WFC::IVector<BenchmarkResult> benchmarkResults = winrt::single_threaded_vector<BenchmarkResult>();

        auto query = generateQuery(queryFolderPath.c_str());

        auto queryResult = RunBenchmark<QueryResult>(L"Query execution",
            [=]() { return executeQuery(query); },
            benchmarkResults
        );

        RunBenchmark(L"Query result enumeration",
            [=]() { enumerateRows(queryResult); },
            benchmarkResults
        );

        // TODO: add tests to find a specific file's index and fetch a neighbors if possible - can maybe try binary searching through rows?

		co_await callingThread;
		co_return benchmarkResults;
	}
}

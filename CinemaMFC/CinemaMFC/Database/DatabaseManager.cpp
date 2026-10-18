#include "pch.h"
#include "../pch.h"         
#include "DatabaseManager.h"
#include <string>
#include <sstream>
const wchar_t* DatabaseManager::SUPABASE_HOST = L"iqtrsrjvhciwxyqwwocp.supabase.co";
const char*    DatabaseManager::ANON_KEY       = "sb_publishable_DV-C2RHzlOqzEJfCUNgdFg_gAxccX6f";
const wchar_t* DatabaseManager::REST_BASE      = L"/rest/v1/";
DatabaseManager::DatabaseManager() {
    m_session = (void*)WinHttpOpen(L"NexoraCinema/1.0",
                                   WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                   WINHTTP_NO_PROXY_NAME,
                                   WINHTTP_NO_PROXY_BYPASS,
                                   0);
}
DatabaseManager::~DatabaseManager() {
    if (m_session) { WinHttpCloseHandle((HINTERNET)m_session); m_session = nullptr; }
}
DatabaseManager& DatabaseManager::GetInstance() {
    static DatabaseManager instance;
    return instance;
}
bool DatabaseManager::Connect() {
    if (!m_session) {
        MessageBoxA(NULL,
            "WinHTTP session failed to initialize.\nThis is required to reach Supabase.",
            "Database Connection Error", MB_OK | MB_ICONERROR);
        return false;
    }
    m_connected = true;
    return true;
}
std::string DatabaseManager::Request(const std::wstring& method,
                                     const std::wstring& path,
                                     const std::string&  body,
                                     const std::wstring& extraHeaders)
{
    if (!m_session) return "";
    std::wstring anonKeyW(ANON_KEY, ANON_KEY + strlen(ANON_KEY));
    std::wstring headers =
        L"apikey: "        + anonKeyW + L"\r\n" +
        L"Authorization: Bearer " + anonKeyW + L"\r\n" +
        L"Content-Type: application/json\r\n"
        L"Accept: application/json\r\n"
        L"Prefer: return=representation\r\n";  
    if (!extraHeaders.empty()) headers += extraHeaders + L"\r\n";
    HINTERNET hConnect = WinHttpConnect((HINTERNET)m_session, SUPABASE_HOST, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) return "";
    HINTERNET hRequest = WinHttpOpenRequest(hConnect,
                                            method.c_str(),
                                            path.c_str(),
                                            NULL,
                                            WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest) { WinHttpCloseHandle(hConnect); return ""; }
    const void* bodyPtr  = body.empty() ? NULL              : (const void*)body.c_str();
    DWORD       bodyLen  = body.empty() ? 0                 : (DWORD)body.size();
    BOOL ok = WinHttpSendRequest(hRequest,
                                 headers.c_str(), (DWORD)-1L,
                                 const_cast<void*>(bodyPtr), bodyLen, bodyLen, 0);
    if (!ok) { WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); return ""; }
    ok = WinHttpReceiveResponse(hRequest, NULL);
    if (!ok) { WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); return ""; }
    DWORD statusCode = 0;
    DWORD statusLen  = sizeof(statusCode);
    WinHttpQueryHeaders(hRequest,
                        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                        WINHTTP_HEADER_NAME_BY_INDEX,
                        &statusCode, &statusLen, WINHTTP_NO_HEADER_INDEX);
    std::string response;
    if (statusCode >= 200 && statusCode < 300) {
        DWORD bytesAvail = 0;
        while (WinHttpQueryDataAvailable(hRequest, &bytesAvail) && bytesAvail > 0) {
            std::string chunk(bytesAvail, '\0');
            DWORD bytesRead = 0;
            WinHttpReadData(hRequest, &chunk[0], bytesAvail, &bytesRead);
            chunk.resize(bytesRead);
            response += chunk;
        }
    }
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    return response;
}
std::string DatabaseManager::HttpGet(const std::string& endpoint, const std::string& query) {
    std::wstring endpointW(endpoint.begin(), endpoint.end());
    std::wstring path = std::wstring(REST_BASE) + endpointW;
    if (!query.empty()) {
        std::wstring queryW(query.begin(), query.end());
        path += L"?" + queryW;
    }
    return Request(L"GET", path, "");
}
std::string DatabaseManager::HttpPost(const std::string& endpoint, const std::string& jsonBody) {
    std::wstring endpointW(endpoint.begin(), endpoint.end());
    std::wstring path = std::wstring(REST_BASE) + endpointW;
    return Request(L"POST", path, jsonBody);
}
std::string DatabaseManager::HttpPatch(const std::string& endpoint,
                                        const std::string& query,
                                        const std::string& jsonBody) {
    std::wstring endpointW(endpoint.begin(), endpoint.end());
    std::wstring path = std::wstring(REST_BASE) + endpointW;
    if (!query.empty()) {
        std::wstring queryW(query.begin(), query.end());
        path += L"?" + queryW;
    }
    return Request(L"PATCH", path, jsonBody);
}
bool DatabaseManager::HttpDelete(const std::string& endpoint, const std::string& query) {
    std::wstring endpointW(endpoint.begin(), endpoint.end());
    std::wstring path = std::wstring(REST_BASE) + endpointW;
    if (!query.empty()) {
        std::wstring queryW(query.begin(), query.end());
        path += L"?" + queryW;
    }
    HINTERNET hConnect = WinHttpConnect((HINTERNET)m_session, SUPABASE_HOST, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) return false;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"DELETE", path.c_str(),
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) { WinHttpCloseHandle(hConnect); return false; }
    std::wstring anonKeyW(ANON_KEY, ANON_KEY + strlen(ANON_KEY));
    std::wstring headers =
        L"apikey: " + anonKeyW + L"\r\n" +
        L"Authorization: Bearer " + anonKeyW + L"\r\n";
    WinHttpSendRequest(hRequest, headers.c_str(), (DWORD)-1L, NULL, 0, 0, 0);
    WinHttpReceiveResponse(hRequest, NULL);
    DWORD statusCode = 0, statusLen = sizeof(statusCode);
    WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                        WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusLen,
                        WINHTTP_NO_HEADER_INDEX);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    return (statusCode >= 200 && statusCode < 300);
}

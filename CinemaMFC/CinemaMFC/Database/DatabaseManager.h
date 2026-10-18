#pragma once
#include <string>
class DatabaseManager {
public:
    static DatabaseManager& GetInstance();
    bool Connect();
    bool IsConnected() const { return m_connected; }
    std::string HttpGet   (const std::string& endpoint, const std::string& query = "");
    std::string HttpPost  (const std::string& endpoint, const std::string& jsonBody);
    std::string HttpPatch (const std::string& endpoint, const std::string& query, const std::string& jsonBody);
    bool        HttpDelete(const std::string& endpoint, const std::string& query);
    bool InitializeSchema() { return true; }
private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    std::string Request(const std::wstring& method,
                        const std::wstring& path,
                        const std::string&  body,
                        const std::wstring& extraHeaders = L"");
    bool m_connected = false;
    void* m_session = nullptr;
    static const wchar_t* SUPABASE_HOST;   
    static const char*    ANON_KEY;        
    static const wchar_t* REST_BASE;       
};

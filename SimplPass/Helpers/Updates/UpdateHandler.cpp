#include "UpdateHandler.h"
#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

#pragma comment(lib, "winhttp.lib")

void checkForUpdates()
{
    std::cout << "checking for updates..." << "\n";
}

std::string getCurrentVersion()
{
    /*std::ifstream configFile("../config.json");  
    if (!configFile.is_open()) {
        std::cout << "[err] failed to open config.json\n";
        return "";
    }

    json config;
    try {
        configFile >> config;  
    } catch (json::parse_error& e) {
        std::cout << "[err] failed to parse config.json: " << e.what() << "\n";
        return "";
    }

    if (config.contains("version") && config["version"].is_string()) {
        std::string version = config["version"];
        return version;  
    } else {
        std::cout << "[err] version key missing or invalid\n";
        return "";
    }
    
    }

    void getVersions()
    {
        HINTERNET hSession = WinHttpOpen(L"A User Agent",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0);
        if (!hSession)
        {
            std::cout << "[err] error: winhttpopen failed: " << GetLastError() << "\n";
        }

        HINTERNET hConnect = WinHttpConnect(hSession, L"example.com", INTERNET_DEFAULT_HTTP_PORT, 0);
        if (!hConnect)
        {
            std::cout << "[err] winhttpconnect failed: " << GetLastError() << "\n";
            WinHttpCloseHandle(hSession);
        }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/raw/f7KKzVyf",
            NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
        if (!hRequest)
        {
            std::cout << "[err] winhttpopenrequest failed: " << GetLastError() << "\n";
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);

        }

        BOOL bResult = WinHttpSendRequest(hRequest, 
                                          WINHTTP_NO_ADDITIONAL_HEADERS, 0, 
                                          WINHTTP_NO_REQUEST_DATA, 0, 
                                          0, 0);
        if (!bResult)
        {
            std::cout << "[err] winhttpsendrequest failed: " << GetLastError() << "\n";
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);

        }

        bResult = WinHttpReceiveResponse(hRequest, NULL);
        if (!bResult)
        {
            std::cout << "[err] winhttpreceiveresponse failed: " << GetLastError() << "\n";
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);

        }

        DWORD dwSize = 0;
        DWORD dwDownloaded = 0;
        LPSTR pszOutBuffer;
        LPSTR pszBuffer;

        do
        {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            {
                std::cout << "[err] winhttpquerydataavailable failed: " << GetLastError() << "\n";
                break;
            }

            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                std::cout << "[err] out of memory\n";
                dwSize = 0;
                break;
            }

            ZeroMemory(pszOutBuffer, dwSize + 1);
            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
            {
                std::cout << "[err] winhttpreaddata failed: " << GetLastError() << "\n";
                delete[] pszOutBuffer;
                break;
            }
            else
            {
                std::cout << "response: " << pszOutBuffer << "\n";
            }

            delete[] pszOutBuffer;
        } while (dwSize > 0);

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);*/
    }





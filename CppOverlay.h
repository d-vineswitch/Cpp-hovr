#ifndef CppOverlay_H
#define CppOverlay_H

// ------------------------------------------------------------------------------ //

/**
 *
 * @file CppOverlay.h
 * @brief A header full of customizable macros that can be used in C++.
 * @author __dvineswxtch
 */

// ----------------------------- CppOverlay_h Begin ------------------------------- //

#include <iostream>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <locale>
#include <ranges>
#include <codecvt>

#if defined(_WIN32)
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <comutil.h>

#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "wbemuuid.lib")

#define OutputHandle GetStdHandle(STD_OUTPUT_HANDLE)
#define InputHandle GetStdHandle(STD_INPUT_HANDLE)
#define GetWindowHandle HANDLE
#endif

#if defined(_WIN32)
#define _usrHomePath _env("USERPROFILE")
#define _winDir _env("WINDIR")
#else
#define _usrHomePath _env("HOME")
#endif

#define __set_to =
#define go continue
#define __find while
#define select bool
#define below <
#define ptr *
#define above >
#define __equiv ==
#define _not !
#define __defcode 0
#define zerocode 0
#define otzero 1
#define undefzero -1
#define isnot !=
#define in for
#define num int
#define newline "\n"
#define str std::string
#define check if
#define other else
#define function void
#define __cppi_version "23.104.22"
#define __returncode return
#define __outstream <<
#define set ->
#define __instream >>
#define __add /
#define _env(_objectstr) std::getenv(_objectstr)
#define __compath(path, subpath) (fs::path(path)) __add(fs::path(subpath))
#define endf std::endl
#define print std::cout
#define no false
#define yes true
#define printerr std::cerr
#define _reduceOverhead inline
#define __currentdirectory getCurrentDirectory()
#define __cptr argc
#define __vptr argv
#define empty NULL
#define emptyPtr nullptr
#define loadptr(argc, argv) num main(int argc, char *argv[])

namespace fs __set_to std::filesystem;

#if defined(USE_USB)
#include <libusb/libusb.h>
#endif

#if defined(SDL_MAIN_HANDLED)
#define WIN_DEFAULT_WIDTH 800
#define WIN_DEFAULT_HEIGHT 600

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

_reduceOverhead select __initSDL2() {
    check (SDL_Init(SDL_INIT_VIDEO) isnot zerocode) {
        print
        __outstream "SDL could not initialize! SDL_Error: " 
        __outstream SDL_GetError()
        __outstream "\n"
        __outstream
        endf;
        __returncode false;
    }
    __returncode true;
} 

_reduceOverhead select __initSDL2Image() {
    check (IMG_Init(IMG_INIT_PNG) __equiv zerocode) {
        printerr
        __outstream
        "IMG_init Error: "
        __outstream
        IMG_GetError()
        __outstream
        endf;
        
        SDL_Quit();
        __returncode(otzero);
    }
}

#endif

#if __cplusplus __equiv 201103L
#define CppStandard "C++11"
#elif __cplusplus __equiv 201402L
#define CppStandard "C++14"
#elif __cplusplus __equiv 201703L
#define CppStandard "C++17"
#elif __cplusplus __equiv 202002L
#define CppStandard "C++20"
#elif __cplusplus __equiv 202302L
#define CppStandard "C++23"
#else
#define CppStandard "C++98/C++03"
#endif

#if defined(_WIN32)
#include <windows.h>
#define __ostype "Win-x32"
#define __osversion GetWindowsVersion()

#elif defined(_WIN64)
#include <windows.h>
#define __ostype "Win-x64"
#define __osversion GetWindowsVersion()

#elif defined(__linux__)
#include <sys/utsname.h>
#define __ostype "Linux"
#define __osversion GetLinuxVersion()

#else
#define __ostype "Unknown"
#define __osversion "Unknown"
#endif

// Compilers
#if defined(__MINGW32__)
#define CppCompiler "MinGW-x32"
#endif

#if defined(__MINGW64__)
#define CppCompiler "MinGW-x64"
#endif

#if defined (__MINGW32__) && defined(__MINGW64__)
#define CppCompiler "MinGW-x64_x86"
#endif

#if defined(_MSVC_VER)
#define CppCompiler "MSVisualStudio"
#endif

#if defined(__INTEL_COMPILER)
#define CppCompiler "IntelCppCompiler"
#endif

#if defined(__clang__)
#define CppCompiler "Clang"
#endif

_reduceOverhead str getCurrentDirectory()
{
    fs::path cwd __set_to fs::current_path();
    std::string cwdStr __set_to cwd.string();
    std::replace(cwdStr.begin(), cwdStr.end(), '\\', '/');
    __returncode cwdStr;
}

_reduceOverhead function GetWindowInputHandle(HANDLE &__inputHandle)
{
    __inputHandle __set_to InputHandle;
}

_reduceOverhead function GetWindowOutputHandle(HANDLE &__outputHandle)
{
    __outputHandle __set_to OutputHandle;
}

_reduceOverhead str wstring_to_string(const std::wstring& wstr) {
    return std::ranges::to<std::string>(wstr);  // Converts std::wstring to std::string
}

#if defined(_WIN32) || defined(_WIN64)
// Windows-specific version retrieval
_reduceOverhead str GetWindowsVersion()
{
    // Initialize COM library
    HRESULT hres __set_to CoInitializeEx(0, COINIT_MULTITHREADED);
    check(FAILED(hres))
    {
        __returncode "Failed to initialize COM library.";
    }

    // Set general COM security levels
    hres __set_to CoInitializeSecurity(
        NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    check(FAILED(hres))
    {
        CoUninitialize();
        __returncode "Failed to initialize security.";
    }

    // Obtain the WMI locator interface
    IWbemLocator *pLoc __set_to NULL;
    hres __set_to CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);
    check(FAILED(hres))
    {
        CoUninitialize();
        __returncode "Failed to create IWbemLocator object.";
    }

    // Connect to the WMI namespace
    IWbemServices *pSvc __set_to nullptr;
    hres __set_to pLoc set ConnectServer(
        SysAllocString(L"ROOT\\CIMV2"), // WMI namespace
        NULL, NULL, 0, 0, 0, 0, &pSvc);
    check(FAILED(hres))
    {
        pLoc set Release();
        CoUninitialize();
        __returncode "Failed to connect to WMI.";
    }

    // Set security levels on the WMI connection
    hres __set_to CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                                    RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    check(FAILED(hres))
    {
        pSvc set Release();
        pLoc set Release();
        CoUninitialize();
        __returncode "Failed to set proxy blanket.";
    }

    // Query WMI for the operating system information
    IEnumWbemClassObject *pEnumerator __set_to NULL;
    hres __set_to pSvc set ExecQuery(
        SysAllocString(L"WQL"),
        SysAllocString(L"SELECT * FROM Win32_OperatingSystem"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);
    check(FAILED(hres))
    {
        pSvc set Release();
        pLoc set Release();
        CoUninitialize();
        __returncode "Query for operating system failed.";
    }

    // Get the results
    IWbemClassObject *pClassObject;
    ULONG uReturn __set_to 0;
    str osInfo __set_to "OS Name: Unknown\nOS Version: Unknown"; // Default return value
    __find(pEnumerator)
    {
        hres __set_to pEnumerator set Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
        check(0 __equiv uReturn)
        {
            break;
        }

        // Retrieve the "Caption" and "Version" properties
        VARIANT vtProp;
        str osName __set_to "Unknown OS Name";
        str osVersion __set_to "Unknown Version";

        hres __set_to pClassObject set Get(L"Caption", 0, &vtProp, 0, 0);
        check(SUCCEEDED(hres))
        {
            // Convert the BSTR (vtProp.bstrVal) to std::wstring and then to std::string
            std::wstring osNameW(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
            osName = wstring_to_string(osNameW); // Convert to std::string
            VariantClear(&vtProp);
        }

        hres __set_to pClassObject set Get(L"Version", 0, &vtProp, 0, 0);
        check(SUCCEEDED(hres))
        {
            // Convert the BSTR (vtProp.bstrVal) to std::wstring and then to std::string
            std::wstring osVersionW(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
            osVersion = wstring_to_string(osVersionW); // Convert to std::string
            VariantClear(&vtProp);
        }

        osInfo __set_to "OS Name: " + osName + "\nOS Version: " + osVersion;

        pClassObject set Release();
    }

    // Clean up
    pEnumerator set Release();
    pSvc set Release();
    pLoc set Release();
    CoUninitialize();

    __returncode osInfo;
}

#endif

#if defined(__linux__)
// Linux-specific version retrieval
_reduceOverhead str GetLinuxVersion()
{
    struct utsname buffer;
    if (uname(&buffer) __equiv 0)
    {
        __returncode std::string(buffer.release); // Release version of Linux
    }
    __returncode "Unknown";
}

#endif

// ---------------------------------------------------------------- //

#endif
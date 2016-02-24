#pragma once

//---------------------------------------------------------------------------//
//
// Settings.hpp
//  設定ファイル 管理クラス
//   Copyright (C) 2016 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

// WinMain.cpp で宣言
extern HINSTANCE g_hInst;

//---------------------------------------------------------------------------//
// Class
//---------------------------------------------------------------------------//

class Settings
{
public:
    bool ComfirmAll;
    bool ComfirmShutdown;
    bool ComfirmReboot;
    bool ComfirmLogOff;
    bool ComfirmLock;
    bool ComfirmSleep;
    bool ComfirmHibernate;

public:
    Settings()  { load(); }
    ~Settings() { save(); }

public:
    void load();
    void save();
};

extern Settings* settings;

//---------------------------------------------------------------------------//
// Methods
//---------------------------------------------------------------------------//

inline void Settings::load()
{
    wchar_t path [MAX_PATH];

    // iniファイル名取得
    const auto len = ::GetModuleFileNameW(g_hInst, path, MAX_PATH);
    path[len - 3] = 'i';
    path[len - 2] = 'n';
    path[len - 1] = 'i';
    WriteLog(elInfo, TEXT("%s: %s"), PLUGIN_NAME, path);

    // パラメータの取得
    ComfirmAll = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmAll", true, path
    )
    ? true : false;

    ComfirmShutdown = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmShutdown", true, path
    )
    ? true : false;

    ComfirmReboot = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmReboot", true, path
    )
    ? true : false;

    ComfirmLogOff = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmLogOff", true, path
    )
    ? true : false;

    ComfirmLock = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmLock", false, path
    )
    ? true : false;

    ComfirmSleep = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmSleep", false, path
    )
    ? true : false;

    ComfirmHibernate = ::GetPrivateProfileIntW
    (
        L"Setting", L"ComfirmHibernate", false, path
    )
    ? true : false;
}

//---------------------------------------------------------------------------//

inline void Settings::save()
{
    wchar_t path [MAX_PATH];
    wchar_t buf  [16];

    // iniファイル名取得
    const auto len = ::GetModuleFileNameW(g_hInst, path, MAX_PATH);
    path[len - 3] = 'i';
    path[len - 2] = 'n';
    path[len - 1] = 'i';

    // パラメータの書き出し
    ::wsprintfW(buf, L"%i", ComfirmAll);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmAll", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmShutdown);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmShutdown", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmReboot);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmReboot", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmLogOff);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmLogOff", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmLock);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmLock", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmSleep);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmSleep", buf, path);

    ::wsprintfW(buf, L"%i", ComfirmHibernate);
    ::WritePrivateProfileStringW(L"Setting", L"ComfirmHibernate", buf, path);
}

//---------------------------------------------------------------------------//

// Settings.hpp
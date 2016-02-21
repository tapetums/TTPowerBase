//---------------------------------------------------------------------------//
//
// Main.cpp
//
//---------------------------------------------------------------------------//

#include <windows.h>
#include <powrprof.h>
#pragma comment(lib, "powrprof.lib")

#include "Plugin.hpp"
#include "MessageDef.hpp"
#include "Utility.hpp"

#include "Main.hpp"

//---------------------------------------------------------------------------//
//
// グローバル変数
//
//---------------------------------------------------------------------------//

HINSTANCE g_hInst { nullptr };

//---------------------------------------------------------------------------//

// コマンドID
enum CMD : INT32
{
    CMD_SHUTDOWN,
    CMD_REBBOT,
    CMD_LOGOFF,
    CMD_SLEEP,
    CMD_HIBERNATE,
    CMD_COUNT
};

//---------------------------------------------------------------------------//

// プラグインの名前
LPCTSTR PLUGIN_NAME { TEXT("TTPowerBase") };

// コマンドの数
DWORD COMMAND_COUNT { CMD_COUNT };

//---------------------------------------------------------------------------//

// コマンドの情報
PLUGIN_COMMAND_INFO g_cmd_info[] =
{
    {
        TEXT("Shutdown"),                    // コマンド名（英名）
        TEXT("シャットダウン"),              // コマンド説明（日本語）
        CMD_SHUTDOWN,                        // コマンドID
        0,                                   // Attr（未使用）
        -1,                                  // ResTd(未使用）
        DISPMENU(dmToolMenu | dmHotKeyMenu), // DispMenu
        0,                                   // TimerInterval[msec] 0で使用しない
        0                                    // TimerCounter（未使用）
    },
    {
        TEXT("Restart"),                     // コマンド名（英名）
        TEXT("再起動"),                      // コマンド説明（日本語）
        CMD_REBBOT,                          // コマンドID
        0,                                   // Attr（未使用）
        -1,                                  // ResTd(未使用）
        DISPMENU(dmToolMenu | dmHotKeyMenu), // DispMenu
        0,                                   // TimerInterval[msec] 0で使用しない
        0                                    // TimerCounter（未使用）
    },
    {
        TEXT("Sign out"),                    // コマンド名（英名）
        TEXT("サインアウト"),                // コマンド説明（日本語）
        CMD_LOGOFF,                          // コマンドID
        0,                                   // Attr（未使用）
        -1,                                  // ResTd(未使用）
        DISPMENU(dmToolMenu | dmHotKeyMenu), // DispMenu
        0,                                   // TimerInterval[msec] 0で使用しない
        0                                    // TimerCounter（未使用）
    },
    {
        TEXT("Sleep"),                       // コマンド名（英名）
        TEXT("スリープ"),                    // コマンド説明（日本語）
        CMD_SLEEP,                           // コマンドID
        0,                                   // Attr（未使用）
        -1,                                  // ResTd(未使用）
        DISPMENU(dmToolMenu | dmHotKeyMenu), // DispMenu
        0,                                   // TimerInterval[msec] 0で使用しない
        0                                    // TimerCounter（未使用）
    },
    {
        TEXT("Hibernate"),                   // コマンド名（英名）
        TEXT("休止状態"),                    // コマンド説明（日本語）
        CMD_HIBERNATE,                       // コマンドID
        0,                                   // Attr（未使用）
        -1,                                  // ResTd(未使用）
        DISPMENU(dmToolMenu | dmHotKeyMenu), // DispMenu
        0,                                   // TimerInterval[msec] 0で使用しない
        0                                    // TimerCounter（未使用）
    },
};

//---------------------------------------------------------------------------//

// プラグインの情報
PLUGIN_INFO g_info =
{
    0,                   // プラグインI/F要求バージョン
    (LPTSTR)PLUGIN_NAME, // プラグインの名前（任意の文字が使用可能）
    nullptr,             // プラグインのファイル名（相対パス）
    ptLoadAtUse,         // プラグインのタイプ
    0,                   // バージョン
    0,                   // バージョン
    COMMAND_COUNT,       // コマンド個数
    &g_cmd_info[0],      // コマンド
    0,                   // ロードにかかった時間（msec）
};

//---------------------------------------------------------------------------//

// PCのシャットダウン権限を有効にする
bool EnableShutDown()
{
     // プロセスのトークンを取得
    HANDLE hToken { nullptr };
    auto ret = ::OpenProcessToken
    (
        ::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken
    );
    if ( ! ret )
    {
        ::CloseHandle(hToken);
        return false;
    }

    // 特権に対応する LUID を取得
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    ret = ::LookupPrivilegeValue
    (
        nullptr, SE_SHUTDOWN_NAME, &(tp.Privileges[0].Luid)
    );
    if ( ! ret )
    {
        ::CloseHandle(hToken);
        return false;
    }

    // 特権を有効にする
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    ::AdjustTokenPrivileges(hToken, FALSE, &tp, 0, nullptr, nullptr);
    ret = ::GetLastError() == ERROR_SUCCESS;
    if ( ! ret )
    {
        ::CloseHandle(hToken);
        return false;
    }

    ::CloseHandle(hToken);
    return true;
}

//---------------------------------------------------------------------------//

// TTBEvent_Init() の内部実装
BOOL WINAPI Init()
{
    return TRUE;
}

//---------------------------------------------------------------------------//

// TTBEvent_Unload() の内部実装
void WINAPI Unload()
{
}

//---------------------------------------------------------------------------//

// TTBEvent_Execute() の内部実装
BOOL WINAPI Execute(INT32 CmdId, HWND hwnd)
{
    ::SetForegroundWindow(hwnd);

    INT_PTR ret;

    switch ( CmdId )
    {
        case CMD_SHUTDOWN:
        {
            ret = ::MessageBox(nullptr, TEXT("シャットダウンしますか？"), PLUGIN_NAME, MB_YESNO);
            if ( ret != IDYES ) { return FALSE; }

            if ( ! EnableShutDown() )
            {
                return FALSE;
            }

            return ::ExitWindowsEx(EWX_SHUTDOWN, 0);
        }
        case CMD_REBBOT:
        {
            ret = ::MessageBox(nullptr, TEXT("再起動しますか？"), PLUGIN_NAME, MB_YESNO);
            if ( ret != IDYES ) { return FALSE; }

            if ( ! EnableShutDown() )
            {
                return FALSE;
            }

            return ::ExitWindowsEx(EWX_REBOOT, 0);
        }
        case CMD_LOGOFF:
        {
            ret = ::MessageBox(nullptr, TEXT("サインアウトしますか？"), PLUGIN_NAME, MB_YESNO);
            if ( ret != IDYES ) { return FALSE; }

            return ::ExitWindowsEx(EWX_LOGOFF, 0);
        }
        case CMD_SLEEP:
        {
            ret = ::MessageBox(nullptr, TEXT("スリープしますか？"), PLUGIN_NAME, MB_YESNO);
            if ( ret != IDYES ) { return FALSE; }

            return ::SetSuspendState(FALSE, FALSE, FALSE);
        }
        case CMD_HIBERNATE:
        {
            ret = ::MessageBox(nullptr, TEXT("休止状態にしますか？"), PLUGIN_NAME, MB_YESNO);
            if ( ret != IDYES ) { return FALSE; }

            return ::SetSuspendState(TRUE, FALSE, FALSE);
        }
        default:
        {
            return FALSE;
        }
    }
}

//---------------------------------------------------------------------------//

// TTBEvent_WindowsHook() の内部実装
void WINAPI Hook(UINT, WPARAM, LPARAM)
{
}

//---------------------------------------------------------------------------//
//
// CRT を使わないため new/delete を自前で実装
//
//---------------------------------------------------------------------------//

#if defined(_NODEFLIB)

void* __cdecl operator new(size_t size)
{
    return ::HeapAlloc(::GetProcessHeap(), 0, size);
}

void __cdecl operator delete(void* p)
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void __cdecl operator delete(void* p, size_t) // C++14
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void* __cdecl operator new[](size_t size)
{
    return ::HeapAlloc(::GetProcessHeap(), 0, size);
}

void __cdecl operator delete[](void* p)
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

void __cdecl operator delete[](void* p, size_t) // C++14
{
    if ( p != nullptr ) ::HeapFree(::GetProcessHeap(), 0, p);
}

// プログラムサイズを小さくするためにCRTを除外
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/entry:DllMain")

#endif

//---------------------------------------------------------------------------//

// DLL エントリポイント
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        g_hInst = hInstance;
    }
    return TRUE;
}

//---------------------------------------------------------------------------//

// Main.cpp
#include <Windows.h>
#include <kthook/kthook.hpp>
#include "CNetGame.h"
#include "CInput.h"
#include "CLocalPlayer.h"

using InitGameInstance = HWND(__cdecl *)(HINSTANCE);
using CTimer_UpdateSignature = void (__cdecl *)();
using CLocalPlayer_EnterVehicleSignature = void (__thiscall *)(CLocalPlayer *, int, BOOL);
kthook::kthook_simple<CLocalPlayer_EnterVehicleSignature> EnterVehicleHook{};
kthook::kthook_simple<CTimer_UpdateSignature> CTimerHook{};
kthook::kthook_simple<WNDPROC> wndproc_hook{};
kthook::kthook_signal<InitGameInstance> game_instance_init_hook{0x745560};

CInput *pInput;
bool bAutoLock{true};
bool bAutoRem{true};
bool bSpacePressed{false};

HWND game_hwnd = [] {
    if (const HWND *hwnd_ptr = *reinterpret_cast<HWND **>(0xC17054); hwnd_ptr != nullptr) {
        return *hwnd_ptr;
    }
    game_instance_init_hook.after += [](const auto &hook, const HWND &return_value, HINSTANCE inst) {
        game_hwnd = return_value;
    };
    return static_cast<HWND>(nullptr);
}();

void AddMessageJumpQ(const char *text, unsigned int time, unsigned short flag, bool bPreviousBrief) {
    ((void (__cdecl *)(const char *, unsigned int, unsigned short, bool)) 0x69F1E0)(text, time, flag, bPreviousBrief);
}

HRESULT __stdcall on_wndproc(const decltype(wndproc_hook) &hook, HWND hwnd, const UINT uMsg,
                             const WPARAM wParam, const LPARAM lParam) {
    if (uMsg == WM_KEYDOWN) {
        if (wParam == VK_SPACE) {
            bSpacePressed = true;
        }
    }
    if (uMsg == WM_KEYUP && bSpacePressed) {
        if (wParam == 0x31) {
            pInput->Send("/lock 1");
            AddMessageJumpQ("lock 1", 100, 0, false);
        }
        if (wParam == 0x32) {
            pInput->Send("/lock 2");
            AddMessageJumpQ("lock 2", 100, 0, false);
        }
        if (wParam == 0x33) {
            pInput->Send("/lock 3");
            AddMessageJumpQ("lock 3", 100, 0, false);
        }
        if (wParam == 0x34) {
            pInput->Send("/lock 4");
            AddMessageJumpQ("lock 4", 100, 0, false);
        }
        if (wParam == 0x35) {
            pInput->Send("/lock 5");
            AddMessageJumpQ("lock 5", 100, 0, false);
        }
        if (wParam == VK_SPACE) {
            bSpacePressed = false;
        }
    }
    return hook.get_trampoline()(hwnd, uMsg, wParam, lParam);
}

void
CLocalPlayer_EnterVehicle(const decltype(EnterVehicleHook) &hook, CLocalPlayer *this_, int nVehicle, BOOL bPassenger) {
    hook.get_trampoline()(this_, nVehicle, bPassenger);
    if (!bPassenger && bAutoRem) {
        std::thread([=]() {
            while (*(uint32_t *) ((uint32_t) this_->m_pPed->m_pGamePed + 0x530) != 50) {
                std::this_thread::sleep_for(std::chrono::milliseconds(0u));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000u));
            pInput->Send("/rem");
        }).detach();
    }
}

void CTimer_Update(const decltype(CTimerHook) &hook) {
    static bool init{};
    if (!init && RefNetGame()) {
        pInput = RefInputBox();

        const auto latest_wndproc_ptr = GetWindowLongPtrA(game_hwnd, GWLP_WNDPROC);
        wndproc_hook.set_dest(latest_wndproc_ptr);
        wndproc_hook.set_cb(&on_wndproc);
        wndproc_hook.install();

        EnterVehicleHook.set_dest(GetAddress(0x58E0));
        EnterVehicleHook.set_cb(&CLocalPlayer_EnterVehicle);
        EnterVehicleHook.install();

        pInput->AddCommand("autolock", [](const char *) {
            bAutoLock = !bAutoLock;
            if (bAutoLock)
                AddMessageJumpQ("~g~autolock by yorenov enabled", 100, 0, false);
            else
                AddMessageJumpQ("~r~autolock by yorenov disabled", 100, 0, false);
        });

        pInput->AddCommand("autorem", [](const char *) {
            bAutoRem = !bAutoRem;
            if (bAutoRem)
                AddMessageJumpQ("~g~autorem by yorenov enabled", 100, 0, false);
            else
                AddMessageJumpQ("~r~autorem by yorenov disabled", 100, 0, false);
        });

        AddMessageJumpQ("autolock & autorem by yorenov loaded", 100, 0, false);
        init = {true};
    }
    hook.get_trampoline()();
}


[[maybe_unused]] BOOL WINAPI DllMain(const HMODULE hinstDLL, const DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CTimerHook.set_dest(0x561B10);
        CTimerHook.set_cb(&CTimer_Update);
        CTimerHook.install();
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        EnterVehicleHook.remove();
        CTimerHook.remove();
        wndproc_hook.remove();
        game_instance_init_hook.remove();
    }
    return TRUE;
}
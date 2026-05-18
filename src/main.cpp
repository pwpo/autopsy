#include <iostream>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>

#include "driver/driver.h"
#include "global.h"
#include "log.h"
#include "ui/graphic.h"
#include "engine/engine.h"
#include "feature/cache.h"
#include "feature/misc.h"
#include "feature/world.h"
#include "feature/ball.h"
#include "feature/aim.h"
#include "feature/silent.h"

#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")

namespace
{
    bool process(const char* processName)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
            return false;

        PROCESSENTRY32 entry{};
        entry.dwSize = sizeof(entry);

        bool found = false;
        if (Process32First(snapshot, &entry))
        {
            do
            {
                if (_stricmp(entry.szExeFile, processName) == 0)
                {
                    found = true;
                    break;
                }
            } while (Process32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return found;
    }

    void watch(const char* processName)
    {
        for (;;)
        {
            Sleep(1000);
            if (!process(processName))
                ExitProcess(0);
        }
    }
}

std::int32_t main(std::int32_t argc, char** argv[])
{
    static constexpr const char* BINARY_NAME = { "RobloxPlayerBeta.exe" };

    if (!process(BINARY_NAME))
    {
        MessageBoxA(nullptr, "Open Roblox first.", "autopsy.lol", MB_ICONWARNING | MB_OK);
        return 0;
    }

    std::thread(watch, BINARY_NAME).detach();

    drive->process(BINARY_NAME);
    drive->attach(BINARY_NAME);
    drive->module(BINARY_NAME);

    auto fakemodel = drive->read<std::uint64_t>(drive->modulebase() + offset::fakemodel::Pointer);
    global::model.Address = drive->read<std::uint64_t>(fakemodel + offset::fakemodel::RealDataModel);
    global::render.Address = drive->read<std::uint64_t>(drive->modulebase() + offset::render::Pointer);
    global::actor.Address = global::model.childclass("Players").Address;
    global::workspace.Address = global::model.childclass("Workspace").Address;
    global::camera.Address = global::workspace.childclass("Camera").Address;
    auto Lightin = global::model.childclass("Lighting");
    global::light = sdk::light(Lightin.Address);

    std::thread(cache::run).detach();
    std::thread(world::run).detach();
    std::thread(aim::run).detach();
    std::thread(silent::run).detach();
    std::thread(misc::run).detach();
    std::thread(ball::run).detach();

    auto workspacetoworld = drive->read<uintptr_t>(global::workspace.Address + offset::workspace::world);
    drive->write<float>(workspacetoworld + 0x658, 200 * 4.f);

    screen->window();
    screen->device();
    screen->imgui();

    for (;;)
    {
        screen->begin();
        screen->visual();
        screen->menu();
        screen->end();
    }

    return 0;
}

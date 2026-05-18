#include "engine/engine.h"
#include "global.h"
#include "feature/cache.h"
#include <iostream>

namespace world {

    void skybox() {

        while (true)
        {
            if (global::world::Skybox)
            {
                auto sky = global::light.child("Sky");

                if (sky.Address)
                {
                    if (global::world::Skybox_Type == 0)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://12635309703");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://12635311686");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://12635312870");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://12635313718");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://12635315817");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://12635316856");
                    }
                    else if (global::world::Skybox_Type == 1)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://12064107");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://12064152");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://12064121");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://12063984");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://12064115");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://12064131");
                    }
                    else if (global::world::Skybox_Type == 2)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://271042516");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://271077243");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://271042556");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://271042310");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://271042467");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://271077958");
                    }
                    else if (global::world::Skybox_Type == 3)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://1876545003");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://1876544331");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://1876542941");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://1876543392");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://1876543764");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://1876544642");
                    }
                    else if (global::world::Skybox_Type == 4)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://116758234");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://116758314");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://116758367");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://116758446");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://116758478");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://116758496");
                    }
                    else if (global::world::Skybox_Type == 5)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://1233158420");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://1233158838");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://1233157105");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://1233157640");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://1233157995");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://1233159158");
                    }
                    else if (global::world::Skybox_Type == 6)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://1327358");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://1327359");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://1327355");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://1327357");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://1327356");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://1327360");
                    }
                    else if (global::world::Skybox_Type == 7)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://570555736");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://570555964");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://570555800");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://570555840");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://570555882");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://570555929");
                    }
                    else if (global::world::Skybox_Type == 8)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://95020137072033");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://92862258103959");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://107665368823185");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://126542804346203");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://103716549795832");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://131036626982613");
                    }
                    else if (global::world::Skybox_Type == 9)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://169210090");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://169210108");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://169210121");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://169210133");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://169210143");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://169210149");
                    }
                    else if (global::world::Skybox_Type == 10)
                    {
                        drive->writestring(sky.Address + offset::sky::SkyboxBk, "rbxassetid://47974894");
                        drive->writestring(sky.Address + offset::sky::SkyboxDn, "rbxassetid://47974690");
                        drive->writestring(sky.Address + offset::sky::SkyboxFt, "rbxassetid://47974821");
                        drive->writestring(sky.Address + offset::sky::SkyboxLf, "rbxassetid://47974776");
                        drive->writestring(sky.Address + offset::sky::SkyboxRt, "rbxassetid://47974859");
                        drive->writestring(sky.Address + offset::sky::SkyboxUp, "rbxassetid://47974909");
                    }

                    if (global::world::Rotate) {

                        auto sky = global::light.childclass("Sky");

                        static float rotY = 0.0f;
                        rotY = (0.0f > 360.0f) ? 0.0f : rotY + global::world::Skybox_Rotate_Speed;

                        drive->write<sdk::vector3>(sky.Address + offset::sky::SkyboxOrientation, { 0.0f, rotY, 0.0f });
                    }

                    sdk::view::invalidate();
                    sdk::view::skybox();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }

    void atmosphere() {

        while (true)
        {
            if (global::world::Ambience)
            {
                sdk::light::ambient(global::light.Address, {global::world::color::Ambience[0], global::world::color::Ambience[1], global::world::color::Ambience[2]} );
                sdk::view::invalidate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void fog() {

        while (true)
        {
            if (global::world::Fog)
            {
                sdk::light::fog(global::light.Address, global::world::Fog_Distance, {global::world::color::Fog[0], global::world::color::Fog[1], global::world::color::Fog[2]} );
                sdk::view::invalidate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void brightness() {

        while (true)
        {
            if (global::world::Brightness)
            {
                sdk::light::brightness(global::light.Address, global::world::BrightnessI);
                sdk::view::invalidate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void exposure() {

        while (true)
        {
            if (global::world::Exposure)
            {
                sdk::light::exposure(global::light.Address, global::world::ExposureI);
                sdk::view::invalidate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void fov() {

        while (true)
        {
            if (global::world::FOV)
            {
				sdk::light::fov(global::camera.Address, global::world::FOV_Distance);
            }
        }

    }

    void run() {

        std::thread(skybox).detach();
        std::thread(atmosphere).detach();
        std::thread(fog).detach();
        std::thread(brightness).detach();
        std::thread(exposure).detach();
		std::thread(fov).detach();
    }
}

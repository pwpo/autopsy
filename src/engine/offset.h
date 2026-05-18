#pragma once

#include <cstdint>
#include <string>
namespace offset {
    inline std::string ClientVersion = "version-9377ee10133e4be3";

    namespace air {
         inline constexpr uintptr_t AirDensity = 0x18;
         inline constexpr uintptr_t GlobalWind = 0x3c;
    }

    namespace animation {
        inline constexpr uintptr_t Animation = 0xd0;
        inline constexpr uintptr_t animator = 0x118;
        inline constexpr uintptr_t IsPlaying = 0xa18;
        inline constexpr uintptr_t Looped = 0xf5;
        inline constexpr uintptr_t Speed = 0xe4;
        inline constexpr uintptr_t TimePosition = 0xe8;
    }

    namespace animator {
        inline constexpr uintptr_t ActiveAnimations = 0x868;
    }

    namespace atmosphere {
        inline constexpr uintptr_t tocolor = 0xd0;
        inline constexpr uintptr_t Decay = 0xdc;
        inline constexpr uintptr_t Density = 0xe8;
        inline constexpr uintptr_t Glare = 0xec;
        inline constexpr uintptr_t Haze = 0xf0;
        inline constexpr uintptr_t Offset = 0xf4;
    }

    namespace attachment {
        inline constexpr uintptr_t Position = 0xdc;
    }

    namespace basepart {
        inline constexpr uintptr_t CastShadow = 0xf5;
        inline constexpr uintptr_t Color3 = 0x194;
        inline constexpr uintptr_t Locked = 0xf6;
        inline constexpr uintptr_t Massless = 0xf7;
        inline constexpr uintptr_t primitive = 0x148;
        inline constexpr uintptr_t Reflectance = 0xec;
        inline constexpr uintptr_t Shape = 0x1b1;
        inline constexpr uintptr_t Transparency = 0xf0;
    }

    namespace beam {
        inline constexpr uintptr_t Attachment0 = 0x170;
        inline constexpr uintptr_t Attachment1 = 0x180;
        inline constexpr uintptr_t Brightness = 0x190;
        inline constexpr uintptr_t CurveSize0 = 0x194;
        inline constexpr uintptr_t CurveSize1 = 0x198;
        inline constexpr uintptr_t LightEmission = 0x19c;
        inline constexpr uintptr_t LightInfluence = 0x1a0;
        inline constexpr uintptr_t Texture = 0x150;
        inline constexpr uintptr_t TextureLength = 0x1ac;
        inline constexpr uintptr_t TextureSpeed = 0x1b4;
        inline constexpr uintptr_t Width0 = 0x1b8;
        inline constexpr uintptr_t Width1 = 0x1bc;
        inline constexpr uintptr_t ZOffset = 0x1c0;
    }

    namespace bloom {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Intensity = 0xd0;
        inline constexpr uintptr_t Size = 0xd4;
        inline constexpr uintptr_t Threshold = 0xd8;
    }

    namespace blur {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Size = 0xd0;
    }

    namespace bytecode {
        inline constexpr uintptr_t Pointer = 0x10;
        inline constexpr uintptr_t Size = 0x20;
    }

    namespace camera {
        inline constexpr uintptr_t CameraSubject = 0xe8;
        inline constexpr uintptr_t CameraType = 0x158;
        inline constexpr uintptr_t FieldOfView = 0x160;
        inline constexpr uintptr_t ImagePlaneDepth = 0x2f0;
        inline constexpr uintptr_t Position = 0x11c;
        inline constexpr uintptr_t Rotation = 0xf8;
        inline constexpr uintptr_t Viewport = 0x2ac;
        inline constexpr uintptr_t ViewportSize = 0x2e8;
    }

    namespace charmesh {
        inline constexpr uintptr_t BaseTextureId = 0xe0;
        inline constexpr uintptr_t BodyPart = 0x160;
        inline constexpr uintptr_t MeshId = 0x110;
        inline constexpr uintptr_t OverlayTextureId = 0x140;
    }

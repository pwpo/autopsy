    namespace sky {
        inline constexpr uintptr_t MoonAngularSize = 0x25c;
        inline constexpr uintptr_t MoonTextureId = 0xe0;
        inline constexpr uintptr_t SkyboxBk = 0x110;
        inline constexpr uintptr_t SkyboxDn = 0x140;
        inline constexpr uintptr_t SkyboxFt = 0x170;
        inline constexpr uintptr_t SkyboxLf = 0x1a0;
        inline constexpr uintptr_t SkyboxOrientation = 0x250;
        inline constexpr uintptr_t SkyboxRt = 0x1d0;
        inline constexpr uintptr_t SkyboxUp = 0x200;
        inline constexpr uintptr_t StarCount = 0x260;
        inline constexpr uintptr_t SunAngularSize = 0x254;
        inline constexpr uintptr_t SunTextureId = 0x230;
    }

    namespace sound {
        inline constexpr uintptr_t Looped = 0x155;
        inline constexpr uintptr_t PlaybackSpeed = 0x134;
        inline constexpr uintptr_t Playing = 0x158;
        inline constexpr uintptr_t RollOffMaxDistance = 0x138;
        inline constexpr uintptr_t RollOffMinDistance = 0x13c;
        inline constexpr uintptr_t SoundGroup = 0x100;
        inline constexpr uintptr_t SoundId = 0xe0;
        inline constexpr uintptr_t Volume = 0x148;
    }

    namespace spawn {
        inline constexpr uintptr_t AllowTeamChangeOnTouch = 0x45;
        inline constexpr uintptr_t Enabled = 0x1f9;
        inline constexpr uintptr_t ForcefieldDuration = 0x1f0;
        inline constexpr uintptr_t Neutral = 0x1fa;
        inline constexpr uintptr_t TeamColor = 0x1f4;
    }

    namespace specialmesh {
        inline constexpr uintptr_t MeshId = 0x108;
        inline constexpr uintptr_t Scale = 0xdc;
    }

    namespace stat {
        inline constexpr uintptr_t Value = 0xc8;
    }

    namespace sunray {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Intensity = 0xd0;
        inline constexpr uintptr_t Spread = 0xd4;
    }

    namespace surface {
        inline constexpr uintptr_t AlphaMode = 0x2a0;
        inline constexpr uintptr_t tocolor = 0x288;
        inline constexpr uintptr_t ColorMap = 0xe0;
        inline constexpr uintptr_t EmissiveMaskContent = 0x110;
        inline constexpr uintptr_t EmissiveStrength = 0x2a4;
        inline constexpr uintptr_t EmissiveTint = 0x294;
        inline constexpr uintptr_t MetalnessMap = 0x140;
        inline constexpr uintptr_t NormalMap = 0x170;
        inline constexpr uintptr_t RoughnessMap = 0x1a0;
    }

    namespace task {
        inline constexpr uintptr_t JobEnd = 0xd0;
        inline constexpr uintptr_t JobName = 0x18;
        inline constexpr uintptr_t JobStart = 0xc8;
        inline constexpr uintptr_t MaxFPS = 0xb0;
        inline constexpr uintptr_t Pointer = 0x7bfe988;
    }

    namespace team {
        inline constexpr uintptr_t BrickColor = 0xd0;
    }

    namespace terrain {
        inline constexpr uintptr_t GrassLength = 0x1f8;
        inline constexpr uintptr_t material = 0x2a8;
        inline constexpr uintptr_t WaterColor = 0x1e8;
        inline constexpr uintptr_t WaterReflectance = 0x200;
        inline constexpr uintptr_t WaterTransparency = 0x204;
        inline constexpr uintptr_t WaterWaveSize = 0x208;
        inline constexpr uintptr_t WaterWaveSpeed = 0x20c;
    }

    namespace texture {
        inline constexpr uintptr_t Decal_Texture = 0x198;
        inline constexpr uintptr_t Texture_Texture = 0x198;
    }

    namespace tool {
        inline constexpr uintptr_t CanBeDropped = 0x4c8;
        inline constexpr uintptr_t Enabled = 0x4c9;
        inline constexpr uintptr_t Grip = 0x4bc;
        inline constexpr uintptr_t ManualActivationOnly = 0x4ca;
        inline constexpr uintptr_t RequiresHandle = 0x4cb;
        inline constexpr uintptr_t TextureId = 0x370;
        inline constexpr uintptr_t Tooltip = 0x478;
    }

    namespace unionop {
        inline constexpr uintptr_t AssetId = 0x2f0;
    }

    namespace input {
        inline constexpr uintptr_t inputstate = 0x2d8;
    }

    namespace vehicle {
        inline constexpr uintptr_t MaxSpeed = 0x238;
        inline constexpr uintptr_t SteerFloat = 0x240;
        inline constexpr uintptr_t ThrottleFloat = 0x248;
        inline constexpr uintptr_t Torque = 0x24c;
        inline constexpr uintptr_t TurnSpeed = 0x250;
    }

    namespace render {
        inline constexpr uintptr_t Dimensions = 0xaa0;
        inline constexpr uintptr_t fakemodel = 0xa80;
        inline constexpr uintptr_t Pointer = 0x7bd51f8;
        inline constexpr uintptr_t view = 0xb80;
        inline constexpr uintptr_t ViewMatrix = 0x140;
    }

    namespace weld {
        inline constexpr uintptr_t Part0 = 0x130;
        inline constexpr uintptr_t Part1 = 0x140;
    }

    namespace weldconstraint {
        inline constexpr uintptr_t Part0 = 0xd0;
        inline constexpr uintptr_t Part1 = 0xe0;
    }

    namespace inputstate {
        inline constexpr uintptr_t CapsLock = 0x40;
        inline constexpr uintptr_t CurrentTextBox = 0x48;
    }

    namespace workspace {
        inline constexpr uintptr_t CurrentCamera = 0x4b0;
        inline constexpr uintptr_t DistributedGameTime = 0x4d0;
        inline constexpr uintptr_t ReadOnlyGravity = 0x9e0;
        inline constexpr uintptr_t world = 0x408;
    }

    namespace world {
        inline constexpr uintptr_t air = 0x218;
        inline constexpr uintptr_t FallenPartsDestroyHeight = 0x208;
        inline constexpr uintptr_t Gravity = 0x210;
        inline constexpr uintptr_t Primitives = 0x280;
        inline constexpr uintptr_t worldStepsPerSec = 0x678;
    }

    namespace silent {
         inline constexpr uintptr_t FramePositionOffsetX = 0x51C;
         inline constexpr uintptr_t FramePositionOffsetY = 0x524;
         inline constexpr uintptr_t FramePositionX = 0x518;
         inline constexpr uintptr_t FramePositionY = 0x520;
         inline constexpr uintptr_t FrameRotation = 0x188;
         inline constexpr uintptr_t FrameSizeOffsetX = 0x540;
         inline constexpr uintptr_t FrameSizeOffsetY = 0x544;
         inline constexpr uintptr_t FrameSizeX = 0x538;
         inline constexpr uintptr_t FrameSizeY = 0x53C;
    }

    namespace flag {
         inline constexpr uintptr_t List = 0x7ce33d8;
         inline constexpr uintptr_t ToValueGetSet = 0x30;
    }

    namespace value {
         inline constexpr uintptr_t ToValue = 0xc0;
    }

}

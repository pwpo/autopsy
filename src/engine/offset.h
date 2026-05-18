#pragma once

#include <cstdint>
#include <string>
namespace offset {
    inline std::string ClientVersion = "version-ec412128eba3476e";

    namespace air {
         inline constexpr uintptr_t AirDensity = 0x18;
         inline constexpr uintptr_t GlobalWind = 0x3c;
    }

    namespace animation {
        inline constexpr uintptr_t Animation = 0xd0;
        inline constexpr uintptr_t animator = 0x118;
        inline constexpr uintptr_t IsPlaying = 0x405;
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

    namespace click {
        inline constexpr uintptr_t MaxActivationDistance = 0x100;
        inline constexpr uintptr_t MouseIcon = 0xe0;
    }

    namespace clothing {
        inline constexpr uintptr_t Color3 = 0x128;
        inline constexpr uintptr_t Template = 0x108;
    }

    namespace correction {
        inline constexpr uintptr_t Brightness = 0xdc;
        inline constexpr uintptr_t Contrast = 0xe0;
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t TintColor = 0xd0;
    }

    namespace grading {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t TonemapperPreset = 0xd0;
    }

    namespace datamodel {
        inline constexpr uintptr_t CreatorId = 0x190;
        inline constexpr uintptr_t GameId = 0x198;
        inline constexpr uintptr_t GameLoaded = 0x638;
        inline constexpr uintptr_t JobId = 0x138;
        inline constexpr uintptr_t PlaceId = 0x1a0;
        inline constexpr uintptr_t PlaceVersion = 0x1bc;
        inline constexpr uintptr_t PrimitiveCount = 0x480;
        inline constexpr uintptr_t scriptcontext = 0x430;
        inline constexpr uintptr_t ServerIP = 0x620;
        inline constexpr uintptr_t ToRenderView1 = 0x1d8;
        inline constexpr uintptr_t ToRenderView2 = 0x8;
        inline constexpr uintptr_t ToRenderView3 = 0x28;
        inline constexpr uintptr_t workspace = 0x178;
    }

    namespace depth {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t FarIntensity = 0xd0;
        inline constexpr uintptr_t FocusDistance = 0xd4;
        inline constexpr uintptr_t InFocusRadius = 0xd8;
        inline constexpr uintptr_t NearIntensity = 0xdc;
    }

    namespace drag {
        inline constexpr uintptr_t ActivatedCursorIcon = 0x1d8;
        inline constexpr uintptr_t CursorIcon = 0xe0;
        inline constexpr uintptr_t MaxActivationDistance = 0x100;
        inline constexpr uintptr_t MaxDragAngle = 0x2c0;
        inline constexpr uintptr_t MaxDragTranslation = 0x284;
        inline constexpr uintptr_t MaxForce = 0x2c4;
        inline constexpr uintptr_t MaxTorque = 0x2c8;
        inline constexpr uintptr_t MinDragAngle = 0x2cc;
        inline constexpr uintptr_t MinDragTranslation = 0x290;
        inline constexpr uintptr_t ReferenceInstance = 0x208;
        inline constexpr uintptr_t Responsiveness = 0x2d8;
    }

    namespace fakemodel {
        inline constexpr uintptr_t Pointer = 0x74f8758;
        inline constexpr uintptr_t RealDataModel = 0x1d0;
    }

    namespace gui2d {
        inline constexpr uintptr_t AbsolutePosition = 0x110;
        inline constexpr uintptr_t AbsoluteRotation = 0x188;
        inline constexpr uintptr_t AbsoluteSize = 0x118;
    }

    namespace gui {
        inline constexpr uintptr_t BackgroundColor3 = 0x548;
        inline constexpr uintptr_t BackgroundTransparency = 0x554;
        inline constexpr uintptr_t BorderColor3 = 0x554;
        inline constexpr uintptr_t Image = 0x990;
        inline constexpr uintptr_t LayoutOrder = 0x588;
        inline constexpr uintptr_t Position = 0x518;
        inline constexpr uintptr_t RichText = 0xb60;
        inline constexpr uintptr_t Rotation = 0x188;
        inline constexpr uintptr_t ScreenGui_Enabled = 0x4cc;
        inline constexpr uintptr_t Size = 0x538;
        inline constexpr uintptr_t text = 0xda8;
        inline constexpr uintptr_t TextColor3 = 0xe58;
        inline constexpr uintptr_t Visible = 0x5b5;
        inline constexpr uintptr_t ZIndex = 0x5ac;
    }

    namespace humanoid {
        inline constexpr uintptr_t AutoJumpEnabled = 0x1e0;
        inline constexpr uintptr_t AutoRotate = 0x1e1;
        inline constexpr uintptr_t AutomaticScalingEnabled = 0x1e2;
        inline constexpr uintptr_t BreakJointsOnDeath = 0x1e3;
        inline constexpr uintptr_t CameraOffset = 0x140;
        inline constexpr uintptr_t DisplayDistanceType = 0x18c;
        inline constexpr uintptr_t DisplayName = 0xd0;
        inline constexpr uintptr_t EvaluateStateMachine = 0x1e4;
        inline constexpr uintptr_t FloorMaterial = 0x190;
        inline constexpr uintptr_t Health = 0x194;
        inline constexpr uintptr_t HealthDisplayDistance = 0x198;
        inline constexpr uintptr_t HealthDisplayType = 0x19c;
        inline constexpr uintptr_t HipHeight = 0x1a0;
        inline constexpr uintptr_t HumanoidRootPart = 0x488;
        inline constexpr uintptr_t HumanoidState = 0x8a8;
        inline constexpr uintptr_t HumanoidStateID = 0x20;
        inline constexpr uintptr_t IsWalking = 0x927;
        inline constexpr uintptr_t Jump = 0x1e6;
        inline constexpr uintptr_t JumpHeight = 0x1ac;
        inline constexpr uintptr_t JumpPower = 0x1b0;
        inline constexpr uintptr_t MaxHealth = 0x1b4;
        inline constexpr uintptr_t MaxSlopeAngle = 0x1b8;
        inline constexpr uintptr_t MoveDirection = 0x158;
        inline constexpr uintptr_t MoveToPart = 0x130;
        inline constexpr uintptr_t MoveToPoint = 0x17c;
        inline constexpr uintptr_t NameDisplayDistance = 0x1bc;
        inline constexpr uintptr_t NameOcclusion = 0x1c0;
        inline constexpr uintptr_t PlatformStand = 0x1e8;
        inline constexpr uintptr_t RequiresNeck = 0x1e9;
        inline constexpr uintptr_t RigType = 0x1cc;
        inline constexpr uintptr_t SeatPart = 0x120;
        inline constexpr uintptr_t Sit = 0x1e9;
        inline constexpr uintptr_t TargetPoint = 0x164;
        inline constexpr uintptr_t UseJumpPower = 0x1ec;
        inline constexpr uintptr_t WalkTimer = 0x418;
        inline constexpr uintptr_t walkspeed = 0x1dc;
        inline constexpr uintptr_t WalkspeedCheck = 0x3c4;
    }

    namespace instance {
        inline constexpr uintptr_t AttributeContainer = 0x48;
        inline constexpr uintptr_t AttributeList = 0x18;
        inline constexpr uintptr_t AttributeToNext = 0x58;
        inline constexpr uintptr_t AttributeToValue = 0x18;
        inline constexpr uintptr_t ChildrenEnd = 0x8;
        inline constexpr uintptr_t ChildrenStart = 0x78;
        inline constexpr uintptr_t ClassBase = 0x47d8;
        inline constexpr uintptr_t ClassDescriptor = 0x18;
        inline constexpr uintptr_t ClassName = 0x8;
        inline constexpr uintptr_t name = 0xb0;
        inline constexpr uintptr_t parent = 0x70;
        inline constexpr uintptr_t This = 0x8;
    }

    namespace light {
        inline constexpr uintptr_t Ambient = 0xe0;
        inline constexpr uintptr_t Brightness = 0x128;
        inline constexpr uintptr_t ClockTime = 0x1c0;
        inline constexpr uintptr_t ColorShift_Bottom = 0xf8;
        inline constexpr uintptr_t ColorShift_Top = 0xec;
        inline constexpr uintptr_t EnvironmentDiffuseScale = 0x12c;
        inline constexpr uintptr_t EnvironmentSpecularScale = 0x130;
        inline constexpr uintptr_t ExposureCompensation = 0x134;
        inline constexpr uintptr_t FogColor = 0x104;
        inline constexpr uintptr_t FogEnd = 0x13c;
        inline constexpr uintptr_t FogStart = 0x140;
        inline constexpr uintptr_t GeographicLatitude = 0x198;
        inline constexpr uintptr_t GlobalShadows = 0x150;
        inline constexpr uintptr_t GradientBottom = 0x19c;
        inline constexpr uintptr_t GradientTop = 0x158;
        inline constexpr uintptr_t LightColor = 0x164;
        inline constexpr uintptr_t LightDirection = 0x170;
        inline constexpr uintptr_t MoonPosition = 0x18c;
        inline constexpr uintptr_t OutdoorAmbient = 0x110;
        inline constexpr uintptr_t sky = 0x1e0;
        inline constexpr uintptr_t Source = 0x17c;
        inline constexpr uintptr_t SunPosition = 0x180;
    }

    namespace localscript {
        inline constexpr uintptr_t bytecode = 0x0;
        inline constexpr uintptr_t GUID = 0xe8;
        inline constexpr uintptr_t Hash = 0x1b8;
    }

    namespace material {
        inline constexpr uintptr_t Asphalt = 0x30;
        inline constexpr uintptr_t Basalt = 0x27;
        inline constexpr uintptr_t Brick = 0xf;
        inline constexpr uintptr_t Cobblestone = 0x33;
        inline constexpr uintptr_t Concrete = 0xc;
        inline constexpr uintptr_t CrackedLava = 0x2d;
        inline constexpr uintptr_t Glacier = 0x1b;
        inline constexpr uintptr_t Grass = 0x6;
        inline constexpr uintptr_t Ground = 0x2a;
        inline constexpr uintptr_t Ice = 0x36;
        inline constexpr uintptr_t LeafyGrass = 0x39;
        inline constexpr uintptr_t Limestone = 0x3f;
        inline constexpr uintptr_t Mud = 0x24;
        inline constexpr uintptr_t Pavement = 0x42;
        inline constexpr uintptr_t Rock = 0x18;
        inline constexpr uintptr_t Salt = 0x3c;
        inline constexpr uintptr_t Sand = 0x12;
        inline constexpr uintptr_t Sandstone = 0x21;
        inline constexpr uintptr_t Slate = 0x9;
        inline constexpr uintptr_t Snow = 0x1e;
        inline constexpr uintptr_t WoodPlanks = 0x15;
    }

    namespace meshprovider {
        inline constexpr uintptr_t AssetID = 0x10;
        inline constexpr uintptr_t cache = 0xf0;
        inline constexpr uintptr_t LRUCache = 0x20;
        inline constexpr uintptr_t meshdata = 0x40;
        inline constexpr uintptr_t ToMeshData = 0x40;
    }

    namespace meshdata {
        inline constexpr uintptr_t FaceEnd = 0x38;
        inline constexpr uintptr_t FaceStart = 0x30;
        inline constexpr uintptr_t VertexEnd = 0x8;
        inline constexpr uintptr_t VertexStart = 0x0;
    }

    namespace meshpart {
        inline constexpr uintptr_t MeshId = 0x2f8;
        inline constexpr uintptr_t Texture = 0x328;
    }

    namespace misc {
        inline constexpr uintptr_t Adornee = 0x108;
        inline constexpr uintptr_t AnimationId = 0xd0;
        inline constexpr uintptr_t StringLength = 0x10;
        inline constexpr uintptr_t Value = 0xd0;
    }

    namespace model {
        inline constexpr uintptr_t PrimaryPart = 0x278;
        inline constexpr uintptr_t Scale = 0x164;
    }

    namespace modulescript {
        inline constexpr uintptr_t bytecode = 0x0;
        inline constexpr uintptr_t GUID = 0xe8;
        inline constexpr uintptr_t Hash = 0x160;
        inline constexpr uintptr_t IsCoreScript = 0x0;
    }

    namespace mouseservice {
        inline constexpr uintptr_t InputObject = 0x108;
        inline constexpr uintptr_t InputObject2 = 0x118;
        inline constexpr uintptr_t MousePosition = 0xec;
        inline constexpr uintptr_t SensitivityPointer = 0x307;
    }

    namespace particle {
        inline constexpr uintptr_t Acceleration = 0x1f0;
        inline constexpr uintptr_t Brightness = 0x22c;
        inline constexpr uintptr_t Drag = 0x230;
        inline constexpr uintptr_t Lifetime = 0x204;
        inline constexpr uintptr_t LightEmission = 0x248;
        inline constexpr uintptr_t LightInfluence = 0x24c;
        inline constexpr uintptr_t Rate = 0x258;
        inline constexpr uintptr_t RotSpeed = 0x20c;
        inline constexpr uintptr_t Rotation = 0x214;
        inline constexpr uintptr_t Speed = 0x21c;
        inline constexpr uintptr_t SpreadAngle = 0x224;
        inline constexpr uintptr_t Texture = 0x1d0;
        inline constexpr uintptr_t TimeScale = 0x26c;
        inline constexpr uintptr_t VelocityInheritance = 0x270;
        inline constexpr uintptr_t ZOffset = 0x274;
    }

    namespace player {
        inline constexpr uintptr_t AccountAge = 0x32c;
        inline constexpr uintptr_t CameraMode = 0x338;
        inline constexpr uintptr_t DisplayName = 0x130;
        inline constexpr uintptr_t HealthDisplayDistance = 0x358;
        inline constexpr uintptr_t LocalPlayer = 0x138;
        inline constexpr uintptr_t LocaleId = 0x110;
        inline constexpr uintptr_t MaxZoomDistance = 0x330;
        inline constexpr uintptr_t MinZoomDistance = 0x334;
        inline constexpr uintptr_t ModelInstance = 0x3a8;
        inline constexpr uintptr_t mouse = 0x1178;
        inline constexpr uintptr_t NameDisplayDistance = 0x368;
        inline constexpr uintptr_t team = 0x2b0;
        inline constexpr uintptr_t TeamColor = 0x374;
        inline constexpr uintptr_t UserId = 0x2d8;
    }

    namespace playerconfig {
        inline constexpr uintptr_t Pointer = 0x307;
    }

    namespace playermouse {
        inline constexpr uintptr_t icon = 0xe0;
        inline constexpr uintptr_t workspace = 0x168;
    }

    namespace primitive {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0x104;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xf8;
        inline constexpr uintptr_t Flags = 0x1b6;
        inline constexpr uintptr_t Material = 0x0;
        inline constexpr uintptr_t Owner = 0x200;
        inline constexpr uintptr_t Position = 0xec;
        inline constexpr uintptr_t Rotation = 0xc8;
        inline constexpr uintptr_t Size = 0x1b8;
        inline constexpr uintptr_t Validate = 0x6;
    }

    namespace primitiveflag {
        inline constexpr uintptr_t Anchored = 0x2;
        inline constexpr uintptr_t CanCollide = 0x8;
        inline constexpr uintptr_t CanQuery = 0x20;
        inline constexpr uintptr_t CanTouch = 0x10;
    }

    namespace prompt {
        inline constexpr uintptr_t ActionText = 0xc8;
        inline constexpr uintptr_t Enabled = 0x14e;
        inline constexpr uintptr_t GamepadKeyCode = 0x134;
        inline constexpr uintptr_t HoldDuration = 0x138;
        inline constexpr uintptr_t KeyCode = 0x13c;
        inline constexpr uintptr_t MaxActivationDistance = 0x140;
        inline constexpr uintptr_t ObjectText = 0xe8;
        inline constexpr uintptr_t RequiresLineOfSight = 0x14f;
    }

    namespace job {
        inline constexpr uintptr_t fakemodel = 0x38;
        inline constexpr uintptr_t RealDataModel = 0x1c0;
        inline constexpr uintptr_t view = 0x1d0;
    }

    namespace view {
        inline constexpr uintptr_t DeviceD3D11 = 0x8;
        inline constexpr uintptr_t LightingValid = 0x148;
        inline constexpr uintptr_t SkyValid = 0x28d;
        inline constexpr uintptr_t render = 0x10;
    }

    namespace run {
        inline constexpr uintptr_t HeartbeatFPS = 0xf4;
        inline constexpr uintptr_t HeartbeatTask = 0x3b8;
    }

    namespace script {
        inline constexpr uintptr_t bytecode = 0x0;
        inline constexpr uintptr_t GUID = 0xe8;
        inline constexpr uintptr_t Hash = 0x1b8;
    }

    namespace scriptcontext {
        inline constexpr uintptr_t RequireBypass = 0x0;
    }

    namespace seat {
        inline constexpr uintptr_t Occupant = 0x220;
    }

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
        inline constexpr uintptr_t Pointer = 0x7c00988;
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
        inline constexpr uintptr_t Pointer = 0x7bd71f8;
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

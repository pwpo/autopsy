#pragma once
#include <memory>
#include <vector>
#include <atomic>

#include "engine/engine.h"
#include "feature/cache.h"
#include "ImGui/imgui.h"

namespace global {

	inline sdk::model model;
	inline std::uint64_t GameID;
	inline sdk::render render;
	inline sdk::player LocalPlayer;
	inline sdk::actor actor;
	inline sdk::model workspace;
	inline sdk::light light;
	inline sdk::view view;
	inline sdk::camera camera;

	inline std::vector<sdk::player> Player_Cache;

	namespace overlay {

		inline bool watermark = true;
		inline bool hotkey = true;
		inline bool Hotkey_Aimbot = true;
		inline bool Hotkey_Silent = true;
		inline bool Hotkey_Fly = true;
		inline bool Hotkey_BladeBallSpam = true;
		inline bool Hotkey_Walkspeed = true;
		inline bool Hotkey_HitboxExpander = true;
		inline bool radar = false;
		inline bool AimWarning = false;
		inline ImVec2 Watermark_Pos = { 18.f, 18.f };
		inline ImVec2 Hotkeys_Pos = { 18.f, 72.f };
		inline ImVec2 Radar_Pos = { 18.f, 224.f };
		inline int Radar_Shape = 0;
		inline bool Radar_Rotate = true;
		inline float Radar_Zoom = 1.25f;
		inline float Radar_Size = 178.f;
		inline float AimView_MaxLength = 250.f;

		namespace color {
			inline float Accent[4]{ 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Accent2[4]{ 0.392f, 0.459f, 1.0f, 1.0f };
			inline float panel[4]{ 0.012f, 0.031f, 0.055f, 0.737f };
			inline float text[4]{ 0.91f, 0.949f, 0.976f, 1.0f };
		}
	}

	namespace setting {

		inline bool Team_Check;
		inline bool Client_Check;
		inline bool Streamproof;
		inline int Performance_Mode = 2;
		inline bool Compact_UI = false;
		inline ImGuiKey Menu_Key = ImGuiKey_Insert;

		namespace color {
			inline float Accent[4]{ 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Accent2[4]{ 0.392f, 0.459f, 1.0f, 1.0f };
			inline float Window[4]{ 0.012f, 0.024f, 0.039f, 1.0f };
			inline float card[4]{ 0.031f, 0.055f, 0.086f, 1.0f };
			inline float text[4]{ 0.91f, 0.949f, 0.976f, 1.0f };
		}
	}

	namespace ball {
		inline constexpr std::uint64_t PlaceId = 13772394625ULL;
		inline bool BallEsp = false;
		inline bool AutoParry = false;
		inline bool DrawParryRange = true;
		inline bool ClashMode = false;
		inline bool pressf = true;
		inline bool PredictTiming = true;
		inline bool DynamicTiming = true;
		inline bool AutoRange = true;
		inline bool AutoTiming = true;
		inline bool SpamParry = false;
		inline float ParryDistance = 32.f;
		inline float DistanceBuffer = 20.f;
		inline float ParryCooldownMs = 90.f;
		inline float ParryTimingMs = 145.f;
		inline float MinBallSpeed = 12.f;
		inline ImGuiKey SpamParry_Key = ImGuiKey_C;
		inline ImKeyBindMode SpamParry_Mode = ImKeyBindMode_Hold;
		inline float BallEspColor[4]{ 0.0f, 0.682f, 1.0f, 1.0f };
		inline float ParryRangeColor[4]{ 0.392f, 0.459f, 1.0f, 0.75f };
	}

	namespace world {

		inline bool Skybox;
		inline bool Rotate;
		inline bool Ambience;
		inline bool Fog;
		inline bool Brightness;
		inline bool Exposure;
		inline bool FOV;

		inline int Skybox_Type = 0;
		inline float Skybox_Rotate_Speed = 1.0f;
		inline float Fog_Distance = 300.0f;
		inline float FOV_Distance = 90.0f;

		inline float ExposureI = 0.0f;
		inline float BrightnessI = 1.0f;

		namespace color {

			inline float Ambience[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Fog[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
		}
	}

	namespace aim {

		inline bool useFov;


		inline bool Enabled;
		inline bool DrawFov;
		inline bool FovSpin;
		inline bool ClosestPlayerFound;
		inline bool FillFov;
		inline bool AimbotSticky;
		inline bool Shake;
		inline bool KnockedCheck;
		inline bool Prediction;
		inline bool AutoPrediction;

		inline float ShakeX{ 0.0f };
		inline float ShakeY{ 0.0f };
		inline float ShakeZ{ 0.0f };
		inline float PredictionX{ 0.12f };
		inline float PredictionY{ 0.12f };
		inline float PredictionZ{ 0.12f };


		inline sdk::instance AimTarget;
		inline float FovSize = 50;

		inline int HitPart = 0;
		inline int Aimbot_type = 0;
		inline int HitChance = 100;

		inline float FovColor[4] = { 0.0f, 0.682f, 1.0f, 1.0f };

		inline int FovSpinSpeed = 1;

		inline ImGuiKey Aimbot_Key = ImGuiKey_Q;
		inline ImKeyBindMode Aimbot_Mode = ImKeyBindMode_Hold;


		namespace camera {
			inline float Smoothing_X{ 1.0f };
			inline float Smoothing_Y{ 1.0f };
		}

		namespace mouse {
			inline float Smoothing_X{ 1.0f };
			inline float Smoothing_Y{ 1.0f };

			inline float Mouse_Sensitivty{ 1.0f };
		}
	}

	namespace silent
	{
		inline bool DrawFov{ false };
		inline bool Enabled{ false };
		inline bool StickyAim{ false };
		inline bool SpoofMouse{ true };
		inline bool UseFov{ false };
		inline bool KnockedCheck{ false };
		inline bool GunBasedFov{ false };
		inline bool Prediction{ false };
		inline bool AutoPrediction{ true };
		inline float fov{ 67.67f };
		inline float FovDoubleBarrel{ 67.67f };
		inline float FovTacticalShotgun{ 67.67f };
		inline float FovRevolver{ 67.67f };
		inline float PredictionX{ 0.135f };
		inline float PredictionY{ 0.135f };
		inline float PredictionZ{ 0.135f };
		inline ImGuiKey Silent_Key = ImGuiKey_Q;
		inline ImKeyBindMode Silent_Mode = ImKeyBindMode_Toggle;
		inline int AimPart{ 0 };
		inline float FovColor[4]{ 0.392f, 0.459f, 1.0f, 1.0f };
		inline int FovSpinSpeed = 1;
		inline bool FovSpin;
		inline bool FillFov;
	}

	namespace esp {

		inline bool Enabled;
		inline bool Box;
		inline bool Box_Fill;
		inline bool Box_Fill_Gradient;
		inline bool Box_Fill_Gradient_Rotate;
		inline bool Healthbar;
		inline bool Health;
		inline bool name;
		inline bool Distance;
		inline bool Rig_Type;
		inline bool tool;
		inline bool Skeleton;
		inline bool Chams;
		inline bool ChamsFade;
		inline bool Trails;
		inline bool Chinese_Hat;
		inline bool aimline;

		inline float Render_Distance = 200.0f;

		inline int ChamsFadeSpeed = 2;
		inline int BoxFillSpeed = 2;
		inline int Healthbar_Type = 1;
		inline int Box_Type = 0;
		inline int Box_Fill_Type = 0;
		inline int Name_Type = 1;
		inline int gap = 2;
		inline int Thickness = 2;

		namespace color {

			inline float Box[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float BoxFill_Top[4] = { 0.0f, 0.682f, 1.0f, 0.50f };
			inline float BoxFill_Bottom[4] = { 0.f, 0.f, 0.f, 0.50f };
			inline float Healthbar[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float name[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Distance[4] = { 0.392f, 0.459f, 1.0f, 1.0f };
			inline float Rig_Type[4] = { 0.392f, 0.459f, 1.0f, 1.0f };
			inline float tool[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Health[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float Skeleton[4] = { 0.392f, 0.459f, 1.0f, 1.0f };
			inline float Chams[4] = { 0.392f, 0.459f, 1.0f, 1.0f };
			inline float ChamsOutline[4] = { 0.f, 0.f, 0.f, 1.0f };
			inline float Trails[4] = { 0.0f, 0.682f, 1.0f, 1.0f };
			inline float hat[4] = { 0.392f, 0.459f, 1.0f, 1.0f };
			inline float aimline[4] = { 1.0f, 0.313f, 0.407f, 1.0f };

			inline float Healthbar_Top[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			inline float Healthbar_Middle[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
			inline float Healthbar_Bottom[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		}
	}

	namespace misc {

		inline bool fly = false;
		inline float Fly_Speed = 50.0f;
		inline ImGuiKey Fly_Key = ImGuiKey_Z;
		inline ImKeyBindMode Fly_Mode = ImKeyBindMode_Toggle;
		inline bool walkspeed = false;
		inline float Walkspeed_Speed = 32.0f;
		inline bool hitbox = false;
		inline float Hitbox_Size_X = 8.0f;
		inline float Hitbox_Size_Y = 8.0f;
		inline float Hitbox_Size_Z = 8.0f;
	}
}

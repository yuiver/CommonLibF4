#pragma once

#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSTArray.h"

namespace RE
{
	class NiCamera;
	class NiDirectionalLight;
	class ShadowSceneNode;

	enum class UI_DEPTH_PRIORITY;

	class OldScreenEffectControl
	{
	public:
		// members
		NiPoint2 UVOffset;         // 00
		float burstAmount;         // 08
		float brightness;          // 0C
		float VScanState;          // 10
		float VScanRate;           // 14
		float pulseIntensity;      // 18
		float pulseRate;           // 1C
		float flickerMinDelay;     // 20
		float flickerMaxDelay;     // 24
		float flickerMinDuration;  // 28
		float flickerMaxDuration;  // 2C
		float flickerFrequency;    // 30
		float flickerBrightness;   // 34
		float VScanDelayMin;       // 38
		float VScanDelayMax;       // 3C
		bool doScanlines;          // 40
		std::uint8_t pad41{ 0 };   // 41
		std::uint16_t pad42{ 0 };  // 42
		float burstState;          // 44
		float burstFadeRate;       // 48
		float shudderState;        // 4C
		float VScanDelay;          // 50
		float flickerDelay;        // 54
		float VHoldState;          // 58
		float shudderScale;        // 5C
		float shudderLength;       // 60
		float VHoldRate;           // 64
		float time;                // 68
		bool flickering;           // 6C
		std::uint8_t pad6D{ 0 };   // 6D
		std::uint16_t pad6E{ 0 };  // 6E
	};
	static_assert(sizeof(OldScreenEffectControl) == 0x70);

	namespace Interface3D
	{
		enum class BackgroundMode;
		enum class HighlightEffect;
		enum class LightType;
		enum class OffscreenMenuBlendMode;
		enum class OffscreenMenuSize;
		enum class PostEffect;
		enum class ScreenMode;

		enum class PostEffect
		{
			kNone = 0,
			kPipboy = 1,
			kHUDGlass = 2,
			kHUDGlassWithMod = 3,
			kModMenu = 4,
			kModMenuHighlightAll = 5,
			kModMenuHighlightAllNoScanlines = 6,
		};

		class Renderer
		{
		public:
			struct LightParams
			{
			public:
				// members
				stl::enumeration<LightType, std::uint32_t> type;  // 00
				float spotFOV;                                    // 04
				NiPoint3 lookAtPos;                               // 08
				std::uint32_t pad14{ 0 };                         // 14
				NiPointer<NiAVObject> lookAtObject;               // 18
				NiPointer<NiLight> light;                         // 20
			};
			static_assert(sizeof(LightParams) == 0x28);

			[[nodiscard]] static Interface3D::Renderer* GetByName(const BSFixedString& a_name)
			{
				using func_t = decltype(&Renderer::GetByName);
				REL::Relocation<func_t> func{ REL::ID(140387) };
				return func(a_name);
			}

			void Enable(bool a_unhideGeometry)
			{
				using func_t = decltype(&Renderer::Enable);
				REL::Relocation<func_t> func{ REL::ID(326895) };
				return func(this, a_unhideGeometry);
			}

			void Disable()
			{
				using func_t = decltype(&Renderer::Disable);
				REL::Relocation<func_t> func{ REL::ID(938370) };
				return func(this);
			}

			// members
			BSTAlignedArray<UIShaderFXInfo> colorFXInfos;                           // 00
			BSTAlignedArray<UIShaderFXInfo> backgroundFXInfos;                      // 18
			BSReadWriteLock cachedQuadsLock;                                        // 30
			NiColorA clearColor;                                                    // 38
			float opacityAlpha;                                                     // 48
			bool enabled;                                                           // 4C
			bool offscreen3DEnabled;                                                // 4D
			bool hideScreenWhenDisabled;                                            // 4E
			bool usePremultAlpha;                                                   // 4F
			bool clearRenderTarget;                                                 // 50
			bool clearDepthStencilMainScreen;                                       // 51
			bool clearDepthStencilOffscreen;                                        // 52
			bool postAA;                                                            // 53
			bool useFullPremltAlpha;                                                // 54
			bool alwaysRenderWhenEnabled;                                           // 55
			bool defRenderMainScreen;                                               // 56
			bool enableAO;                                                          // 57
			bool useLongRangeCamera;                                                // 58
			bool needsLightSetup;                                                   // 59
			bool needsLightSetupOffscreen;                                          // 5A
			std::uint8_t pad5B{ 0 };                                                // 5B
			stl::enumeration<BackgroundMode, std::uint32_t> bgMode;                 // 5C
			stl::enumeration<PostEffect, std::uint32_t> postFX;                     // 60
			stl::enumeration<HighlightEffect, std::uint32_t> highlightFX;           // 64
			stl::enumeration<OffscreenMenuSize, std::uint32_t> offscreenMenuSize;   // 68
			stl::enumeration<ScreenMode, std::uint32_t> screenMode;                 // 6C
			stl::enumeration<OffscreenMenuBlendMode, std::uint32_t> menuBlendMode;  // 70
			stl::enumeration<UI_DEPTH_PRIORITY, std::uint32_t> depth;               // 74
			NiPointer<NiAVObject> worldAttachedElementRoot;                         // 78
			NiPointer<NiAVObject> screenAttachedElementRoot;                        // 80
			NiPointer<NiAVObject> offscreenElement;                                 // 88
			BSTArray<NiPointer<BSGeometry>> displayGeometry;                        // 90
			NiAVObject* elementRequiringFullViewport;                               // A8
			NiAVObject* highlightedElement;                                         // B0
			bool highlightOffscreen;                                                // B8
			std::uint8_t padB9{ 0 };                                                // B9
			std::uint16_t padBA{ 0 };                                               // BA
			OldScreenEffectControl pipboyFX;                                        // BC
			OldScreenEffectControl vatsFX;                                          // 12C
			std::uint32_t pad19C{ 0 };                                              // 19C
			NiPointer<BSShaderAccumulator> accumulator;                             // 1A0
			NiPointer<NiCamera> camera_PipboyAspect;                                // 1A8
			NiPointer<NiCamera> camera_NativeAspect;                                // 1B0
			NiPointer<NiCamera> camera_NativeAspect_LongRange;                      // 1B8
			std::int32_t customRenderTarget;                                        // 1C0
			std::int32_t customSwapTarget;                                          // 1C4
			BSTArray<LightParams> mainLights;                                       // 1C8
			BSTArray<LightParams> offscreenLights;                                  // 1E0
			float menuDiffuseIntensity;                                             // 1F8
			float menuEmitIntensity;                                                // 1FC
			float menuDiffuseIntensityWA;                                           // 200
			float menuEmitIntensityWA;                                              // 204
			BSFixedString name;                                                     // 208
			BSFixedString screenGeomName;                                           // 210
			BSFixedString screenMaterialName;                                       // 218
			BSFixedString maskedGeomName;                                           // 220
			BSFixedString maskedMaterialName;                                       // 228
			NiDirectionalLight* directionalLight;                                   // 230
			NiPointer<ShadowSceneNode> screenSSN;                                   // 238
			NiPointer<ShadowSceneNode> offscreenSSN;                                // 240
		};
		static_assert(sizeof(Renderer) == 0x248);
	}
}

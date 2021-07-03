#pragma once

#include "RE/Bethesda/Atomic.h"
#include "RE/Bethesda/BGSCreatedObjectManager.h"
#include "RE/Bethesda/BGSInventoryInterface.h"
#include "RE/Bethesda/BGSInventoryItem.h"
#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSInputEventUser.h"
#include "RE/Bethesda/BSPointerHandle.h"
#include "RE/Bethesda/BSTArray.h"
#include "RE/Bethesda/BSTHashMap.h"
#include "RE/Bethesda/BSTInterpolator.h"
#include "RE/Bethesda/BSTOptional.h"
#include "RE/Bethesda/BSTSmartPointer.h"
#include "RE/Bethesda/BSTTuple.h"
#include "RE/Bethesda/Events.h"
#include "RE/Bethesda/InventoryUserUIUtils.h"
#include "RE/Bethesda/SendHUDMessage.h"
#include "RE/Bethesda/SWFToCodeFunctionHandler.h"
#include "RE/Bethesda/TESForms.h"
#include "RE/Bethesda/UIMessage.h"
#include "RE/Bethesda/UserEvents.h"
#include "RE/NetImmerse/NiColor.h"
#include "RE/NetImmerse/NiMatrix3.h"
#include "RE/NetImmerse/NiPoint2.h"
#include "RE/NetImmerse/NiPoint3.h"
#include "RE/NetImmerse/NiQuaternion.h"
#include "RE/NetImmerse/NiRect.h"
#include "RE/NetImmerse/NiSmartPointer.h"
#include "RE/NetImmerse/NiTexture.h"

namespace RE
{
	enum class ContainerMenuMode;

	namespace Workshop
	{
		struct BuildableAreaEvent;
		struct PlacementStatusEvent;
	}

	class BSGFxFunctionBase;
	class BSGFxShaderFXTarget;
	class ExtraDataList;
	class MessageBoxData;
	class NiAVObject;
	class TESBoundObject;
	class TESForm;
	class TESObjectREFR;
	class UserEventEnabledEvent;
	class WorkshopMenuGeometry;

	struct InvInterfaceStateChangeEvent;
	struct LoadedInventoryModel;
	struct PickRefUpdateEvent;

	enum class HUDColorTypes
	{
		kNoColorMultiplier = 0,
		kMenuNoColorBackground = 1,
		kGameplayHUDColor = 2,
		kPlayerSetColor = 3,
		kPowerArmorColorOnly = 4,
		kWarningColor = 5,
		kAltWarningColor = 6,
		kCustomColor = 7
	};

	enum class MENU_RENDER_CONTEXT : std::int32_t
	{
		kMenuDelete,
		kPreDisplay,
		kRenderScreenspace,
		kRenderCopyQuads,
		kRenderImagespace,
		kEnsureDisplayMenuCalled,
		kPostDisplay
	};

	enum class UI_MENU_FLAGS : std::uint32_t
	{
		kPausesGame = 1 << 0,
		kAlwaysOpen = 1 << 1,
		kUsesCursor = 1 << 2,
		kUsesMenuContext = 1 << 3,
		kModal = 1 << 4,
		kFreezeFrameBackground = 1 << 5,
		kOnStack = 1 << 6,
		kDisablePauseMenu = 1 << 7,
		kRequiresUpdate = 1 << 8,
		kTopmostRenderedMenu = 1 << 9,
		kUpdateUsesCursor = 1 << 10,
		kAllowSaving = 1 << 11,
		kRendersOffscreenTargets = 1 << 12,
		kInventoryItemMenu = 1 << 13,
		kDontHideCursorWhenTopmost = 1 << 14,
		kCustomRendering = 1 << 15,
		kAssignCursorToRenderer = 1 << 16,
		kApplicationMenu = 1 << 17,
		kHasButtonBar = 1 << 18,
		kIsTopButtonBar = 1 << 19,
		kAdvancesUnderPauseMenu = 1 << 20,
		kRendersUnderPauseMenu = 1 << 21,
		kUsesBlurredBackground = 1 << 22,
		kCompanionAppAllowed = 1 << 23,
		kFreezeFramePause = 1 << 24,
		kSkipRenderDuringFreezeFrameScreenshot = 1 << 25,
		kLargeScaleformRenderCacheMode = 1 << 26,
		kUsesMovementToDirection = 1 << 27
	};

	class IMenu :
		public SWFToCodeFunctionHandler,  // 00
		public BSInputEventUser           // 10
	{
	public:
		static constexpr auto RTTI{ RTTI::IMenu };
		static constexpr auto VTABLE{ VTABLE::IMenu };

		using SWFToCodeFunctionHandler::operator new;
		using SWFToCodeFunctionHandler::operator delete;

		// NOLINTNEXTLINE(modernize-use-override)
		virtual ~IMenu()  // 00
		{
			if (uiMovie) {
				const auto id = Scaleform::GetCurrentThreadId();
				uiMovie->SetCaptureThread(id);

				const auto heap = uiMovie->GetHeap();
				if (heap) {
					heap->AssignToCurrentThread();
				}
			}
		}

		// override (BSInputEventUser)
		bool ShouldHandleEvent(const InputEvent* a_event) override  // 01
		{
			using func_t = decltype(&IMenu::ShouldHandleEvent);
			REL::Relocation<func_t> func{ REL::ID(1241790) };
			return func(this, a_event);
		}

		void HandleEvent(const ButtonEvent* a_event) override  // 08
		{
			if (menuObj.IsObject()) {
				auto strUserEvent = a_event->QUserEvent();
				if (a_event->disabled && CanHandleWhenDisabled(a_event))
				{
					strUserEvent = a_event->QRawUserEvent();
				}

				if (inputEventHandlingEnabled && menuObj.HasMember("ProcessUserEvent"))
				{
					Scaleform::GFx::Value args[2];
					args[0] = strUserEvent.c_str();
					args[1] = a_event->QJustPressed();
					menuObj.Invoke("ProcessUserEvent", nullptr, args, 2);
				}
			}
		}

		// add
		virtual UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message)  // 03
		{
			switch (*a_message.type) {
			case UI_MESSAGE_TYPE::kShow:
			case UI_MESSAGE_TYPE::kHide:
				return UI_MESSAGE_RESULTS::kHandled;
			case UI_MESSAGE_TYPE::kScaleformEvent:
				return ProcessScaleformEvent(uiMovie.get(), a_message.QData());
			case UI_MESSAGE_TYPE::kUpdateController:
				RefreshPlatform();
				return UI_MESSAGE_RESULTS::kPassOn;
			default:
				return UI_MESSAGE_RESULTS::kPassOn;
			}
		}

		virtual void AdvanceMovie(float a_timeDelta, [[maybe_unused]] std::uint64_t a_time)  // 04
		{
			if (uiMovie) {
				DoAdvanceMovie(a_timeDelta);
				hasDoneFirstAdvanceMovie = true;
			}
		}

		virtual void PreDisplay() { return; }   // 05
		virtual void PostDisplay() { return; }  // 06

		virtual bool PassesRenderConditionText(MENU_RENDER_CONTEXT a_reason, const BSFixedString& a_customRendererName) const  // 07
		{
			using func_t = decltype(&IMenu::PassesRenderConditionText);
			REL::Relocation<func_t> func{ REL::ID(937304) };
			return func(this, a_reason, a_customRendererName);
		}

		virtual void SetIsTopButtonBar([[maybe_unused]] bool a_isTopButtonBar) { return; }  // 08

		virtual void OnMenuStackChanged(const BSFixedString& a_topMenuName, bool a_passesTopMenuTest)  // 09
		{
			const bool topMenuTest = a_passesTopMenuTest || (a_topMenuName == "PauseMenu"sv && RendersUnderPauseMenu());
			if (passesTopMenuTest != topMenuTest) {
				passesTopMenuTest = topMenuTest;
				OnMenuDisplayStateChanged();
			}
		}

		virtual void OnMenuDisplayStateChanged() { return; }  // 0A

		virtual void OnAddedToMenuStack()  // 0B
		{
			menuFlags.set(UI_MENU_FLAGS::kOnStack);
			OnMenuDisplayStateChanged();
		}

		virtual void OnRemovedFromMenuStack()  // 0C
		{
			menuFlags.reset(UI_MENU_FLAGS::kOnStack);
			OnMenuDisplayStateChanged();
		}

		virtual bool CanAdvanceMovie(bool a_pauseMenuShowing)  // 0D
		{
			return !a_pauseMenuShowing || depthPriority > 10 || AdvancesUnderPauseMenu();
		}

		virtual bool CanHandleWhenDisabled([[maybe_unused]] const ButtonEvent* a_event) { return false; }                      // 0E
		virtual bool OnButtonEventRelease([[maybe_unused]] const BSFixedString& a_eventName) { return false; }                 // 0F
		virtual bool CacheShaderFXQuadsForRenderer_Impl() { return false; }                                                    // 10
		virtual void TransferCachedShaderFXQuadsForRenderer([[maybe_unused]] const BSFixedString& a_rendererName) { return; }  // 11
		virtual void SetViewportRect([[maybe_unused]] const NiRect<float>& a_viewportRect) { return; }                         // 12

		[[nodiscard]] constexpr bool AdvancesUnderPauseMenu() const noexcept { return menuFlags.all(UI_MENU_FLAGS::kAdvancesUnderPauseMenu); }

		void DoAdvanceMovie(float a_timeDelta)
		{
			++advanceWithoutRenderCount;
			uiMovie->Advance(a_timeDelta);
		}

		void OnSetSafeRect()
		{
			using func_t = decltype(&IMenu::OnSetSafeRect);
			REL::Relocation<func_t> func{ REL::ID(964859) };
			return func(this);
		}

		[[nodiscard]] bool OnStack() const noexcept { return menuFlags.all(UI_MENU_FLAGS::kOnStack); }

		UI_MESSAGE_RESULTS ProcessScaleformEvent(Scaleform::GFx::Movie* a_movie, const IUIMessageData* a_data)
		{
			using func_t = decltype(&IMenu::ProcessScaleformEvent);
			REL::Relocation<func_t> func{ REL::ID(150211) };
			return func(this, a_movie, a_data);
		}

		void RefreshPlatform()
		{
			using func_t = decltype(&IMenu::RefreshPlatform);
			REL::Relocation<func_t> func{ REL::ID(1071829) };
			return func(this);
		}

		[[nodiscard]] constexpr bool RendersUnderPauseMenu() const noexcept { return menuFlags.all(UI_MENU_FLAGS::kRendersUnderPauseMenu); }

		[[nodiscard]] constexpr bool IsMenuDisplayEnabled() const noexcept { return passesTopMenuTest && menuCanBeVisible; }

		void SetMenuCodeObject(Scaleform::GFx::Movie& a_movie, stl::zstring a_menuObjPath)
		{
			a_movie.GetVariable(std::addressof(menuObj), a_menuObjPath.data());
			RegisterCodeObject(a_movie, menuObj);
		}

		void UpdateFlag(UI_MENU_FLAGS a_flag, bool a_set) noexcept
		{
			if (a_set) {
				menuFlags.set(a_flag);
			} else {
				menuFlags.reset(a_flag);
			}
		}

		// members
		Scaleform::GFx::Value menuObj;                                                                                     // 20
		Scaleform::Ptr<Scaleform::GFx::Movie> uiMovie;                                                                     // 40
		BSFixedString customRendererName;                                                                                  // 48
		BSFixedString menuName;                                                                                            // 50
		stl::enumeration<UI_MENU_FLAGS, std::uint32_t> menuFlags;                                                          // 58
		BSTAtomicValue<std::uint32_t> advanceWithoutRenderCount{ 0 };                                                      // 5C
		bool passesTopMenuTest{ true };                                                                                    // 60
		bool menuCanBeVisible{ true };                                                                                     // 61
		bool hasQuadsForCumstomRenderer{ false };                                                                          // 62
		bool hasDoneFirstAdvanceMovie{ false };                                                                            // 63
		std::uint8_t depthPriority{ 6 };                                                                                   // 64
		std::uint8_t pad65{ 0 };                                                                                           // 65
		std::uint16_t pad66{ 0 };                                                                                          // 66
		stl::enumeration<UserEvents::INPUT_CONTEXT_ID, std::int32_t> inputContext{ UserEvents::INPUT_CONTEXT_ID::kNone };  // 68
		std::uint32_t pad6C{ 0 };                                                                                          // 6C
	};
	static_assert(sizeof(IMenu) == 0x70);

	struct UIShaderColors
	{
	public:
		enum class Flags
		{
			kBackgroundQuad = 1u << 0,
			kColorMultiplier = 1u << 1,
			kVerticalGradient = 1u << 2,
			kUseAlphaForDropshadow = 1u << 3
		};

		// members
		NiRect<float> backgroundQuad;                          // 00
		NiColorA backgroundColor;                              // 10
		NiColorA colorMultipliers;                             // 20
		float colorBrightness;                                 // 30
		stl::enumeration<Flags, std::uint32_t> enabledStates;  // 34
	};
	static_assert(sizeof(UIShaderColors) == 0x38);

	struct alignas(0x10) UIShaderFXInfo
	{
	public:
		// members
		NiRect<float> renderQuad;  // 00
		UIShaderColors shaderFX;   // 10
	};
	static_assert(sizeof(UIShaderFXInfo) == 0x50);

	class HUDModeType
	{
	public:
		HUDModeType(const char* a_modeString) :
			modeString(a_modeString)
		{}

		// members
		BSFixedString modeString;  // 0
	};
	static_assert(sizeof(HUDModeType) == 0x8);

	class UsesBSGFXFunctionHandler
	{
	public:
		static constexpr auto RTTI{ RTTI::UsesBSGFXFunctionHandler };

		// members
		BSTArray<msvc::unique_ptr<BSGFxFunctionBase>> functions;  // 00
	};
	static_assert(sizeof(UsesBSGFXFunctionHandler) == 0x18);

	class BSGFxObject :
		public Scaleform::GFx::Value,    // 00
		public UsesBSGFXFunctionHandler  // 20
	{
	public:
		static constexpr auto RTTI{ RTTI::BSGFxObject };

		BSGFxObject(const Scaleform::GFx::Value& a_flashObject) :
			Scaleform::GFx::Value(a_flashObject)
		{}

		BSGFxObject(const Scaleform::GFx::Value& a_flashObject, const char* a_relativePathToMember)
		{
			AcquireFlashObjectByMemberName(a_flashObject, a_relativePathToMember);
		}

		BSGFxObject(const Scaleform::GFx::Movie& a_parentMovie, const char* a_pathToObject)
		{
			AcquireFlashObjectByPath(a_parentMovie, a_pathToObject);
		}

		Scaleform::GFx::Value* AcquireFlashObjectByMemberName(const Scaleform::GFx::Value& a_flashObject, const char* a_relativePathToMember)
		{
			using func_t = decltype(&BSGFxObject::AcquireFlashObjectByMemberName);
			REL::Relocation<func_t> func{ REL::ID(1172680) };
			return func(this, a_flashObject, a_relativePathToMember);
		}

		Scaleform::GFx::Value* AcquireFlashObjectByPath(const Scaleform::GFx::Movie& a_parentMovie, const char* a_absolutePathToMember)
		{
			using func_t = decltype(&BSGFxObject::AcquireFlashObjectByPath);
			REL::Relocation<func_t> func{ REL::ID(1065592) };
			return func(this, a_parentMovie, a_absolutePathToMember);
		}
	};
	static_assert(sizeof(BSGFxObject) == 0x38);

	class BSGFxDisplayObject :
		public BSGFxObject  // 00
	{
	public:
		static constexpr auto RTTI{ RTTI::BSGFxDisplayObject };
		static constexpr auto VTABLE{ VTABLE::BSGFxDisplayObject };

		struct InitialDisplayState
		{
		public:
			// members
			float originalWidth;   // 00
			float originalHeight;  // 04
		};
		static_assert(sizeof(InitialDisplayState) == 0x08);

		BSGFxDisplayObject(const Scaleform::GFx::Value& a_flashObject) :
			BSGFxObject(a_flashObject)
		{
			ctor_shared();
		}

		BSGFxDisplayObject(const Scaleform::GFx::Value& a_flashObject, const char* a_relativePathToMember) :
			BSGFxObject(a_flashObject, a_relativePathToMember)
		{
			ctor_shared();
		}

		BSGFxDisplayObject(const Scaleform::GFx::Movie& a_parentMovie, const char* a_pathToObject) :
			BSGFxObject(a_parentMovie, a_pathToObject)
		{
			ctor_shared();
		}

		virtual ~BSGFxDisplayObject()
		{
			if (parentDisplayObject) {
				parentDisplayObject->RemoveChild(*this);
			}
		}

		void RemoveChild(const BSGFxDisplayObject& a_child) const
		{
			using func_t = decltype(&BSGFxDisplayObject::RemoveChild);
			REL::Relocation<func_t> func{ REL::ID(1229383) };
			return func(this, a_child);
		}

		// members
		const BSGFxDisplayObject* parentDisplayObject{ nullptr };  // 40
		InitialDisplayState initialState;                          // 48

	private:
		void ctor_shared()
		{
			if (HasMember("height")) {
				Scaleform::GFx::Value height;
				if (GetMember("height", &height)) {
					if (height.IsNumber()) {
						initialState.originalHeight = static_cast<float>(height.GetNumber());
					}
				}
			}

			if (HasMember("width")) {
				Scaleform::GFx::Value width;
				if (GetMember("width", &width)) {
					if (width.IsNumber()) {
						initialState.originalWidth = static_cast<float>(width.GetNumber());
					}
				}
			}
		}
	};
	static_assert(sizeof(BSGFxDisplayObject) == 0x50);

	class BSGFxShaderFXTarget :
		public BSGFxDisplayObject,                  // 00
		public BSTEventSink<ApplyColorUpdateEvent>  // 50
	{
	public:
		static constexpr auto RTTI{ RTTI::BSGFxShaderFXTarget };
		static constexpr auto VTABLE{ VTABLE::BSGFxShaderFXTarget };

		BSGFxShaderFXTarget(const Scaleform::GFx::Value& a_flashObject) :
			BSGFxDisplayObject(a_flashObject)
		{
			ctor_shared();
		}

		BSGFxShaderFXTarget(const Scaleform::GFx::Value& a_flashObject, const char* a_relativePathToMember) :
			BSGFxDisplayObject(a_flashObject, a_relativePathToMember)
		{
			ctor_shared();
		}

		BSGFxShaderFXTarget(const Scaleform::GFx::Movie& a_parentMovie, const char* a_pathToObject) :
			BSGFxDisplayObject(a_parentMovie, a_pathToObject)
		{
			ctor_shared();
		}

		virtual ~BSGFxShaderFXTarget()
		{
			if (auto source = ApplyColorUpdateEvent::GetEventSource(); source) {
				source->UnregisterSink(this);
			}
		}

		// override
		virtual BSEventNotifyControl ProcessEvent(const ApplyColorUpdateEvent& a_event, BSTEventSource<ApplyColorUpdateEvent>* a_source) override  // 01
		{
			using func_t = decltype(&BSGFxShaderFXTarget::ProcessEvent);
			REL::Relocation<func_t> func{ REL::ID(848563) };
			return func(this, a_event, a_source);
		}

		// add
		virtual void AppendShaderFXInfos(BSTArray<UIShaderFXInfo>& a_colorFXInfo, BSTArray<UIShaderFXInfo>& a_backgroundFXInfo)  // 02
		{
			using func_t = decltype(&BSGFxShaderFXTarget::AppendShaderFXInfos);
			REL::Relocation<func_t> func{ REL::ID(544646) };
			return func(this, a_colorFXInfo, a_backgroundFXInfo);
		}

		void CreateAndSetFiltersToHUD(HUDColorTypes a_colorType, float a_scale = 1.0)
		{
			using func_t = decltype(&BSGFxShaderFXTarget::CreateAndSetFiltersToHUD);
			REL::Relocation<func_t> func{ REL::ID(876001) };
			func(this, a_colorType, a_scale);
		}

		void EnableShadedBackground(HUDColorTypes a_colorType, float a_scale = 1.0)
		{
			using func_t = decltype(&BSGFxShaderFXTarget::EnableShadedBackground);
			REL::Relocation<func_t> func{ REL::ID(278402) };
			func(this, a_colorType, a_scale);
		}

		void SetToHUDColor(bool a_useWarningColor)
		{
			auto colorType = (a_useWarningColor) ? RE::HUDColorTypes::kWarningColor : RE::HUDColorTypes::kGameplayHUDColor;
			CreateAndSetFiltersToHUD(colorType, 1.0);
		}

		// members
		UIShaderColors shaderFX;                                             // 58
		BSTArray<BSGFxShaderFXTarget*> shaderFXObjects;                      // 90
		stl::enumeration<HUDColorTypes, std::uint32_t> HUDColorType;         // A8
		stl::enumeration<HUDColorTypes, std::uint32_t> backgroundColorType;  // AC

	private:
		void ctor_shared()
		{
			if (HasMember("bUseShadedBackground")) {
				Scaleform::GFx::Value bUseShadedBackground;
				if (GetMember("bUseShadedBackground", &bUseShadedBackground)) {
					if (bUseShadedBackground.IsBoolean()) {
						if (bUseShadedBackground.GetBoolean()) {
							EnableShadedBackground(HUDColorTypes::kMenuNoColorBackground);
						}
					}
				}
			}

			if (auto source = ApplyColorUpdateEvent::GetEventSource(); source) {
				source->RegisterSink(this);
			}
		}
	};
	static_assert(sizeof(BSGFxShaderFXTarget) == 0xB0);

	class __declspec(novtable) ButtonHintBar :
		public BSGFxShaderFXTarget  // 00
	{
	public:
		static constexpr auto RTTI{ RTTI::ButtonHintBar };
		static constexpr auto VTABLE{ VTABLE::ButtonHintBar };

		// members
		Scaleform::GFx::Value sourceButtons;  // B0
		bool redirectToButtonBarMenu;         // D0
		bool isTopButtonBar;                  // D1
	};
	static_assert(sizeof(ButtonHintBar) == 0xD8);

	class GameMenuBase :
		public IMenu  // 00
	{
	public:
		static constexpr auto RTTI{ RTTI::GameMenuBase };
		static constexpr auto VTABLE{ VTABLE::GameMenuBase };

		GameMenuBase()
		{
			customRendererName = "FlatScreenModel";
		}

		virtual ~GameMenuBase() = default;

		// override
		virtual void SetIsTopButtonBar(bool a_isTopButtonBar) override  // 08
		{
			using func_t = decltype(&GameMenuBase::SetIsTopButtonBar);
			REL::Relocation<func_t> func{ REL::ID(1367353) };
			return func(this, a_isTopButtonBar);
		}

		virtual void OnMenuDisplayStateChanged() override  // 0A
		{
			using func_t = decltype(&GameMenuBase::OnMenuDisplayStateChanged);
			REL::Relocation<func_t> func{ REL::ID(1274450) };
			return func(this);
		}

		virtual void OnAddedToMenuStack() override  // 0B
		{
			IMenu::OnAddedToMenuStack();
			if (this->menuHUDMode.has_value()) {
				SendHUDMessage::PushHUDMode(this->menuHUDMode.value());
			}
		}

		virtual void OnRemovedFromMenuStack() override  // 0C
		{
			IMenu::OnRemovedFromMenuStack();
			if (this->menuHUDMode.has_value()) {
				SendHUDMessage::PopHUDMode(this->menuHUDMode.value());
			}
		}

		virtual bool CacheShaderFXQuadsForRenderer_Impl() override  // 10
		{
			using func_t = decltype(&GameMenuBase::CacheShaderFXQuadsForRenderer_Impl);
			REL::Relocation<func_t> func{ REL::ID(863029) };
			return func(this);
		}

		virtual void TransferCachedShaderFXQuadsForRenderer(const BSFixedString& a_rendererName) override  // 11
		{
			using func_t = decltype(&GameMenuBase::TransferCachedShaderFXQuadsForRenderer);
			REL::Relocation<func_t> func{ REL::ID(65166) };
			return func(this, a_rendererName);
		}

		virtual void SetViewportRect(const NiRect<float>& a_viewportRect) override  // 12
		{
			using func_t = decltype(&GameMenuBase::SetViewportRect);
			REL::Relocation<func_t> func{ REL::ID(1554334) };
			return func(this, a_viewportRect);
		}

		// add
		virtual void AppendShaderFXInfos(BSTAlignedArray<UIShaderFXInfo>& a_colorFXInfos, BSTAlignedArray<UIShaderFXInfo>& a_backgroundFXInfos) const  // 13
		{
			using func_t = decltype(&GameMenuBase::AppendShaderFXInfos);
			REL::Relocation<func_t> func{ REL::ID(583584) };
			return func(this, a_colorFXInfos, a_backgroundFXInfos);
		}

		void SetUpButtonBar(BSGFxShaderFXTarget& a_parentObject, const char* a_buttonBarPath, HUDColorTypes a_colorType)
		{
			using func_t = decltype(&GameMenuBase::SetUpButtonBar);
			REL::Relocation<func_t> func{ REL::ID(531584) };
			func(this, a_parentObject, a_buttonBarPath, a_colorType);
		}

		// members
		BSTArray<BSGFxShaderFXTarget*> shaderFXObjects;           // 70
		msvc::unique_ptr<BSGFxShaderFXTarget> filterHolder;       // 88
		msvc::unique_ptr<ButtonHintBar> buttonHintBar;            // 90
		BSTAlignedArray<UIShaderFXInfo> cachedColorFXInfos;       // 98
		BSTAlignedArray<UIShaderFXInfo> cachedBackgroundFXInfos;  // B0
		BSReadWriteLock cachedQuadsLock;                          // C8
		BSTOptional<HUDModeType> menuHUDMode;                     // D0
	};
	static_assert(sizeof(GameMenuBase) == 0xE0);

	struct Rumble
	{
	public:
		struct AutoRumblePause
		{
		public:
		};
		static_assert(std::is_empty_v<AutoRumblePause>);
	};
	static_assert(std::is_empty_v<Rumble>);

	struct __declspec(novtable) Console :
		public GameMenuBase  // 00
	{
	public:
		static constexpr auto RTTI{ RTTI::Console };
		static constexpr auto VTABLE{ VTABLE::Console };
		static constexpr auto MENU_NAME{ "Console"sv };

		[[nodiscard]] static decltype(auto) GetCurrentPickIndex()
		{
			REL::Relocation<std::int32_t*> currentPickIndex{ REL::ID(1407033) };
			return *currentPickIndex;
		}

		[[nodiscard]] static decltype(auto) GetPickRef()
		{
			REL::Relocation<ObjectRefHandle*> ref{ REL::ID(170742) };
			return *ref;
		}

		[[nodiscard]] static decltype(auto) GetPickRefs()
		{
			REL::Relocation<BSTArray<ObjectRefHandle>*> pickRefs{ REL::ID(875116) };
			return *pickRefs;
		}

		[[nodiscard]] static ObjectRefHandle GetCurrentPickREFR()
		{
			const auto idx = GetCurrentPickIndex();
			const auto& refs = GetPickRefs();
			return 0 <= idx && static_cast<std::size_t>(idx) < refs.size() ?
			           refs[static_cast<std::size_t>(idx)] :
                       ObjectRefHandle{};
		}

		void SetCurrentPickREFR(stl::not_null<ObjectRefHandle*> a_refr)
		{
			using func_t = decltype(&Console::SetCurrentPickREFR);
			REL::Relocation<func_t> func{ REL::ID(79066) };
			return func(this, a_refr);
		}

		// members
		Rumble::AutoRumblePause* rumbleLock;  // E0
		bool minimized;                       // E8
	};
	static_assert(sizeof(Console) == 0xF0);

	struct BaseLoadedInventoryModel
	{
	public:
		// members
		CreatedObjPtr<TESForm> itemBase;  // 00
		TESBoundObject* modelObj;         // 08
		NiPointer<NiAVObject> model;      // 10
		NiPoint2 panMinima;               // 18
		NiPoint2 panMaxima;               // 20
		float initialDistance;            // 28
		float boundRadius;                // 2C
		float horizontalBound;            // 30
		float verticalBound;              // 34
		float verticalBoundOffset;        // 38
	};
	static_assert(sizeof(BaseLoadedInventoryModel) == 0x40);

	struct DisplayItemModel :
		public BaseLoadedInventoryModel  // 00
	{
	public:
		// members
		float itemRotation;         // 40
		std::uint32_t uniqueIndex;  // 44
		std::uint16_t column;       // 48
		std::uint16_t index;        // 4A
		std::uint16_t row;          // 4C
	};
	static_assert(sizeof(DisplayItemModel) == 0x50);

	namespace nsInventory3DManager
	{
		class NewInventoryMenuItemLoadTask;
	}

	class __declspec(novtable) Inventory3DManager :
		public BSInputEventUser  // 000
	{
	public:
		static constexpr auto RTTI{ RTTI::Inventory3DManager };
		static constexpr auto VTABLE{ VTABLE::Inventory3DManager };

		void ClearModel()
		{
			using func_t = decltype(&Inventory3DManager::ClearModel);
			REL::Relocation<func_t> func{ REL::ID(63218) };
			return func(this);
		}

		// members
		bool useBoundForScale: 1;                                                // 010:0
		bool startedZoomThisFrame: 1;                                            // 010:1
		bool useStoredModelPosition: 1;                                          // 010:2
		bool rotating: 1;                                                        // 010:3
		bool modelPositionInScreenCoords: 1;                                     // 010:4
		bool centerOnBoundCenter: 1;                                             // 010:5
		NiPoint3 modelPosition;                                                  // 014
		float modelScale;                                                        // 020
		alignas(0x10) BSTArray<LoadedInventoryModel> loadedModels;               // 030
		NiPoint3 initialPosition;                                                // 048
		NiPoint3 storedPostion;                                                  // 054
		NiMatrix3 initialRotation;                                               // 060
		NiQuaternion storedRotation;                                             // 090
		NiPoint2 previousInput;                                                  // 0A0
		NiPointer<nsInventory3DManager::NewInventoryMenuItemLoadTask> loadTask;  // 0A8
		TESObjectREFR* tempRef;                                                  // 0B0
		BSTSmartPointer<ExtraDataList> originalExtra;                            // 0B8
		BSFixedString str3DRendererName;                                         // 0C0
		BGSInventoryItem queuedDisplayItem;                                      // 0C8
		std::uint32_t itemExtraIndex;                                            // 0D8
		TESForm* itemBase;                                                       // 0E0
		std::int8_t disableInputUserCount;                                       // 0E8
		BSTSet<BSFixedString> disableRendererUsers;                              // 0F0
		float storedXRotation;                                                   // 120
		float zoomDirection;                                                     // 124
		float zoomProgress;                                                      // 128
		float minZoomModifier;                                                   // 12C
		float maxZoomModifier;                                                   // 130
		std::uint32_t hightlightedPart;                                          // 134
		bool queueShowItem;                                                      // 138
		bool mouseRotation;                                                      // 139
		bool prevUsesCursorFlag;                                                 // 13A
		bool prevUpdateUsesCursorFlag;                                           // 13B
		bool addedLightsToScene;                                                 // 13C
	};
	static_assert(sizeof(Inventory3DManager) == 0x140);

	class __declspec(novtable) WorkshopMenu :
		public GameMenuBase,                                 // 000
		public BSTEventSink<UserEventEnabledEvent>,          // 0E0
		public BSTEventSink<Workshop::BuildableAreaEvent>,   // 0E8
		public BSTEventSink<PickRefUpdateEvent>,             // 0F0
		public BSTEventSink<Workshop::PlacementStatusEvent>  // 0F8
	{
	public:
		static constexpr auto RTTI{ RTTI::WorkshopMenu };
		static constexpr auto VTABLE{ VTABLE::WorkshopMenu };
		static constexpr auto MENU_NAME{ "WorkshopMenu"sv };

		class FXWorkshopMenu;

		struct IconBG
		{
		public:
			BSTAlignedArray<UIShaderFXInfo> cachedColorFXInfos;       // 00
			BSTAlignedArray<UIShaderFXInfo> cachedBackgroundFXInfos;  // 18
			BSReadWriteLock cachedQuadsLock;                          // 30
		};
		static_assert(sizeof(IconBG) == 0x38);

		// members
		BSTArray<NiPoint3> item3DPositions[4];                                                                          // 100
		BSTArray<BSTTuple<DisplayItemModel, TESObjectREFR*>> displayItemModels;                                         // 160
		IconBG iconBG;                                                                                                  // 178
		Inventory3DManager inv3DModelManager;                                                                           // 1B0
		BSTArray<BSTTuple<NiPointer<nsInventory3DManager::NewInventoryMenuItemLoadTask>, NiPoint3>> loadTasks;          // 2F0
		BSTInterpolator<float, EaseOutInterpolator, GetCurrentPositionFunctor> upDownGlassAnimationInterpolator;        // 308
		BSTInterpolator<float, EaseOutInterpolator, GetCurrentPositionFunctor> leftRightGlassAnimationInterpolator[4];  // 320
		BSTSmartPointer<WorkshopMenuGeometry> displayGeometry;                                                          // 380
		BSFixedString dpadInput;                                                                                        // 388
		BGSListForm includeList;                                                                                        // 390
		BGSListForm excludeList;                                                                                        // 3D8
		long double lastBudget;                                                                                         // 420
		std::uint16_t topMenuCount;                                                                                     // 428
		bool inputAdjustMode;                                                                                           // 42A
		bool verticalAdjustment;                                                                                        // 42B
		bool disableAdjustOnThumbEvent;                                                                                 // 42C
		bool initialized;                                                                                               // 42D
		bool inEditMode;                                                                                                // 42E
		bool electricalDevice;                                                                                          // 42F
		bool useMovementAsDirectional;                                                                                  // 430
		bool motionBlurActive;                                                                                          // 431
		bool exitDebounce;                                                                                              // 432
		msvc::unique_ptr<FXWorkshopMenu> workshopMenuBase;                                                              // 438
	};
	static_assert(sizeof(WorkshopMenu) == 0x440);

	struct DisableHeavyItemsFunc
	{
	public:
		// members
		float playerCurrEncumbrance;     // 00
		float playerMaxEncumbrance;      // 04
		float containerCurrEncumbrance;  // 08
		float containerMaxEncumbrance;   // 0C
	};
	static_assert(sizeof(DisableHeavyItemsFunc) == 0x10);

	struct InventoryUserUIInterfaceEntry
	{
	public:
		// members
		InventoryInterface::Handle invHandle;       // 00
		BSTSmallArray<std::uint8_t, 4> stackIndex;  // 08
	};
	static_assert(sizeof(InventoryUserUIInterfaceEntry) == 0x20);

	class __declspec(novtable) InventoryUserUIInterface :
		public BSTEventSource<InvInterfaceStateChangeEvent>  // 00
	{
	public:
		// members
		ObjectRefHandle inventoryRef;                            // 58
		BSTArray<InventoryUserUIInterfaceEntry> stackedEntries;  // 60
		bool entriesInvalid;                                     // 78
	};
	static_assert(sizeof(InventoryUserUIInterface) == 0x80);

	class __declspec(novtable) ContainerMenuBase :
		public GameMenuBase,                                // 000
		public BSTEventSink<InvInterfaceStateChangeEvent>,  // 0E0
		public BSTEventSink<MenuOpenCloseEvent>             // 0E8
	{
	public:
		static constexpr auto RTTI{ RTTI::ContainerMenuBase };
		static constexpr auto VTABLE{ VTABLE::ContainerMenuBase };

		class __declspec(novtable) FXQuantityMenu :
			public BSGFxShaderFXTarget  // 000
		{
		public:
			// members
			BSGFxShaderFXTarget label;                  // 0B0
			BSGFxShaderFXTarget value;                  // 160
			BSGFxShaderFXTarget quantityBracketHolder;  // 210
		};
		static_assert(sizeof(FXQuantityMenu) == 0x2C0);

		struct ItemSorter
		{
		public:
			enum class SORT_ON_FIELD
			{
				kAlphabetical = 0,
				kDamage = 1,
				kRateOfFire = 2,
				kRange = 3,
				kAccuracy = 4,
				kValue = 5,
				kWeight = 6,
			};

			void IncrementSort()
			{
				using func_t = decltype(&ItemSorter::IncrementSort);
				REL::Relocation<func_t> func{ REL::ID(1307263) };
				return func(this);
			}

			void SetTab(std::uint32_t a_tab)
			{
				currentTab = a_tab;
			}

			// members
			stl::enumeration<SORT_ON_FIELD, std::uint32_t> currentSort[14];  // 00
			std::uint32_t currentTab;                                        // 38
		};
		static_assert(sizeof(ItemSorter) == 0x3C);

		// override
		virtual void Call(const Params&) override;                                    // 01
		virtual void MapCodeObjectFunctions() override;                               // 02
		virtual void AdvanceMovie(float a_timeDelta, std::uint64_t a_time) override;  // 04
		virtual void PreDisplay() override;                                           // 05

		// add
		virtual void ConfirmInvestment() { return; }                                                                                                                          // 14
		virtual void DoItemTransfer(std::uint32_t a_itemIndex, std::uint32_t a_count, bool a_fromContainer) = 0;                                                              // 15
		virtual bool GetDisplayBarterValues() { return false; }                                                                                                               // 16
		virtual bool GetCanEquipItem([[maybe_unused]] std::uint32_t a_itemIndex, [[maybe_unused]] bool a_inContainer) { return false; }                                       // 17
		virtual bool GetIsItemEquipped([[maybe_unused]] std::uint32_t a_itemIndex, [[maybe_unused]] bool a_inContainer) { return false; }                                     // 18
		virtual void ToggleItemEquipped([[maybe_unused]] std::uint32_t a_itemIndex, [[maybe_unused]] bool a_inContainer) { return; }                                          // 19
		virtual std::uint32_t GetItemValue(std::uint32_t a_itemIndex, bool a_inContainer);                                                                                    // 1A
		virtual const InventoryUserUIInterfaceEntry* GetInventoryItemByListIndex(bool a_inContainer, std::uint32_t a_index);                                                  // 1B
		virtual void PopulateMenuObj(ObjectRefHandle a_inventoryRef, const InventoryUserUIInterfaceEntry& a_entry, Scaleform::GFx::Value& a_menuObj) = 0;                     // 1C
		virtual void SetMenuSuppressed(bool a_suppressed);                                                                                                                    // 1D
		virtual void UpdateEncumbranceAndCaps(bool a_inContainer, std::int32_t a_capsDifferential);                                                                           // 1E
		virtual void UpdateItemPickpocketInfo([[maybe_unused]] std::int32_t a_index, [[maybe_unused]] bool a_inContainer, [[maybe_unused]] std::int32_t a_count) { return; }  // 1F
		virtual void UpdateList(bool a_inContainer) = 0;                                                                                                                      // 20

		void SetMessageBoxMode(bool a_messageBoxMode)
		{
			if (menuObj.IsObject() && menuObj.HasMember("messageBoxIsActive"sv))
			{
				menuObj.SetMember("messageBoxIsActive"sv, a_messageBoxMode);
			}
		}

		// members
		ItemSorter containerItemSorter;                                       // 0F0
		ItemSorter playerItemSorter;                                          // 12C
		msvc::unique_ptr<BSGFxShaderFXTarget> playerBracketBackground_mc;     // 168
		msvc::unique_ptr<BSGFxShaderFXTarget> containerBracketBackground_mc;  // 170
		msvc::unique_ptr<BSGFxShaderFXTarget> containerList_mc;               // 178
		msvc::unique_ptr<BSGFxShaderFXTarget> playerInventory_mc;             // 180
		msvc::unique_ptr<BSGFxShaderFXTarget> containerInventory_mc;          // 188
		msvc::unique_ptr<BSGFxShaderFXTarget> itemCard_mc;                    // 190
		msvc::unique_ptr<FXQuantityMenu> quantityMenu_mc;                     // 198
		InventoryUserUIInterface playerInv;                                   // 1A0
		InventoryUserUIInterface containerInv;                                // 220
		Inventory3DManager inv3DModelManager;                                 // 2A0
		BSTArray<const TESBoundObject*> partialPlayerUpdateList;              // 3E0
		BSTArray<const TESBoundObject*> partialContainerUpdateList;           // 3F8
		stl::enumeration<ContainerMenuMode, std::uint32_t> menuMode;          // 410
		Rumble::AutoRumblePause autoRumblePause;                              // 414
		DisableHeavyItemsFunc disableHeavyFunc;                               // 418
		ObjectRefHandle containerRef;                                         // 428
		bool suppressed;                                                      // 42C
		bool menuOpening;                                                     // 42D
	};
	static_assert(sizeof(ContainerMenuBase) == 0x430);

	class __declspec(novtable) BarterMenuTentativeInventoryUIInterface :
		public InventoryUserUIInterface  // 00
	{
	public:
	};
	static_assert(sizeof(BarterMenuTentativeInventoryUIInterface) == 0x80);

	class __declspec(novtable) BarterMenu :
		public ContainerMenuBase  // 000
	{
	public:
		static constexpr auto RTTI{ RTTI::BarterMenu };
		static constexpr auto VTABLE{ VTABLE::BarterMenu };
		static constexpr auto MENU_NAME{ "BarterMenu"sv };

		struct ItemBarterData
		{
		public:
			// members
			BSTHashMap<std::uint32_t, std::int32_t> stackQuantityMap;  // 00
			std::uint32_t capsOwedByPlayer;                            // 30
		};
		static_assert(sizeof(ItemBarterData) == 0x38);

		// override
		virtual UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;                                                                               // 03
		virtual bool OnButtonEventRelease(const BSFixedString& a_eventName) override;                                                                           // 0F
		virtual void ConfirmInvestment() override;                                                                                                              // 14
		virtual void DoItemTransfer(std::uint32_t a_itemIndex, std::uint32_t a_count, bool a_fromContainer) override;                                           // 15
		virtual bool GetDisplayBarterValues() override;                                                                                                         // 16
		virtual std::uint32_t GetItemValue(std::uint32_t a_itemIndex, bool a_inContainer) override;                                                             // 1A
		virtual const InventoryUserUIInterfaceEntry* GetInventoryItemByListIndex(bool a_inContainer, std::uint32_t a_index) override;                           // 1B
		virtual void PopulateMenuObj(ObjectRefHandle a_inventoryRef, const InventoryUserUIInterfaceEntry& a_entry, Scaleform::GFx::Value& a_menuObj) override;  // 1C
		virtual void SetMenuSuppressed(bool a_suppressed) override;                                                                                             // 1D
		virtual void UpdateEncumbranceAndCaps(bool a_inContainer, std::int32_t a_capsDifferential) override;                                                    // 1E
		virtual void UpdateList(bool a_inContainer) override;                                                                                                   // 20

		void ClearTradingData()
		{
			using func_t = decltype(&BarterMenu::ClearTradingData);
			REL::Relocation<func_t> func{ REL::ID(1112285) };
			return func(this);
		}

		void CompleteTrade()
		{
			using func_t = decltype(&BarterMenu::CompleteTrade);
			REL::Relocation<func_t> func{ REL::ID(379932) };
			return func(this);
		}

		[[nodiscard]] std::int64_t GetCapsOwedByPlayer()
		{
			using func_t = decltype(&BarterMenu::GetCapsOwedByPlayer);
			REL::Relocation<func_t> func{ REL::ID(672405) };
			return func(this);
		}

		// members
		BSTHashMap<InventoryInterface::Handle, ItemBarterData*> barteredItems;  // 430
		std::unique_ptr<BSGFxShaderFXTarget> capsTransferInfo_mc;               // 460
		std::unique_ptr<BSGFxShaderFXTarget> capsTransferBackground_mc;         // 468
		ObjectRefHandle vendorChestRef;                                         // 470
		ObjectRefHandle vendorActor;                                            // 474
		BarterMenuTentativeInventoryUIInterface playerTentativeInv;             // 478
		BarterMenuTentativeInventoryUIInterface containerTentativeInv;          // 4F8
		bool confirmingTrade;                                                   // 578
	};
	static_assert(sizeof(BarterMenu) == 0x580);

	class __declspec(novtable) ContainerMenu :
		public ContainerMenuBase  // 000
	{
	public:
		static constexpr auto RTTI{ RTTI::ContainerMenu };
		static constexpr auto VTABLE{ VTABLE::ContainerMenu };
		static constexpr auto MENU_NAME{ "ContainerMenu"sv };

		// override
		virtual UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;                                                                               // 03
		virtual void DoItemTransfer(std::uint32_t a_itemIndex, std::uint32_t a_count, bool a_fromContainer) override;                                           // 15
		virtual bool GetCanEquipItem(std::uint32_t a_itemIndex, bool a_inContainer) override;                                                                   // 17
		virtual bool GetIsItemEquipped(std::uint32_t a_itemIndex, bool a_inContainer) override;                                                                 // 18
		virtual void ToggleItemEquipped(std::uint32_t a_itemIndex, bool a_inContainer) override;                                                                // 19
		virtual void PopulateMenuObj(ObjectRefHandle a_inventoryRef, const InventoryUserUIInterfaceEntry& a_entry, Scaleform::GFx::Value& a_menuObj) override;  // 22
		virtual void UpdateItemPickpocketInfo(std::int32_t a_index, bool a_inContainer, std::int32_t a_count) override;                                         // 25
		virtual void UpdateList(bool a_inContainer) override;                                                                                                   // 26

		void TakeAllItems()
		{
			using func_t = decltype(&ContainerMenu::TakeAllItems);
			REL::Relocation<func_t> func{ REL::ID(1323703) };
			return func(this);
		}

		// members
		std::unique_ptr<BSGFxShaderFXTarget> pickpocketInfo_mc;  // 430
		std::uint32_t valueStolenFromContainer;                  // 438
		bool containerAccessed;                                  // 43C
		bool addedTempItems;                                     // 43D
		bool plantedExplosiveWeapon;                             // 43E
		bool containerIsAnimatingOpen;                           // 43F
	};
	static_assert(sizeof(ContainerMenu) == 0x440);

	class _declspec(novtable) MessageBoxMenu :
		public GameMenuBase,                      // 00
		public BSTEventSink<MenuModeChangeEvent>  // E0
	{
	public:
		static constexpr auto RTTI{ RTTI::MessageBoxMenu };
		static constexpr auto VTABLE{ VTABLE::MessageBoxMenu };
		static constexpr auto MENU_NAME{ "MessageBoxMenu"sv };

		// override
		virtual void Call(const Params&) override;                           // 01
		virtual void MapCodeObjectFunctions() override;                      // 02
		virtual UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage&) override;  // 03

		void ShowMessage()
		{
			using func_t = decltype(&MessageBoxMenu::ShowMessage);
			REL::Relocation<func_t> func{ REL::ID(442479) };
			return func(this);
		}

		// members
		MessageBoxData* currentMessage;  // E8
	};
	static_assert(sizeof(MessageBoxMenu) == 0xF0);
}

#pragma once

namespace RE
{
	class __declspec(novtable) MenuTopicManager :
		public BSTEventSink<MenuOpenCloseEvent>,	// 00
		public BSTEventSink<PositionPlayerEvent>,	// 08
		public BSTSingletonSDM<MenuTopicManager>	// 10
	{
	public:
		static constexpr auto RTTI{ RTTI::MenuTopicManager };
		static constexpr auto VTABLE{ VTABLE::MenuTopicManager };

		[[nodiscard]] static MenuTopicManager* GetSingleton()
		{

		}
	};
}

#pragma once

namespace RE::Scaleform::GFx
{
	class ASStringManager;

	class ASStringNode
	{
	public:
		// members
		const char* data;          // 00
		ASStringManager* manager;  // 08
		union
		{
			ASStringNode* lower;
			ASStringNode* next;
		};                        // 10
		std::uint32_t refCount;   // 18
		std::uint32_t hashFlags;  // 1C
		std::uint32_t size;       // 20
	};
	static_assert(sizeof(ASStringNode) == 0x28);

	class ASStringNodeHolder
	{
	public:
		// members
		ASStringNode* node;  // 00
	};
	static_assert(sizeof(ASStringNodeHolder) == 0x08);

	class ASConstString :
		public ASStringNodeHolder
	{
	public:
		const char* ToCStr() const noexcept
		{
			return node->data;
		}
	};
	static_assert(sizeof(ASConstString) == 0x08);

	class ASString :
		public ASConstString
	{
	public:
	};
	static_assert(sizeof(ASString) == 0x08);
}
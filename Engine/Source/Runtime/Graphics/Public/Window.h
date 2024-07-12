#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	typedef void (*PF_WINDOWEVENTHANDLER)();

	struct WindowProperties
	{
		const wchar_t* pTitle;
		U32 width;
		U32 height;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void SetEventHandler(PF_WINDOWEVENTHANDLER pfn) = 0;
		virtual void PollWindowEvents() = 0;
		virtual WindowProperties GetProperties() const = 0;
		virtual void Destroy() = 0;

		static Window* New(WindowProperties properties);
	};
}
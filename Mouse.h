#pragma once
#include <queue>


class Mouse
{
	friend class Window;
public:
	
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave
		};

		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			bLPressed(parent.bLPressed),
			bRPressed(parent.bRPressed),
			bMPressed(parent.bMPressed),
			xPos(parent.xPos),
			yPos(parent.yPos)
		{}
	private:
		Type type;
		bool bLPressed;
		bool bRPressed;
		bool bMPressed;
		int xPos;
		int yPos;
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;
	
	int xPos{0};
	int yPos{0};
	int wheelDelta{0};
	bool bLPressed{false};
	bool bRPressed{false};
	bool bMPressed{false};
	std::queue<Event> eventBuffer;
	static constexpr unsigned int bufferSize = 16u;

	
};
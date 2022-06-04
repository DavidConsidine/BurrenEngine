#include <Windows.h>
#include "Mouse.h"

void Mouse::OnMouseMove(int x, int y) noexcept
{
	this->xPos = x;
	this->yPos = y;
	eventBuffer.push(Mouse::Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
}

void Mouse::OnMouseEnter() noexcept
{
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bLPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bLPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bRPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bRPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bMPressed = true;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int x, int y) noexcept
{
	xPos = x;
	yPos = y;
	bMPressed = false;
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp() noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown() noexcept
{
	eventBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	xPos = x;
	yPos = y;
	wheelDelta += delta;
	if (wheelDelta >= WHEEL_DELTA)
	{
		wheelDelta -= WHEEL_DELTA;
		OnWheelUp();
	}
	else if (wheelDelta <= -WHEEL_DELTA)
	{
		wheelDelta += WHEEL_DELTA;
		OnWheelDown();
	}
}

void Mouse::TrimBuffer() noexcept
{
	while (eventBuffer.size() > bufferSize)
	{
		eventBuffer.pop();
	}
}

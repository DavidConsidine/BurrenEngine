#pragma once
#include <bitset>
#include <queue>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release
		};
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}

		bool IsPressed() const noexcept
		{
			return type == Type::Press;
		}

		bool IsReleased() const noexcept
		{
			return type == Type::Release;
		}

		unsigned char GetCode() const noexcept
		{
			return code;
		}

	private:
		Type type;
		unsigned char code;
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool IsAutoRepeatEnabled() const noexcept;
	void SetAutoRepeatEnabled(bool isEnabled) noexcept;

private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
	void ClearState() noexcept;

	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

#pragma once
#include "KeyboardEvent.h"
#include <queue>

namespace s3d { namespace input {
	class Keyboard {
	public:
		Keyboard();

		bool IsKeyPressed(unsigned char keycode);
		bool IsKeyBufferEmpty() const;
		bool IsCharBufferEmpty() const;

		KeyboardEvent ReadKey();
		unsigned char ReadChar();

		void OnKeyPressed(unsigned char key);
		void OnKeyReleased(unsigned char key);
		void OnChar(unsigned char key);

		bool IsKeysAutoRepeat() const;
		void EnableAutoRepeatKeys();
		void DisableAutoRepeatKeys();

		bool IsCharsAutoRepeat() const;
		void EnableAutoRepeatChars();
		void DisableAutoRepeatChars();

	private:
		bool _autoRepeatKeys = false;
		bool _autoRepeatChars = false;
		bool _keyStates[256]{};
		std::queue<KeyboardEvent> _keyBuffer;
		std::queue<unsigned char> _charBuffer;
	};
} }
#include "Keyboard.h"

namespace s3d {
	namespace input
	{
		Keyboard::Keyboard()
		{
			for (auto& keyState : this->_keyStates)
				keyState = false;
		}

		bool Keyboard::IsKeyPressed(const unsigned char keycode)
		{
			return this->_keyStates[keycode];
		}

		bool Keyboard::IsKeyBufferEmpty() const
		{
			return this->_keyBuffer.empty();
		}

		bool Keyboard::IsCharBufferEmpty() const
		{
			return this->_charBuffer.empty();
		}

		KeyboardEvent Keyboard::ReadKey()
		{
			if (this->IsKeyBufferEmpty())
			{
				return {};
			}
			const auto e = this->_keyBuffer.front();
			this->_keyBuffer.pop();
			return e;
		}

		unsigned char Keyboard::ReadChar()
		{
			if (this->IsCharBufferEmpty())
			{
				return 0u;
			}
			const auto e = this->_charBuffer.front();
			this->_charBuffer.pop();
			return e;
		}

		void Keyboard::OnKeyPressed(const unsigned char key)
		{
			this->_keyStates[key] = true;
			this->_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
		}

		void Keyboard::OnKeyReleased(const unsigned char key)
		{
			this->_keyStates[key] = false;
			this->_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
		}

		void Keyboard::OnChar(const unsigned char key)
		{
			this->_charBuffer.push(key);
		}

		bool Keyboard::IsKeysAutoRepeat() const
		{
			return this->_autoRepeatKeys;
		}

		void Keyboard::EnableAutoRepeatKeys()
		{
			this->_autoRepeatKeys = true;
		}

		void Keyboard::DisableAutoRepeatKeys()
		{
			this->_autoRepeatKeys = false;
		}

		bool Keyboard::IsCharsAutoRepeat() const
		{
			return this->_autoRepeatChars;
		}

		void Keyboard::EnableAutoRepeatChars()
		{
			this->_autoRepeatChars = true;
		}

		void Keyboard::DisableAutoRepeatChars()
		{
			this->_autoRepeatChars = false;
		}
	}
}	
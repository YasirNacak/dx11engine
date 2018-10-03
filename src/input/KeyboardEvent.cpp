#include "KeyboardEvent.h"

namespace s3d {
	namespace input
	{
		KeyboardEvent::KeyboardEvent() :
			_type(Invalid), _key(0u)
		{
		}

		KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) :
			_type(type), _key(key)
		{
		}

		unsigned char KeyboardEvent::GetKeyCode() const
		{
			return this->_key;
		}

		bool KeyboardEvent::IsPress() const
		{
			return this->_type == Press;
		}

		bool KeyboardEvent::IsRelease() const
		{
			return this->_type == Release;
		}

		bool KeyboardEvent::IsValid() const
		{
			return this->_type != Invalid;
		}
	}
}	
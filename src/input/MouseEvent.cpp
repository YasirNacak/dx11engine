#include "MouseEvent.h"

namespace s3d { namespace input {
	MouseEvent::MouseEvent() :
		_type(Invalid), _x(0), _y(0)
	{
	}

	MouseEvent::MouseEvent(const EventType type, const int x, const int y) :
		_type(type), _x(x), _y(y)
	{
	}

	bool MouseEvent::IsValid() const
	{
		return this->_type != Invalid;
	}

	MouseEvent::EventType MouseEvent::GetType() const
	{
		return this->_type;
	}

	MousePoint MouseEvent::GetPosition() const
	{
		return { this->_x, this->_y };
	}
} }
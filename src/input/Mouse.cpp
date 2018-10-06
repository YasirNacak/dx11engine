#include "Mouse.h"

namespace s3d { namespace input {
	void Mouse::OnLeftPressed(const int x, const int y)
	{
		this->_isLeftDown = true;
		this->_eventBuffer.push({ MouseEvent::EventType::LPress, x, y });
	}

	void Mouse::OnLeftReleased(const int x, const int y)
	{
		this->_isLeftDown = false;
		this->_eventBuffer.push({ MouseEvent::EventType::LRelease, x, y });
	}

	void Mouse::OnRightPressed(const int x, const int y)
	{
		this->_isRightDown = true;
		this->_eventBuffer.push({ MouseEvent::EventType::RPress, x, y });
	}

	void Mouse::OnRightReleased(const int x, const int y)
	{
		this->_isRightDown = false;
		this->_eventBuffer.push({ MouseEvent::EventType::RRelease, x, y });
	}

	void Mouse::OnMiddlePressed(const int x, const int y)
	{
		this->_isMiddleDown = true;
		this->_eventBuffer.push({ MouseEvent::EventType::MPress, x, y });
	}

	void Mouse::OnMiddleReleased(const int x, const int y)
	{
		this->_isMiddleDown = false;
		this->_eventBuffer.push({ MouseEvent::EventType::MRelease, x, y });
	}

	void Mouse::OnWheelUp(const int x, const int y)
	{
		this->_eventBuffer.push({MouseEvent::EventType::WheelUp, x, y});
	}

	void Mouse::OnWheelDown(int x, int y)
	{
		this->_eventBuffer.push({MouseEvent::EventType::WheelDown, x, y});
	}

	void Mouse::OnMouseMove(int x, int y)
	{
		this->_x = x;
		this->_y = y;
		this->_eventBuffer.push({MouseEvent::EventType::Move, x, y});
	}

	void Mouse::OnMouseMoveRaw(int x, int y)
	{
		this->_eventBuffer.push({ MouseEvent::EventType::RawMove, x, y });
	}

	bool Mouse::IsLeftDown() const
	{
		return this->_isLeftDown;
	}

	bool Mouse::IsRightDown() const
	{
		return this->_isRightDown;
	}

	bool Mouse::IsMiddleDown() const
	{
		return this->_isMiddleDown;
	}

	MousePoint Mouse::GetPosition() const
	{
		return { this->_x, this->_y };
	}

	bool Mouse::IsEventBufferEmpty() const
	{
		return this->_eventBuffer.empty();
	}

	MouseEvent Mouse::ReadEvent()
	{
		if (IsEventBufferEmpty())
			return {};
		const auto event = this->_eventBuffer.front();
		this->_eventBuffer.pop();
		return event;
	}
} }
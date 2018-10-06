#pragma once

namespace s3d { namespace input {
	struct MousePoint
	{
		int x;
		int y;
	};

	class MouseEvent
	{
	public:
		enum EventType
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
			RawMove,
			Invalid
		};
	private:
		EventType _type;
		int _x;
		int _y;
	public:
		MouseEvent();
		MouseEvent(EventType type, int x, int y);
		bool IsValid() const;
		EventType GetType() const;
		MousePoint GetPosition() const;
	};
} }
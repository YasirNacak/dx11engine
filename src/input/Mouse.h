#pragma once
#include "MouseEvent.h"
#include <queue>

namespace s3d
{
	namespace input
	{
		class Mouse
		{
		public:
			void OnLeftPressed(int x, int y);
			void OnLeftReleased(int x, int y);
			void OnRightPressed(int x, int y);
			void OnRightReleased(int x, int y);
			void OnMiddlePressed(int x, int y);
			void OnMiddleReleased(int x, int y);
			void OnWheelUp(int x, int y);
			void OnWheelDown(int x, int y);
			void OnMouseMove(int x, int y);
			void OnMouseMoveRaw(int x, int y);

			bool IsLeftDown() const;
			bool IsRightDown() const;
			bool IsMiddleDown() const;

			MousePoint GetPosition() const;

			bool IsEventBufferEmpty() const;
			MouseEvent ReadEvent();

		private:
			std::queue<MouseEvent> _eventBuffer;
			bool _isLeftDown = false;
			bool _isRightDown = false;
			bool _isMiddleDown = false;
			int _x = 0;
			int _y = 0;
		};
	}
}
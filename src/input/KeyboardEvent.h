#pragma once

namespace s3d {
	namespace input
	{
		class KeyboardEvent {
		public:
			enum EventType {
				Press,
				Release,
				Invalid
			};

			KeyboardEvent();
			KeyboardEvent(EventType type, unsigned char key);

			unsigned char GetKeyCode() const;

			bool IsPress() const;
			bool IsRelease() const;
			bool IsValid() const;
		private:
			EventType _type;
			unsigned char _key;
		};
	}
}	
#ifndef SC_EVENTS_H
#define SC_EVENTS_H

#include <vector>
#include <Society/Core/Log.h>
#include <Society/Managers/EventSystem.h>
#include <Society/Core/Base.h>

namespace sc {
	enum EVENT_TYPE {
		// Window Events
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS,
		WINDOW_ICONIFY, WINDOW_MAXIMIZE, WINDOW_CONTENT_SCALE,
		WINDOW_POS, WINDOW_REFRESH,
		// App Events
		APP_TICK, APP_UPDATE, APP_RENDER,
		// Key Events
		KEY_PRESSED, KEY_RELEASED, KEY_REPEATED,
		KEY_TYPED,
		// Mouse Events
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVED,
		MOUSE_SCROLLED
	};

	class DECLSPEC Event {
		public:
			/* data */
			EVENT_TYPE m_type;

		public:
			explicit Event(const EVENT_TYPE type);

			virtual ~Event();

			virtual void Print();
	};

	class KeyEvent final : public Event {
		public:
			/* data */
			int m_keyCode;
			int m_scanCode;
			int m_action;
			int m_mods;

		public:
			KeyEvent(
				EVENT_TYPE type,
				int keyCode,
				int scanCode,
				int action,
				int mods
			);

			~KeyEvent() override;

			void Print() override;
	};

	class MouseEvent final : public Event {
		public:
			/* data */
			int m_button;
			int m_action;
			int m_mods;

		public:
			MouseEvent(
				const EVENT_TYPE type,
				const int button,
				const int action,
				const int mods
			);

			~MouseEvent() override;

			void Print() override;
	};

	class WindowEvent final : public Event {
		public:
			/* data */
			int m_width;
			int m_height;
			int m_xscale;
			int m_yscale;
			int m_xpos;
			int m_ypos;
			int m_iconified;
			int m_maximized;
			int m_focused;

		public:
			WindowEvent(
				const EVENT_TYPE type,
				const int width,
				const int height,
				const int xscale,
				const int yscale,
				const int xpos,
				const int ypos,
				const int iconified,
				const int maximized,
				const int focused
			);

			~WindowEvent() override;

			void Print() override;
	};

	class Observer {
		private:
			/* data */
			std::vector<EVENT_TYPE> m_events;

		public:
			virtual ~Observer() = 0;

			std::vector<EVENT_TYPE> GetEvents();

			static void ListenFor();

			template<typename First, typename... Events>
			void ListenFor(First arg, const Events &... rest);

			virtual void OnNotify(Event *event) = 0;
	};
}

#endif  // !SC_EVENTS_H

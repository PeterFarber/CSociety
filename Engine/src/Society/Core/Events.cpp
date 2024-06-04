
#include <Society/Core/Events.h>

namespace sc {
	Event::Event(const EVENT_TYPE type): m_type(type) {}

	Event::~Event() = default;

	void Event::Print() {
		SC_CORE_INFO("Event Type: {0}", static_cast<int>(m_type));
	}

	KeyEvent::KeyEvent(
		const EVENT_TYPE type,
		const int keyCode,
		const int scanCode,
		const int action,
		const int mods
	): Event(type), m_keyCode(keyCode), m_scanCode(scanCode),
	   m_action(action), m_mods(mods) {}

	KeyEvent::~KeyEvent() = default;

	void KeyEvent::Print() {
		Event::Print();

		SC_CORE_INFO(
			"Key: {0}, Scancode: {1}, Action: {2}, Mods: {3}",
			m_keyCode,
			m_scanCode,
			m_action,
			m_mods
		);
	}

	MouseEvent::MouseEvent(
		const EVENT_TYPE type,
		const int button,
		const int action,
		const int mods
	): Event(type), m_button(button), m_action(action), m_mods(mods) {}

	MouseEvent::~MouseEvent() = default;

	void MouseEvent::Print() {
		Event::Print();

		SC_CORE_INFO(
			"Button: {0}, Action: {1}, Mods: {2}",
			m_button,
			m_action,
			m_mods
		);
	}

	WindowEvent::WindowEvent(
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
	): Event(type), m_width(width), m_height(height), m_xscale(xscale),
	   m_yscale(yscale), m_xpos(xpos), m_ypos(ypos),
	   m_iconified(iconified), m_maximized(maximized),
	   m_focused(focused) {}

	WindowEvent::~WindowEvent() = default;

	void WindowEvent::Print() {
		Event::Print();

		switch (m_type) {
			case EVENT_TYPE::WINDOW_RESIZE:
				SC_CORE_INFO(
					"Window Resize: {0}, {1}",
					m_width,
					m_height
				);
				break;
			case EVENT_TYPE::WINDOW_CONTENT_SCALE:
				SC_CORE_INFO(
					"Window content scale: {0}, {1}",
					m_xscale,
					m_yscale
				);
				break;
			case EVENT_TYPE::WINDOW_POS:
				SC_CORE_INFO(
					"Window position: {0}, {1}",
					m_xpos,
					m_ypos
				);
				break;
			case EVENT_TYPE::WINDOW_ICONIFY:
				SC_CORE_INFO("Window iconify: {0}", m_iconified);
				break;
			case EVENT_TYPE::WINDOW_MAXIMIZE:
				SC_CORE_INFO("Window maximize: {0}", m_maximized);
				break;
			case EVENT_TYPE::WINDOW_FOCUS:
				SC_CORE_INFO("Window focus: {0}", m_focused);
				break;
			case EVENT_TYPE::WINDOW_CLOSE:
				SC_CORE_INFO("Window close");
				break;
			case EVENT_TYPE::WINDOW_REFRESH:
				SC_CORE_INFO("Window refresh");
				break;
			default:
				break;
		}
	}

	std::vector<EVENT_TYPE> Observer::GetEvents() { return m_events; }

	template<typename First, typename... Events>
	void Observer::ListenFor(First arg, const Events &... rest) {
		m_events.push_back(arg);
		EventSystem::GetInstance()->AddObserver(this);
		ListenFor(rest...);
	}

	void Observer::ListenFor() {}
}


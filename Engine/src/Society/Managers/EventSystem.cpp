

#include <Society/Core/Events.h>
#include <Society/Managers/EventSystem.h>

namespace sc {
	void EventSystem::AddObserver(Observer *observer) {
		m_observers.push_back(observer);
	}

	void EventSystem::RemoveObserver(Observer *observer) {
		m_observers.erase(
			std::remove(m_observers.begin(), m_observers.end(), observer),
			m_observers.end()
		);
	}

	void EventSystem::NotifyObservers(Event *event) const {
		for (const auto observer: m_observers) {
			if (std::vector<EVENT_TYPE> events = observer->GetEvents();
				std::find(events.begin(), events.end(), event->m_type) == events
				.end()) {
				continue;
			} else {
				observer->OnNotify(event);
			}
		}
	}
}

#ifndef SC_EVENTSYSTEM_H
#define SC_EVENTSYSTEM_H
#include <Society/Interfaces/ISingleton.h>
#include <Society/Core/Base.h>

namespace sc {
	class Observer;
	class Event;

	class DECLSPEC EventSystem : public ISingleton<EventSystem> {
		private:
			std::vector<Observer *> m_observers;

		public:
			void AddObserver(Observer *observer);

			void RemoveObserver(Observer *observer);

			void NotifyObservers(Event *event) const;
	};
}

#endif

#pragma once
#include <mutex>

template<typename T>
class ISingleton {
	private:
		static T *m_instance;
		static std::mutex m_mutex;

	protected:
		ISingleton() {}

		~ISingleton() {}

	public:
		ISingleton(ISingleton const &) = delete;

		ISingleton &operator=(ISingleton const &) = delete;

		static T *GetInstance() {
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_instance == nullptr) {
				m_instance = new T();
			}
			return m_instance;
		}
};

template<typename T>
T *ISingleton<T>::m_instance = nullptr;

template<typename T>
std::mutex ISingleton<T>::m_mutex;

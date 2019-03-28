#pragma once

#include <vector>
#include <algorithm>

#include "Observer.h"

namespace observer_pattern
{
	template<typename T>
	class Subject
	{
	public:
		using observer_t = Observer<T>;

		void addObserver(observer_t* observer)
		{
			m_observers.push_back(observer);
		}
		
		void removeObserver(observer_t* observer)
		{
			auto itr = std::find(m_observers.begin(), m_observers.end(), observer);
			if (itr != m_observers.end())
			{
				m_observers.erase(itr);
			}
		}

		void notify(const T& notification);
	private:
		std::vector<observer_t*> m_observers;
	};

	template<typename T>
	void Subject<T>::notify(const T& notification)
	{
		for_each(m_observers.begin(), m_observers.end(), [&notification](observer_t* _observer)
		{
			_observer->onNotify(notification);
		});
	}
}
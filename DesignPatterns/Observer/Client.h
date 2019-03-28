#pragma once

#include <iostream>
#include <string>
#include"Observer.h"

namespace observer_pattern
{
	class Client: public Observer<std::string>
	{
	public:
		Client(std::string _name)
			: m_name(_name)
		{}

		void onNotify(const std::string& message) override;

		void notification(std::string _msg)
		{
			onNotify((const std::string&)_msg);
		}

	private:
		std::string m_name;
	};

	inline void Client::onNotify(const std::string& message)
	{
		std::cout << m_name << ": " << message << std::endl;
	}
}

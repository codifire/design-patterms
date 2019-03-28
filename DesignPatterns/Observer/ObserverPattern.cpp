#include <vector>
#include <string>

#include "ObserverPattern.h"
#include "Server.h"
#include "Client.h"


#include "../Delegate/Delegate.h"

namespace observer_pattern
{
	template<typename RET, typename ARG0>
	struct Subscription
	{
		using delegate_t = Delegate<RET (ARG0)>;

		void subscribe(delegate_t sub)
		{
			m_subscribers.push_back(sub);
		}

		void unsubscribe(const delegate_t& sub)
		{
			auto itr = std::find(m_subscribers.begin(), m_subscribers.end(), sub);

			std::remove_if(m_subscribers.begin(), m_subscribers.end(), [&sub](const delegate_t& _other)
			{
				bool result = false;
				
				if (_other == sub)
					result = true;

				return result;
			});
		}

		std::vector<delegate_t> m_subscribers;
	};
	
	struct AnotherServer
	{
		Subscription<void, std::string> m_artsSubscribers;
		Subscription<void, std::string> m_gadgetsSubscribers;
		Subscription<void, std::string> m_AnythingSubscribers;
	};
}

void ObserverPattern::entry()
{
	using namespace observer_pattern;

	Server server;

	Client grg("George"),
		   brd("Brad"),
		   nic("Nicolas");

	server.registerNotification(Server::NotificationType::Arts, &grg);
	server.registerNotification(Server::NotificationType::Gadgets, &brd);
	server.registerNotification(Server::NotificationType::Anything, &nic);

	server.pushNotification(Server::NotificationType::Arts, std::string("Monalisa"));
	server.pushNotification(Server::NotificationType::Gadgets, std::string("iPhoneX"));

	AnotherServer anotherServer;
	
	Delegate<void(std::string)> d;
	d.Bind<Client, &Client::notification>(&grg);
	
	anotherServer.m_artsSubscribers.subscribe(d);
}
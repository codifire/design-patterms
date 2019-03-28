#pragma once

#include <string>
#include "Subject.h"

namespace observer_pattern
{
	class Server
	{
	public:
		enum class NotificationType
		{
			Arts,
			Gadgets,
			Anything
		};

		void registerNotification(NotificationType _type, Observer<std::string>* ptr)
		{
			switch (_type)
			{
			case NotificationType::Arts:
				m_subscribersArts.addObserver(ptr);
				break;

			case NotificationType::Gadgets:
				m_subscribersGadgets.addObserver(ptr);
				break;

			default:
				m_subscribersAnything.addObserver(ptr);
			}
		}

		void pushNotification(NotificationType _type, const std::string& _notif)
		{
			switch (_type)
			{
			case NotificationType::Arts:
				m_subscribersArts.notify(_notif);
				break;

			case NotificationType::Gadgets:
				m_subscribersGadgets.notify(_notif);
				break;
			}

			m_subscribersAnything.notify(_notif);
		}

	private:
		Subject<std::string> m_subscribersArts;
		Subject<std::string> m_subscribersGadgets;
		Subject<std::string> m_subscribersAnything;
	};
}
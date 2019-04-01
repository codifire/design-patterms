#pragma once
#include <string>
namespace adapter_pattern
{
	// the interface of our engine socket
	class IUDPSendSocket
	{
	public:
		virtual ~IUDPSendSocket() {}

		virtual void open() = 0;
		virtual void close() = 0;
		virtual void send(const char* host, const char* buffer, size_t size) = 0;
	};

	// platform socket lib
	struct BSDSocket
	{
		enum class Protocol { TCP, UDP, RAW };
		struct AcceptConnectionInfo {};

		void open(Protocol _type) { (void)_type; }
		void shutdown() {}
		void bind(short port) { (void)port; }
		void listen(){}
		void connect(const char* host) { (void)host; }
		bool accept(AcceptConnectionInfo& _info)
		{
			(void)_info;
			return true;
		}
		void send(const char* host, const char* buff, size_t size)
		{
			(void)buff;
			(void)size;
		}
		size_t recv(char* buff, size_t size)
		{
			(void)buff;
			(void)size;
			return 0;
		}
		bool gethostbyname(const char* host, char* buff, size_t size)
		{
			(void)host;
			(void)buff;
			(void)size;
			return true;
		}
		bool gethostbyaddr(const char* host, char* buff, size_t size)
		{
			(void)host;
			(void)buff;
			(void)size;
			return true;
		}
	};

	// object adapter
	// will adapt BSDSocket to IUDPSendSocket
	class AdapterBSD_UDP: public IUDPSendSocket
	{
	public:
		void open() override { socket.open(BSDSocket::Protocol::UDP); }
		void close() override { socket.shutdown(); }

		void send(const char* host, const char* buffer, size_t size) override
		{
			char hostIP[256];
			memset(hostIP, 0, 256);
			socket.gethostbyname(host, hostIP, 256);

			socket.send(hostIP, buffer, size);
		}
	private:
		BSDSocket socket;
	};

	// class adapter
	// Lightweight because it doesnt contain a socket object so no extra indirection
	class AdapterBSD_UDPLightweight: public IUDPSendSocket, private BSDSocket
	{
	public:
		AdapterBSD_UDPLightweight() { BSDSocket::open(Protocol::UDP); }
		~AdapterBSD_UDPLightweight() override { BSDSocket::shutdown(); }

		// we rather open/close it in the c'tor/d'tor to give it a leight weight appearence
		void open() override {}
		void close() override {}

		void send(const char* host, const char* buffer, size_t size) override
		{
			char hostIP[256];
			memset(hostIP, 0, 256);
			BSDSocket::gethostbyname(host, hostIP, 256);
			BSDSocket::send(hostIP, buffer, size);
		}
	};

	void QuickSend(const char* hostname, const char* buffer, size_t size)
	{
		AdapterBSD_UDP socket;
		socket.send(hostname, buffer, size);
	}

	void QuickSend2(const char* hostname, const char* buffer, size_t size)
	{
		AdapterBSD_UDPLightweight().send(hostname, buffer, size);
	}
}
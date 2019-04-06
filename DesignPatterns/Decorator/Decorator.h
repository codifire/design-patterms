#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace decorator_pattern
{
	// Intent: Attach additional responsibilities to an object dynamically. Inheritance can become impractical due to explosion of classes needed to be derived from or inherited for various combinations of behaviors.
	
	// class defines the interface of a component to which different behaviors can be added on runtime (or which can be decorated)
	struct IDataSource
	{
		virtual ~IDataSource() {}
		virtual size_t read(void* buff, size_t sz) = 0;
		virtual size_t write(const void* buff, size_t sz) = 0;
	};

	// concrete class to which behaviors can be added dynamically
	struct FileDataSource : public IDataSource
	{
		FileDataSource(std::string) {}
		size_t read(void*, size_t) override 
		{
			std::cout << "data read from file" << std::endl;
			return 0; 
		}
		size_t write(const void*, size_t) override
		{
			std::cout << "data written to file" << std::endl;
			return 0; 
		}
	};

	// concrete class which can be decorated
	struct TCPSocketDataSource : public IDataSource
	{
		TCPSocketDataSource(short) {}
		size_t read(void*, size_t) override 
		{
			std::cout << "data read from socket" << std::endl;
			return 0; 
		}
		size_t write(const void*, size_t) override 
		{
			std::cout << "data written to socket" << std::endl;
			return 0; 
		}
	};

	// interface for the classes used to decorate the components (or adding the responsibilities to the component
	// looks like a proxy; derived from the same interface which it maintains a reference to. decorators can decorate other decorators
	class IDataSourceDecorators : public IDataSource
	{
	public:
		IDataSourceDecorators(IDataSource* _source) : m_dataSource(_source)
		{}

		size_t read(void* buff, size_t sz) override 
		{
			return m_dataSource->read(buff, sz);
		}
		size_t write(const void* buff, size_t sz) override 
		{ 
			return m_dataSource->write(buff, sz);
		}

	private:
		IDataSource* m_dataSource; // component to be decorated
	};

	// a concrete decorator that compresses/uncompresses data before/after writing/reading correspondingly.
	class DataCompressionDecorator: public IDataSourceDecorators
	{
	public:
		DataCompressionDecorator(IDataSource* _source, std::string _algorithm)
			: IDataSourceDecorators(_source)

		{}

		size_t read(void* buff, size_t sz) override
		{
			auto newsz = IDataSourceDecorators::read(buff, sz);
			std::cout << "un-compressing data with alg: " << m_algorithm << std::endl;
			// newsz = size after uncompressing
			return newsz;
		}

		size_t write(const void* buff, size_t sz) override
		{
			std::cout << "compressing data with alg: " << m_algorithm << std::endl;
			return IDataSourceDecorators::write(buff, sz);
		}

	private:
		std::string m_algorithm;
	};

	// a concrete decorator that encrypts/decrypts data before/after writing/reading correspondingly.
	class DataEncryptionDecorator : public IDataSourceDecorators
	{
	public:
		DataEncryptionDecorator(IDataSource* _source, std::string _algorithm)
			: IDataSourceDecorators(_source)

		{}

		size_t read(void* buff, size_t sz) override
		{
			auto newsz = IDataSourceDecorators::read(buff, sz);
			std::cout << "decrypting data with alg: " << m_algorithm << std::endl;
			return newsz;
		}

		size_t write(const void* buff, size_t sz) override
		{
			std::cout << "encrypting data with alg: " << m_algorithm << std::endl;
			return IDataSourceDecorators::write(buff, sz);
		}

	private:
		std::string m_algorithm;
	};
	
	void demo()
	{
		std::cout << std::endl;

		IDataSource* filePtr = new FileDataSource("data.txt");
		filePtr = new DataCompressionDecorator(filePtr, std::string("zip"));
		filePtr = new DataEncryptionDecorator(filePtr, std::string("AES"));
		
		std::cout << std::endl;

		char buff[512 * 1024]{};
		filePtr->read(buff, sizeof(buff));
		
		std::cout << std::endl;
		
		filePtr->write(buff, sizeof(buff));

		std::cout << std::endl;

		IDataSource* sockPtr = new TCPSocketDataSource(8080);
		sockPtr = new DataCompressionDecorator(sockPtr, std::string("zip"));
		sockPtr = new DataEncryptionDecorator(sockPtr, std::string("AES"));
		sockPtr->write(buff, sizeof(buff));

		std::cout << std::endl;
	}
}
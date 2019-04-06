#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

namespace decorator_pattern
{
	class Buffer
	{
	public:
		Buffer(const size_t size = 64 * 1024)
		{
			m_buff.reserve(size);
		}

		size_t read(void* buff, size_t sz, size_t offset)
		{
			const size_t ReadSize = std::min(m_buff.size(), sz);
			memcpy(buff, &m_buff[offset], ReadSize);
		}
			
		void write(const void* buff, size_t sz)
		{
			m_buff.append(reinterpret_cast<const char*>(buff), sz);
		}

		size_t size() const
		{
			return m_buff.size();
		}

	private:
		std::string m_buff;
	};


	struct DataSource
	{
		virtual ~DataSource() {}
		virtual size_t read(void* buff, size_t sz) = 0;
		virtual size_t write(const void* buff, size_t sz) = 0;
	};

	struct FileDataSource : public DataSource
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

	struct TCPSocketDataSource : public DataSource
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

	class DataSourceDecorators : public DataSource
	{
	public:
		DataSourceDecorators(DataSource* _source) : m_dataSource(_source)
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
		DataSource* m_dataSource;
	};

	class DataCompressionDecorator: public DataSourceDecorators
	{
	public:
		DataCompressionDecorator(DataSource* _source, std::string _algorithm)
			: DataSourceDecorators(_source)

		{}

		size_t read(void* buff, size_t sz) override
		{
			auto newsz = DataSourceDecorators::read(buff, sz);
			std::cout << "un-compressing data with alg: " << m_algorithm << std::endl;
			// newsz = size after uncompressing
			return newsz;
		}

		size_t write(const void* buff, size_t sz) override
		{
			std::cout << "compressing data with alg: " << m_algorithm << std::endl;
			return DataSourceDecorators::write(buff, sz);
		}

	private:
		std::string m_algorithm;
	};

	class DataEncryptionDecorator : public DataSourceDecorators
	{
	public:
		DataEncryptionDecorator(DataSource* _source, std::string _algorithm)
			: DataSourceDecorators(_source)

		{}

		size_t read(void* buff, size_t sz) override
		{
			auto newsz = DataSourceDecorators::read(buff, sz);
			std::cout << "decrypting data with alg: " << m_algorithm << std::endl;
			return newsz;
		}

		size_t write(const void* buff, size_t sz) override
		{
			std::cout << "encrypting data with alg: " << m_algorithm << std::endl;
			return DataSourceDecorators::write(buff, sz);
		}

	private:
		std::string m_algorithm;
	};
	
	void sendData()
	{
		std::cout << std::endl;

		DataSource* filePtr = new FileDataSource("data.txt");
		filePtr = new DataCompressionDecorator(filePtr, std::string("zip"));
		filePtr = new DataEncryptionDecorator(filePtr, std::string("AES"));
		
		std::cout << std::endl;

		char buff[512 * 1024]{};
		filePtr->read(buff, sizeof(buff));
		
		std::cout << std::endl;
		
		filePtr->write(buff, sizeof(buff));

		std::cout << std::endl;

		DataSource* sockPtr = new TCPSocketDataSource(8080);
		sockPtr = new DataCompressionDecorator(sockPtr, std::string("zip"));
		sockPtr = new DataEncryptionDecorator(sockPtr, std::string("AES"));
		sockPtr->write(buff, sizeof(buff));

		std::cout << std::endl;
	}
}
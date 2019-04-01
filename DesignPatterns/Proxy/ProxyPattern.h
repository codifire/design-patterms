#pragma once
#include <string>
#include <fstream>

namespace proxy_pattern
{
	class IFile
	{
	public:
		virtual bool open(const char* name) = 0;
		virtual void close() = 0;
		virtual size_t read(char* buffer, size_t size) = 0;
		virtual void write(char* buffer, size_t size) = 0;
		virtual void seek(size_t offset) = 0;
		
		virtual size_t position() = 0;
		virtual size_t size() const = 0;
		virtual bool isOpen() const = 0;

		virtual ~IFile() {};
	};

	class File: public IFile
	{
	public:
		bool open(const char* name) override;
		void close() override;
		size_t read(char* buffer, size_t size) override;
		void write(char* buffer, size_t size) override;
		void seek(size_t offset) override;
		size_t position() override;
		size_t size() const override;
		bool isOpen() const override;
	
	private:
		size_t m_size;
		std::fstream file;
	};

	bool File::open(const char* name)
	{
		file.open(name, std::ifstream::ate | std::ifstream::binary);
		m_size = file.tellg();
		file.seekg(std::ifstream::beg);
		return file.is_open();
	}

	void File::close()
	{
		file.close();
	}

	size_t File::read(char* buffer, size_t size)
	{
		file.read(buffer, size);
	}

	void File::write(char* buffer, size_t size)
	{
		file.write(buffer, size);
	}

	void File::seek(size_t offset)
	{
		file.seekg(offset);
	}

	size_t File::position()
	{
		return file.tellg();
	}

	size_t File::size() const
	{
		return m_size;
	}

	bool File::isOpen() const
	{
		return file.is_open();
	}

	// proxy
	// LazyFile and File can be used interchangeably
	class LazyFile: public IFile
	{
		bool open(const char* _name) override
		{
			name = _name;
			return !name.empty();
		}
		void close() override
		{
			if (file.isOpen())
				file.close();
		}
		size_t read(char* buffer, size_t size) override
		{
			openIfRequired();
			return file.read(buffer, size);
		}
		void write(char* buffer, size_t size) override
		{
			openIfRequired();
			return file.write(buffer, size);
		}
		void seek(size_t offset) override
		{
			openIfRequired();
			file.seek(offset);
		}
		size_t position() override
		{
			return file.position();
		}
		size_t size() const override
		{
			return file.size();
		}
		bool isOpen() const 
		{ 
			file.isOpen(); 
		}

	private:
		void openIfRequired()
		{
			if (!isOpen())
				file.open(name.c_str());
		}

		std::string name;
		File file;
	};

	class MemoryFile: public IFile
	{
	public:
		explicit MemoryFile(size_t sz = 256)
		{
			file.reserve(sz);
		}



		std::string file;
	};
}
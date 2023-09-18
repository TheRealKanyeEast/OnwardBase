#include "Logger.hpp"

namespace Onward
{
	void Logger::Initialize()
	{
		m_FilePath.append(std::getenv("USERPROFILE"));
		m_FilePath.append("Documents");
		m_FilePath.append(ONWARD_NAME);
		if (!std::filesystem::exists(m_FilePath))
			std::filesystem::create_directory(m_FilePath);
		m_FilePath.append(ONWARD_NAME ".log");

		AllocConsole();
		SetConsoleTitleA(ONWARD_NAME);

		SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 235, LWA_ALPHA);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		m_Console.open("$CONOUT");

		m_File.open(m_FilePath, std::ios_base::out | std::ios_base::app);
	}

	void Logger::Uninitialize()
	{
		FreeConsole();
		m_Console.clear();
		m_FilePath.clear();
		m_Console.close();
		m_File.close();
	}

	void Logger::Raw(const char* prefix, const char* format, std::va_list args)
	{
		std::lock_guard lock(m_Mutex);

		auto time = std::time(nullptr);
		auto localtime = std::localtime(&time);

		char prefixFinal[64] = {};
		std::snprintf(prefixFinal, sizeof(prefixFinal) - 1, "[%02d:%02d:%02d] [%s] ", localtime->tm_hour, localtime->tm_min, localtime->tm_sec, prefix);

		std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
		auto messageBuffer = std::make_unique<char[]>(messageLength);

		std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
		std::vsnprintf(messageBuffer.get(), messageLength, format, args);

		m_File << prefix << messageBuffer.get() << std::endl;
		m_Console << prefix << messageBuffer.get() << std::endl;
	}

	void Logger::Custom(eLogColor logColor, const char* prefix, const char* format, ...)
	{
		std::va_list args{};
		va_start(args, format);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), logColor);
		Raw(prefix, format, args);
		va_end(args);
	}

	namespace Log
	{
		Logger* GetInstance()
		{
			static Logger instance;
			return &instance;
		}
	}
}
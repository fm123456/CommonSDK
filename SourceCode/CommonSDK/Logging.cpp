#if defined(WIN32) || defined(_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#endif
#include <boost/thread.hpp>
#include "Logging.h"
#include "DateTime.h"

#if defined(WIN32) || defined(_WINDOWS)
  #define open   _open
  #define fdopen _fdopen
  #define close  _close
  #define unlink _unlink
#endif

namespace fm {

// 日志功能的实现类。该类仅在内部使用，负责日志的创建、管理和输出。
class LoggingImpl
{
	friend class Logging;
public:
	LoggingImpl(const char* name, const char* path, int config, int rollover);

	~LoggingImpl();

	void Log(LoggingMessage& message);

private:
	void CreateLogFile();

	// 实现日志卷回滚
	int rollover_size, rollover_frequency, rollover_attempt;

	// 日志文件
	FILE* log_file;
	std::string log_name, log_path;
	std::string host, user;
	int log_config, log_file_size;
	boost::mutex log_mutex;

	const int MAX_LOGFILE_SIZE;
};

// 日志系统类，包含所有的有效日志及相关配置。
struct LoggingSystem
{
	LoggingSystem() : default_logging(NULL)
	{
#ifdef _DEBUG
		severity = SEV_DEBUG;
#else
		severity = SEV_INFO;
#endif
	}

	// 输出的日志级别
	int severity;

	// 实现向其它侦听者发送日志
	std::list<LoggingListener*> listeners;
	boost::mutex listener_mutex;

	// 所有打开的日志
	LoggingImpl* default_logging;
	std::map<std::string, LoggingImpl*> loggings;
	boost::mutex logging_mutex;
};

static LoggingSystem& GetLoggingSystem()
{
	static LoggingSystem logging_system;
	return logging_system;
}

LoggingImpl::LoggingImpl(const char* name, const char* path, int config, int rollover)
	: rollover_size(rollover), rollover_frequency(8), rollover_attempt(0), log_file(NULL), log_name(name==NULL?"":name),
	  log_path(path==NULL?"":path), log_config(config), log_file_size(0), MAX_LOGFILE_SIZE(16*1024*1024)
{
	if( (log_config & LOG_NO_FILE_CREATED) == 0 ) {
		//host = GetHostName(false);
		//user = GetUserName();

		CreateLogFile();
	}
}

LoggingImpl::~LoggingImpl()
{
	if( log_file != NULL )
		fclose(log_file);
	log_file = NULL;
}

void LoggingImpl::Log(LoggingMessage& message)
{
	std::string log_text = message.Stream(false).str();

	if( log_file != NULL ) {
		boost::lock_guard<boost::mutex> lock(log_mutex);
		// 向日志文件输出日志
		if( log_file_size >= MAX_LOGFILE_SIZE ) {
			// 创建新的日志文件
			fclose(log_file);

			rollover_attempt++;
			if( rollover_attempt >= rollover_frequency )
				rollover_attempt = 0;

			CreateLogFile();
			log_file_size = 0;
		}
		int text_size = int(log_text.length());
		log_file_size += text_size;
		fwrite(log_text.c_str(), 1, text_size, log_file);
	}

	// 发送日志信息给Listener
	LoggingSystem& logging_system = GetLoggingSystem();
	if( !logging_system.listeners.empty() && (LOG_IGNORE_LISTENER & log_config) == 0 ) {
		boost::lock_guard<boost::mutex> lock(logging_system.listener_mutex);
		for(std::list<LoggingListener*>::iterator it = logging_system.listeners.begin();
			it != logging_system.listeners.end();
			++it)
			(*it)->Log(message.Name(), message.Severity(), log_text);
	}

	// 发送日志信息给标准输出
	if( (log_config & LOG_STD_STREAM) != 0 ) {
		if( message.Severity() <= SEV_ERROR )
			std::cerr<<log_text;
		else
			std::cout<<log_text;
	}
#if (defined(WIN32) || defined(_WINDOWS)) && defined(_DEBUG)
	::OutputDebugStringA(log_text.c_str());
#endif
}

void LoggingImpl::CreateLogFile()
{
	std::ostringstream stream;
	if( log_path.empty() )
		stream<<"./";
	else
		stream<<log_path<<"/";
	stream<<log_name;
	if( (log_config & LOG_NAME_COMPUTER) != 0 )
		stream<<"."<<host;
	if( (log_config & LOG_NAME_USER) != 0 )
		stream<<"."<<user;
	stream<<"_"<<rollover_attempt;
	if( (log_config & LOG_NAME_TIMESTAMP) != 0 ) {
		Time t = Time::Now();
		stream<<"_"<<std::setfill('0')<<std::setw(4)<<t.GetDate().GetYear()
			<<std::setw(2)<<t.GetDate().GetMonth()<<std::setw(2)<<t.GetDate().GetDay()
			<<std::setw(2)<<t.GetHour()<<std::setw(2)<<t.GetMinute()<<std::setw(2)<<t.GetSecond();
	}
	stream<<".log";

	std::string full_path = stream.str();
	int fd = open(full_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0664);
	if (fd == -1)
		std::cerr<<"Unable to open log file "<<full_path<<": "<<strerror(errno)<<std::endl;
	else {
		log_file = fdopen(fd, "a");
		if (log_file == NULL) {
			std::cerr<<"Unable to make a stream object on log file descriptor."<<std::endl;
			close(fd);
			unlink(full_path.c_str());
		} else if ((log_config & LOG_ENABLE_BUFFER) == 0)
			setbuf(log_file, NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////
Logging::Logging()
{
}

Logging::~Logging()
{
}

void Logging::Init(const char* name, const char* path, int config, int rollover)
{
	assert(name != NULL);
	assert(rollover != 0);

	const char* log_name = (strrchr(name, '/') == NULL) ? strrchr(name, '\\') : strrchr(name, '/');
	log_name = (log_name == NULL) ? name : log_name+1;

	LoggingSystem& system = GetLoggingSystem();
	boost::lock_guard<boost::mutex> lock(system.logging_mutex);
	if( system.loggings.find(log_name) == system.loggings.end() ) {
		LoggingImpl* impl = new LoggingImpl(log_name, path, config, rollover);
		system.loggings[log_name] = impl;
		if( system.default_logging == NULL )
			system.default_logging = impl;
	}
}

void Logging::InstallListener(LoggingListener* listener)
{
	if( listener == NULL )
		return;
	LoggingSystem& logging_system = GetLoggingSystem();
	boost::lock_guard<boost::mutex> lock(logging_system.listener_mutex);
	logging_system.listeners.push_back(listener);
}

void Logging::RemoveListener(LoggingListener* listener)
{
	if( listener == NULL )
		return;
	LoggingSystem& logging_system = GetLoggingSystem();
	boost::lock_guard<boost::mutex> lock(logging_system.listener_mutex);
	std::remove(logging_system.listeners.begin(), logging_system.listeners.end(), listener);
}

int& Logging::Severity()
{
	return GetLoggingSystem().severity;
}

void Logging::Shutdown(const char* name)
{
	LoggingSystem& system = GetLoggingSystem();
	boost::lock_guard<boost::mutex> lock(system.logging_mutex);
	if( name == NULL ) {
		std::map<std::string, LoggingImpl*>::iterator it = system.loggings.begin();
		while( it != system.loggings.end() ) {
			delete it->second;
			it++;
		}
		system.loggings.clear();
		system.default_logging = NULL;
	} else {
		std::map<std::string, LoggingImpl*>::iterator it = system.loggings.find(name);
		if( it != system.loggings.end() ) {
			if( it->second == system.default_logging )
				system.default_logging = NULL;
			delete it->second;
			system.loggings.erase(it);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
LoggingMessage::LoggingMessage(const char* name, int severity) : log_severity(severity), log_name(name)
{
}

LoggingMessage::~LoggingMessage()
{
	LoggingSystem& logging_system = GetLoggingSystem();
	// 析构时输出日志
	if( log_name == NULL && logging_system.default_logging != NULL )
		logging_system.default_logging->Log(*this);
	else {
		if( log_name != NULL ) {
			// 找到指定名字的日志
			std::map<std::string, LoggingImpl*>::iterator it = logging_system.loggings.find(log_name);
			if( it != logging_system.loggings.end() )
				it->second->Log(*this);
		} else {
			// 未创建日志文件时直接输出到标准流中
			if( log_severity <= SEV_ERROR )
				std::cerr<<log_stream.str();
			else
				std::cout<<log_stream.str();
#if (defined(WIN32) || defined(_WINDOWS)) && defined(_DEBUG)
			::OutputDebugStringA(log_stream.str().c_str());
#endif
		}
	}
}

std::ostringstream& LoggingMessage::Stream(bool header)
{
	if( !header )
		return log_stream;

	// 输出日志记录的时间信息和类型信息
	static const char SeverityName[][8] = {"FATAL  ", "ERROR  ", "WARNING", "INFO   ", "DEBUG  "};
	log_stream<<Time::Now().FormatString()<<" "<<SeverityName[log_severity]<<": ";
	return log_stream;
}

///////////////////////////////////////////////////////////////////////////////
LoggingTimer::LoggingTimer(const char* name, std::string desc)
{
	if( name ) log_name = name;
	log_desc = desc;
}

LoggingTimer::~LoggingTimer()
{
	LoggingMessage(log_name.c_str(), SEV_INFO).Stream()<<log_desc<<" "<<timer.Seconds()<<" second(s)."<<std::endl;
}

}

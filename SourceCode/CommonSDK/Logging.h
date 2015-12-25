#ifndef _FM_SDK_LOGGING_H_
#define _FM_SDK_LOGGING_H_

#include "Utility.h"
#include "Timer.h"

namespace fm {

const int SEV_FATAL            = 0;     /**< 严重错误级别 */
const int SEV_ERROR            = 1;     /**< 一般错误级别 */
const int SEV_WARNING          = 2;     /**< 警告级别     */
const int SEV_INFO             = 3;     /**< 信息级别     */
const int SEV_DEBUG            = 4;     /**< 调试级别     */

const int LOG_NAME_COMPUTER    = 0x01;  /**< 日志名中包含计算机名   */
const int LOG_NAME_USER        = 0x02;  /**< 日志名中包含用户名     */
const int LOG_NAME_TIMESTAMP   = 0x04;  /**< 日志名中包含时间戳     */
const int LOG_IGNORE_LISTENER  = 0x08;  /**< 日志输出忽略侦听器     */
const int LOG_NO_FILE_CREATED  = 0x10;  /**< 不创建日志文件         */
const int LOG_STD_STREAM       = 0x20;  /**< 同时输出日志到标准流   */
const int LOG_ENABLE_BUFFER    = 0x40;  /**< 允许日志输出时使用缓冲 */

/**
 * @brief 日志侦听器接口。
 *
 * LoggingListener 接口定义了通用的日志侦听器接口，允许程序的其它
 * 部分实现自定义的日志记录机制并装载到日志系统中。
 */
class LoggingListener
{
public:
	/**
	 * @brief 记录日志。
     * 
	 * @param name 日志的名称。
	 * @param severity 日志严重级别。
	 * @param msg 日志消息文本。
     */
	virtual void Log(const char* name, int severity, const std::string& msg) = 0;
};

/**
 * @brief 日志系统管理。
 *
 * Logging 类实现了 Titan SDK 中的标准日志系统。应用程序应在程序开始时启动日志，
 * 在程序结束时关闭日志。在这一过程中，也可以装载/移除自定义的日志侦听器。
 * @note 日志系统仅会输出比当前系统日志级别更高的日志消息，应用程序可在运行时动
 *       态调整当前的日志级别。可用的日志级别如下：
 * - LogFatal   = 0： 严重错误级别
 * - LogError   = 1： 一般错误级别
 * - LogWarning = 2： 警告级别
 * - LogInfo    = 3： 信息级别
 * - LogDebug   = 4： 调试级别
 */
class LIB_SDK Logging
{
public:
	/**
	 * @brief 初始化日志系统。
     * 
	 * @param name 日志的标识性名称。
	 * @param path 日志文件所在的路径（若不指定则为当前路径）。
	 * @param config 日志的配置选项（默认为 LogNameComputer|LogNameUser）。
	 * @param rollover 日志的卷大小（以兆为单位，默认为 8）。
	 * @note 日志的配置选项可以是以下值的按位或：
	 * - LogNameComputer    = 0x01： 日志名中包含计算机名
	 * - LogNameUser        = 0x02： 日志名中包含用户名
	 * - LogNameTimestamp   = 0x04： 日志名中包含时间戳
	 * - LogIgnoreListener  = 0x08： 日志输出忽略侦听器
	 * - LogNoFileCreated   = 0x10： 不创建日志文件
	 * - LogStdStream       = 0x20： 同时输出日志到标准流
     */
	static void Init(const char* name, const char* path = NULL, int config = LOG_NAME_COMPUTER|LOG_NAME_USER, int rollover = 8);

	/**
	 * @brief 安装自定义的日志侦听器。
     * 
	 * @param listener 日志侦听器接口。
     */
	static void InstallListener(LoggingListener* listener);

	/**
	 * @brief 删除指定的日志侦听器。
     * 
	 * @param listener 要删除的日志侦听器接口。
     */
	static void RemoveListener(LoggingListener* listener);

	/**
	 * @brief 获取或修改当前的日志严重级别。
     * 
	 * @return 日志严重级别的引用。
     */
	static int& Severity();

	/**
	 * @brief 关闭日志系统。
	 * 
	 * @param name 要关闭的日志。
	 * @note 如果关闭日志时不指定具体的名称，则关闭所有日志输出。
     */
	static void Shutdown(const char* name = NULL);

private:
	Logging();

	~Logging();
};

/**
 * @brief 日志消息。
 *
 * LoggingMessage 类代表了一条需要记录到日志中的消息。
 * @note
 * 程序代码中不应直接使用 LoggingMessage 类，而是应该使用以下宏间接地创建和使用该类的实例：
 * - LOG_FATAL: 输出严重级别的日志(Fatal)
 * - LOG_ERROR: 输出错误界别的日志(Error)
 * - LOG_WARNING: 输出警告级别的日志(Warning)
 * - LOG_INFO: 输出信息级别的日志(Info)
 * - LOG_DEBUG: 输出调试级别的日志(Debug)
 * - LOG_CHECK: 检查表达式是否成立，若不成立则输出日志并执行特定语句，如 LOG_CHECK(x!=100, "Wrong value:"<<x, return);
 * - LOG_IF: 检查表达式是否成立，若成立则输出日志，如 LOG_IF(x==100, "Correct value:"<<x);
 * - LOG_TIMER: 在当前作用域内声明计时器输出，如 LOG_TIMER("It is costs ");在离开当前作用域时会输出：It is costs 10.0 seconds.
 * .
 * 使用日志宏时可以结合 C++ 的 << 操作符实现数据变量的输出，例如：LOG_ERROR("Name "<<usr_name<<" has been used by others.");。
 * 若某些日志输出仅希望在调试阶段使用，程序发布时需要从代码中删除，可以使用上述宏的“仅调试”版本，即在宏名称前面加“D”，如：
 * - DLOG_FATAL
 * - DLOG_ERROR
 * - DLOG_CHECK
 * - ......
 */
class LIB_SDK LoggingMessage
{
public:
	/**
	 * @brief 构造函数。
     *
	 * @param name 输出的日志名称。
	 * @param severity 当前日志消息的严重级别。
     */
	LoggingMessage(const char* name, int severity);

	/**
	 * @brief 析构函数。
     */
	~LoggingMessage();

	/**
	 * @brief 获取日志消息的名称。
     * 
	 * @return 日志的名称。
     */
	inline const char* Name() const { return log_name; }

	/**
	 * @brief 获取日志消息的序列化流。
     * 
	 * @param header 是否自动生成日志消息的时间戳。
	 * @return 序列化日志消息的流。
     */
	std::ostringstream& Stream(bool header = true);

	/**
	 * @brief 获取日志消息严重级别。
     * 
	 * @return 严重级别。
     */
	inline int Severity() const { return log_severity; }

private:
	int                log_severity;
	const char*        log_name;
	std::ostringstream log_stream;
};

/**
 * @brief 输出计时时间的辅助日志类。
 */
class LIB_SDK LoggingTimer
{
public:
	LoggingTimer(const char* name, std::string desc);

	~LoggingTimer();

private:
	std::string log_name;

	std::string log_desc;

	Timer timer;
};

#define LOG(name, severity, msg)                                              \
	do {                                                                      \
		if( severity <= ::fm::Logging::Severity() )                        \
			::fm::LoggingMessage(name, severity).Stream()<<msg<<std::endl; \
	}while(0)
#define PLOG(name, severity, msg) LOG(name, severity, msg<<": "<<strerror(errno))

#ifdef _DEBUG
#define DLOG(name, severity, msg) LOG(name, severity, msg)
#else
#define DLOG(name, severity, msg)
#endif

#define LOG_FATAL(msg)	 LOG(NULL, ::fm::SEV_FATAL,   msg)
#define LOG_ERROR(msg)	 LOG(NULL, ::fm::SEV_ERROR,   msg)
#define LOG_WARNING(msg) LOG(NULL, ::fm::SEV_WARNING, msg)
#define LOG_INFO(msg)	 LOG(NULL, ::fm::SEV_INFO,    msg)
#define LOG_DEBUG(msg)	 LOG(NULL, ::fm::SEV_DEBUG,   msg)
#define LOG_CHECK(cond, msg, deal)	\
	if(!(cond)) {					\
		LOG_ERROR(msg<<#cond);		\
		deal;						\
	}
#define LOG_IF(cond, msg)			\
	if( cond )						\
		LOG_DEBUG(msg)
#define LOG_TIMER(msg) ::fm::LoggingTimer _logging_timer_##__LINE__##(NULL, msg)

#define PLOG_FATAL(msg)   PLOG(NULL, ::fm::SEV_FATAL,   msg)
#define PLOG_ERROR(msg)   PLOG(NULL, ::fm::SEV_ERROR,   msg)
#define PLOG_WARNING(msg) PLOG(NULL, ::fm::SEV_WARNING, msg)
#define PLOG_INFO(msg)    PLOG(NULL, ::fm::SEV_INFO,    msg)
#define PLOG_DEBUG(msg)   PLOG(NULL, ::fm::SEV_DEBUG,   msg)

#define DLOG_FATAL(msg)   DLOG(NULL, ::fm::SEV_FATAL,   msg)
#define DLOG_ERROR(msg)   DLOG(NULL, ::fm::SEV_ERROR,   msg)
#define DLOG_WARNING(msg) DLOG(NULL, ::fm::SEV_WARNING, msg)
#define DLOG_INFO(msg)    DLOG(NULL, ::fm::SEV_INFO,    msg)
#define DLOG_DEBUG(msg)   DLOG(NULL, ::fm::SEV_DEBUG,   msg)
#define DLOG_CHECK(cond, msg, deal) \
	if(!(cond)) {					\
		deal;						\
	}
#define DLOG_IF(cond, msg) cond
#define DLOG_TIMER(msg)

}

#endif

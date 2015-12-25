#ifndef _FM_SDK_EXCEPTION_H_
#define _FM_SDK_EXCEPTION_H_

#include <exception>
#include <string>
#include <boost/format.hpp>
#include "SystemExport.h"

namespace fm {

/**
 * @brief 基本异常类。
 *
 * Exception 类是 Titan ImageSDK 中所有异常的基类，派生自标准库中的 std::exception，
 * 除了记录任意的错误描述信息外，可保存异常发生的源代码文件和行数。
 * @note 程序中抛出 Exception 及其派生异常时，请使用宏 THROW/1/2()，同时指定异常的
 *       描述信息和格式化参数(可选)，例如：
 * -     THROW( FileNotFoundException, "config.xml does not exits." );
 * -     THROW1( FileNotFoundException, "%s does not exits.", file_name );
 */
class LIB_SDK Exception : public std::exception
{
public:
	/**
	 * @brief 缺省构造函数。
	 */
	Exception();

	/**
	 * @brief 构造函数。
	 *
	 * @param desc 异常描述信息。
	 */
	Exception(const std::string& desc);

	/**
	 * @brief 拷贝构造函数。
	 */
	Exception(const Exception& other);

	/**
	 * @brief 析构函数。
	 */
	virtual ~Exception() throw();

	/**
	 * @brief 获取异常描述信息。
	 *
	 * @return 异常描述信息。
	 */
	const char * what() const throw();

	/**
	 * @brief 获取异常类型描述。
	 *
	 * @return 异常类型描述。
	 */
	const char* Type() const;

	/**
	 * @brief 获取异常发生的源代码文件名。
	 *
	 * @return 源代码文件名。
	 */
	const std::string& SourceFile() const;

	/**
	 * @brief 获取异常发生的行数。
	 *
	 * @return 源代码行数。
	 */
	int LineNumber() const;

	/**
	 * @brief 设定异常的抛出位置信息，并输出日志。
	 */
	Exception& SetThrowSource(const char* source, int line);

	/**
	 * @brief 抛出指定类型的异常，并设定相关信息。
	 */
	static void Throw(const char* type, const char* desc, const char* source, int line);

private:
	std::string description;

	std::string source_file;

	int line_number;
};

/**
 * @brief 系统异常。基本异常类型之一。
 */
class LIB_SDK SystemException : public Exception
{
public:
	SystemException(const SystemException& other) : Exception(other) { };
	SystemException(const std::string& desc) : Exception(desc) { };
};

/**
 * @brief 无效的库异常。指示加载或使用了无效的链接库。
 */
class LIB_SDK InvalidLibraryException : public SystemException
{
public:
	InvalidLibraryException(const InvalidLibraryException& other) : SystemException(other) { };
	InvalidLibraryException(const std::string& desc) : SystemException(desc) { };
};

/**
 * @brief 文件系统异常。指示访问文件系统时发生异常。
 */
class LIB_SDK FileSystemException : public SystemException
{
public:
	FileSystemException(const FileSystemException& other) : SystemException(other) { };
	FileSystemException(const std::string& desc) : SystemException(desc) { };
};

/**
 * @brief 操作未实现异常。指示请求的接口操作在当前下上文中没有被实现。
 */
class LIB_SDK NotImplementedException : public SystemException
{
public:
	NotImplementedException(const NotImplementedException& other) : SystemException(other) { };
	NotImplementedException(const std::string& desc) : SystemException(desc) { };
};

/**
 * @brief 不支持的操作异常。指示请求的操作不被上下文环境或系统平台支持。
 */
class LIB_SDK NotSupportedException : public SystemException
{
public:
	NotSupportedException(const NotSupportedException& other) : SystemException(other) { };
	NotSupportedException(const std::string& desc) : SystemException(desc) { };
};

/**
 * @brief 安全异常。指示发生了安全验证方面的错误。
 */
class LIB_SDK SecurityException : public SystemException
{
public:
	SecurityException(const SecurityException& other) : SystemException(other) { };
	SecurityException(const std::string& desc) : SystemException(desc) { };
};

/**
 * @brief 访问被拒绝异常。指示操作访问了未被授予权限的资源，或执行了未被授权的操作。
 */
class LIB_SDK PermissionDeniedException : public SecurityException
{
public:
	PermissionDeniedException(const PermissionDeniedException& other) : SecurityException(other) { };
	PermissionDeniedException(const std::string& desc) : SecurityException(desc) { };
};

/**
 * @brief 认证异常。指示登录过程中认证失败。
 */
class LIB_SDK AuthenticationException : public SecurityException
{
public:
	AuthenticationException(const AuthenticationException& other) : SecurityException(other) { };
	AuthenticationException(const std::string& desc) : SecurityException(desc) { };
};

/**
 * @brief 运行时异常。基本异常类型之一。
 */
class LIB_SDK RuntimeException : public Exception
{
public:
	RuntimeException(const RuntimeException& other) : Exception(other) { };
	RuntimeException(const std::string& desc) : Exception(desc) { };
};

/**
 * @brief 参数异常。指示传入的调用参数不符合操作要求。
 */
class LIB_SDK ArgumentException : public RuntimeException
{
public:
	ArgumentException(const ArgumentException& other) : RuntimeException(other) { };
	ArgumentException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 类型异常。指示运行中相关数据对象的类型不符合。
 */
class LIB_SDK TypeErrorException : public RuntimeException
{
public:
	TypeErrorException(const TypeErrorException& other) : RuntimeException(other) { };
	TypeErrorException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 算术异常。指示运行过程中发生计算错误。
 */
class LIB_SDK ArithmeticException : public RuntimeException
{
public:
	ArithmeticException(const ArithmeticException& other) : RuntimeException(other) { };
	ArithmeticException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 库未找到异常。指示需要加载的动态链接库没有找到。
 */
class LIB_SDK LibraryNotFoundException : public RuntimeException
{
public:
	LibraryNotFoundException(const LibraryNotFoundException& other) : RuntimeException(other) { };
	LibraryNotFoundException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 入口点未找到异常。指示动态链接库的指定入口函数没有找到。
 */
class LIB_SDK EntryPointNotFoundException : public RuntimeException
{
public:
	EntryPointNotFoundException(const EntryPointNotFoundException& other) : RuntimeException(other) { };
	EntryPointNotFoundException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 格式异常。指示运行中发生格式方面的错误。
 */
class LIB_SDK FormatException : public RuntimeException
{
public:
	FormatException(const FormatException& other) : RuntimeException(other) { };
	FormatException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 资源定位符格式异常。指示资源定位符URI的格式不正确。
 */
class LIB_SDK UriFormatException : public FormatException
{
public:
	UriFormatException(const UriFormatException& other) : FormatException(other) { };
	UriFormatException(const std::string& desc) : FormatException(desc) { };
};

/**
 * @brief 文件格式异常。指示访问的文件其格式不符合预期的要求。
 */
class LIB_SDK FileFormatException : public FormatException
{
public:
	FileFormatException(const UriFormatException& other) : FormatException(other) { };
	FileFormatException(const std::string& desc) : FormatException(desc) { };
};

/**
 * @brief 路径未找到异常。指示指定的路径并不存在。
 */
class LIB_SDK PathNotExistException : public RuntimeException
{
public:
	PathNotExistException(const PathNotExistException& other) : RuntimeException(other) { };
	PathNotExistException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 文件未找到异常。指示需要的文件没有找到。
 */
class LIB_SDK FileNotFoundException : public RuntimeException
{
public:
	FileNotFoundException(const FileNotFoundException& other) : RuntimeException(other) { };
	FileNotFoundException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 主机未找到异常。指示要连接或者解析的主机没有找到。
 */
class LIB_SDK HostNotFoundException : public RuntimeException
{
public:
	HostNotFoundException(const LibraryNotFoundException& other) : RuntimeException(other) { };
	HostNotFoundException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 索引超出范围异常。指示访问序列数据时索引超出了允许的最大范围。
 */
class LIB_SDK IndexOutOfRangeException : public RuntimeException
{
public:
	IndexOutOfRangeException(const IndexOutOfRangeException& other) : RuntimeException(other) { };
	IndexOutOfRangeException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 无效的转换异常。指示数据类型或者接口类型转换无效。
 */
class LIB_SDK InvalidCastException : public RuntimeException
{
public:
	InvalidCastException(const InvalidCastException& other) : RuntimeException(other) { };
	InvalidCastException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 无效的操作异常。指示在当前上下文环境中请求的操作无效。
 */
class LIB_SDK InvalidOperationException : public RuntimeException
{
public:
	InvalidOperationException(const InvalidOperationException& other) : RuntimeException(other) { };
	InvalidOperationException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 空指针异常。指示在执行或计算过程中发现意外的空指针。
 */
class LIB_SDK NullPointerException : public RuntimeException
{
public:
	NullPointerException(const NullPointerException& other) : RuntimeException(other) { };
	NullPointerException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 数据锁定异常。指示要访问的数据被其它用户或程序锁定。
 */
class LIB_SDK DataLockedException : public RuntimeException
{
public:
	DataLockedException(const DataLockedException& other) : RuntimeException(other) { };
	DataLockedException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 远程操作异常。指示调用的远程操作中发生异常。
 */
class LIB_SDK RemoteOperationException : public RuntimeException
{
public:
	RemoteOperationException(const RemoteOperationException& other) : RuntimeException(other) { };
	RemoteOperationException(const std::string& desc) : RuntimeException(desc) { };
};

/**
 * @brief 逻辑异常。基本异常类型之一。
 */
class LIB_SDK LogicalException : public Exception
{
public:
	LogicalException(const LogicalException& other) : Exception(other) { };
	LogicalException(const std::string& desc) : Exception(desc) { };
};

/**
 * @brief 输入/输出异常。基本异常类型之一。
 */
class LIB_SDK IOException : public Exception
{
public:
	IOException(const IOException& other) : Exception(other) { };
	IOException(const std::string& desc) : Exception(desc) { };
};

/**
 * @brief 文件读写异常。指示在读写文件过程中发生了错误。
 */
class LIB_SDK FileIOException : public IOException
{
public:
	FileIOException(const IOException& other) : IOException(other) { };
	FileIOException(const std::string& desc) : IOException(desc) { };
};

/**
 * @brief 数据库访问异常。指示在访问数据库过程中发生了错误。
 */
class LIB_SDK DatabaseIOException : public IOException
{
public:
	DatabaseIOException(const DatabaseIOException& other) : IOException(other) { };
	DatabaseIOException(const std::string& desc) : IOException(desc) { };
};

/**
 * @brief 网络访问异常。指示在通过网络读写数据过程中发生了错误。
 */
class LIB_SDK NetworkIOException : public IOException
{
public:
	NetworkIOException(const DatabaseIOException& other) : IOException(other) { };
	NetworkIOException(const std::string& desc) : IOException(desc) { };
};

/**
 * @brief 套接字异常。指示套接字访问发生错误。
 */
class LIB_SDK SocketException : public NetworkIOException
{
public:
	SocketException(const SocketException& other) : NetworkIOException(other) { };
	SocketException(const std::string& desc) : NetworkIOException(desc) { };
};

/**
 * @brief 协议异常。指示网络协议不匹配或无法满足要求。
 */
class LIB_SDK ProtocolException : public NetworkIOException
{
public:
	ProtocolException(const ProtocolException& other) : NetworkIOException(other) { };
	ProtocolException(const std::string& desc) : NetworkIOException(desc) { };
};

template<typename ExceptionType>
ExceptionType MakeException(const std::string& msg, const char* source, int line)
{
	ExceptionType ex(msg);
	ex.SetThrowSource(source, line);
	return ex;
}

#if !defined(THROW) || !defined(DISABLE_FM_THROW)
#undef  THROW
#define THROW(exp, desc) do{ std::ostringstream oss; oss<<desc; throw ::fm::MakeException<exp>(oss.str(), __FILE__, __LINE__); } while(0)
#endif
#define THROW_EXCEPT(exp, desc) do{ std::ostringstream oss; oss<<desc; throw ::fm::MakeException<exp>(oss.str(), __FILE__, __LINE__); } while(0)
#define RETHROW(type, desc, src, line) ::fm::Exception::Throw(type, desc, src, line)

}

#endif

#ifndef _FM_SDK_ERROR_H_
#define _FM_SDK_ERROR_H_

#include "SystemExport.h"

namespace fm {

const int ErrNone               = 0;
const int ErrError              = 1000;
const int ErrSystem             = 1100;
const int ErrInvalidLibrary     = 1101;
const int ErrFileSystem         = 1102;
const int ErrNotImplemented     = 1103;
const int ErrNotSupported       = 1104;
const int ErrSecurity           = 1105;
const int ErrPermissionDenied   = 1106;
const int ErrAuthentication     = 1107;
const int ErrRuntime            = 1200;
const int ErrArgument           = 1200;
const int ErrArithmetic         = 1201;
const int ErrLibraryNotFound    = 1202;
const int ErrEntryPointNotFound = 1203;
const int ErrFormat             = 1204;
const int ErrUriFormat          = 1205;
const int ErrFileFormat         = 1206;
const int ErrPathNotExist       = 1207;
const int ErrFileNotFound       = 1208;
const int ErrHostNotFound       = 1209;
const int ErrIndexOutOfRange    = 1210;
const int ErrInvalidCast        = 1211;
const int ErrInvalidOperation   = 1212;
const int ErrNullPointer        = 1213;
const int ErrDataLocked         = 1214;
const int ErrRemoteOperation    = 1215;
const int ErrLogical            = 1300;
const int ErrIO                 = 1400;
const int ErrFileIO             = 1401;
const int ErrDatabaseIO         = 1402;
const int ErrNetworkIO          = 1403;
const int ErrSocket             = 1404;
const int ErrProtocol           = 1405;
const int ErrUserCancelled      = 1500;

/**
 * @brief 错误类。
 *
 * Error 表示可返回的错误对象。Error对象包含必要的错误描述和位置信息，并且支持布尔值的判断语法。
 */
class LIB_SDK Error
{
public:
	/**
	 * @brief 缺省构造函数，构建通用错误对象
	 */
	Error();

	/**
	 * @brief 构造函数。
	 *
	 * @param code 错误码。
	 */
	Error(int code);

	/**
	 * @brief 构造函数。
	 *
	 * @param desc 错误描述信息。
	 */
	Error(const std::string& desc);

	/**
	 * @brief 构造函数。
	 *
	 * @param code 错误码。
	 * @param desc 错误描述信息。
	 */
	Error(int code, const std::string& desc);

	/**
	 * @brief 拷贝构造函数。
	 */
	Error(const Error& other);

	/**
	 * @brief 获取错误描述信息。
	 *
	 * @return 错误描述信息。
	 */
	const std::string& Description() const;

	/**
	 * @brief 获取指定的错误码。
	 *
	 * @return 错误码。
	 */
	int ErrorCode() const;

	/**
	 * @brief 获取错误发生的源代码文件名。
	 *
	 * @return 源代码文件名。
	 */
	const char* SourceFile() const;

	/**
	 * @brief 获取错误发生的行数。
	 *
	 * @return 源代码行数。
	 */
	int LineNumber() const;

	/**
	 * @brief 设定返回错误的位置信息，并输出日志。
	 */
	Error& SetErrorSource(const char* source, int line);

	/**
	 * @brief 设定返回标准错误的位置信息，并输出日志。
	 */
	Error& SetStdErrorSource(const char* source, int line);

	typedef const Error* unspecified_bool_type;

    operator unspecified_bool_type() const
    {
        return (error_code == 0) ? this : 0;
    }

    bool operator!() const
    {
        return error_code != 0;
    }

private:
	int error_code;

	std::string description;

	const char* source_file;

	int line_number;
};

LIB_SDK bool operator!=(const Error& lhs, int rhs);

LIB_SDK bool operator==(const Error& lhs, int rhs);

LIB_SDK Error MakeError(bool perr, int code, const std::string& msg, const char* source, int line);

#define RETURN_ERROR(desc)           do{ std::ostringstream oss; oss<<desc; return ::fm::MakeError(false, ErrError, oss.str(), __FILE__, __LINE__); } while(0)
#define RETURN_PERROR(desc)          do{ std::ostringstream oss; oss<<desc; return ::fm::MakeError(true,  ErrError, oss.str(), __FILE__, __LINE__); } while(0)
#define RETURN_ERRCODE(code, desc)   do{ std::ostringstream oss; oss<<desc; return ::fm::MakeError(false, code,     oss.str(), __FILE__, __LINE__); } while(0)
#define RETURN_PERRCODE(code, desc)  do{ std::ostringstream oss; oss<<desc; return ::fm::MakeError(true,  code,     oss.str(), __FILE__, __LINE__); } while(0)

#define MAKE_ERROR(err, code, desc)  err = ::fm::MakeError(false, code, desc, __FILE__, __LINE__)

}

#endif

#include "Exception.h"
#include "Logging.h"
#include "FileSystem.h"

namespace fm {

Exception::Exception() : line_number(0)
{
}

Exception::Exception(const Exception& other) : std::exception(other)
{
	description = other.description;
	source_file = other.source_file;
	line_number = other.line_number;
}

Exception::Exception(const std::string& desc)
	: description(desc),
	  line_number(0)
{
}

Exception::~Exception() throw ()
{
}

const std::string& Exception::SourceFile() const
{
	return source_file;
}

int Exception::LineNumber() const
{
	return line_number;
}

const char* Exception::Type() const
{
	const char* exp_type = typeid(*this).name();
	const char* name = strchr(exp_type, ' ');
	if( name != NULL )
		return ++name;
	return exp_type;
}

const char* Exception::what() const throw ()
{
	return description.c_str();
}

Exception& Exception::SetThrowSource(const char* source, int line)
{
	if (source)
		source_file = Path(source).FileName();
	line_number = line;
	LOG_ERROR(Type()<<"("<<source_file<<":"<<line_number<<"): "<<description);
	return *this;
}

void Exception::Throw(const char* type, const char* desc, const char* source, int line)
{
	if (strcmp(type, "IOException") == 0)
		throw IOException(desc).SetThrowSource(source, line);
	if (strcmp(type, "FileIOException") == 0)
		throw FileIOException(desc).SetThrowSource(source, line);
	if (strcmp(type, "DatabaseIOException") == 0)
		throw DatabaseIOException(desc).SetThrowSource(source, line);
	if (strcmp(type, "NetworkIOException") == 0)
		throw NetworkIOException(desc).SetThrowSource(source, line);
	if (strcmp(type, "SocketException") == 0)
		throw SocketException(desc).SetThrowSource(source, line);
	if (strcmp(type, "FileSystemException") == 0)
		throw FileSystemException(desc).SetThrowSource(source, line);
	if (strcmp(type, "RuntimeException") == 0)
		throw RuntimeException(desc).SetThrowSource(source, line);
	if (strcmp(type, "LibraryNotFoundException") == 0)
		throw LibraryNotFoundException(desc).SetThrowSource(source, line);
	if (strcmp(type, "EntryPointNotFoundException") == 0)
		throw EntryPointNotFoundException(desc).SetThrowSource(source, line);
	if (strcmp(type, "FormatException") == 0)
		throw FormatException(desc).SetThrowSource(source, line);
	if (strcmp(type, "UriFormatException") == 0)
		throw UriFormatException(desc).SetThrowSource(source, line);
	if (strcmp(type, "FileFormatException") == 0)
		throw FileFormatException(desc).SetThrowSource(source, line);
	if (strcmp(type, "PathNotExistException") == 0)
		throw PathNotExistException(desc).SetThrowSource(source, line);
	if (strcmp(type, "FileNotFoundException") == 0)
		throw FileNotFoundException(desc).SetThrowSource(source, line);
	if (strcmp(type, "HostNotFoundException") == 0)
		throw HostNotFoundException(desc).SetThrowSource(source, line);
	if (strcmp(type, "IndexOutOfRangeException") == 0)
		throw IndexOutOfRangeException(desc).SetThrowSource(source, line);
	if (strcmp(type, "InvalidCastException") == 0)
		throw InvalidCastException(desc).SetThrowSource(source, line);
	if (strcmp(type, "InvalidOperationException") == 0)
		throw InvalidOperationException(desc).SetThrowSource(source, line);
	if (strcmp(type, "ArgumentException") == 0)
		throw ArgumentException(desc).SetThrowSource(source, line);
	if (strcmp(type, "ArithmeticException") == 0)
		throw ArithmeticException(desc).SetThrowSource(source, line);
	if (strcmp(type, "NullPointerException") == 0)
		throw NullPointerException(desc).SetThrowSource(source, line);
	if (strcmp(type, "DataLockedException") == 0)
		throw DataLockedException(desc).SetThrowSource(source, line);
	if (strcmp(type, "RemoteOperationException") == 0)
		throw RemoteOperationException(desc).SetThrowSource(source, line);
	if (strcmp(type, "LogicalException") == 0)
		throw LogicalException(desc).SetThrowSource(source, line);
	if (strcmp(type, "ProtocolException") == 0)
		throw ProtocolException(desc).SetThrowSource(source, line);
	if (strcmp(type, "SystemException") == 0)
		throw SystemException(desc).SetThrowSource(source, line);
	if (strcmp(type, "InvalidLibraryException") == 0)
		throw InvalidLibraryException(desc).SetThrowSource(source, line);
	if (strcmp(type, "NotImplementedException") == 0)
		throw NotImplementedException(desc).SetThrowSource(source, line);
	if (strcmp(type, "NotSupportedException") == 0)
		throw NotSupportedException(desc).SetThrowSource(source, line);
	if (strcmp(type, "SecurityException") == 0)
		throw SecurityException(desc).SetThrowSource(source, line);
	if (strcmp(type, "PermissionDeniedException") == 0)
		throw PermissionDeniedException(desc).SetThrowSource(source, line);
	if (strcmp(type, "AuthenticationException") == 0)
		throw AuthenticationException(desc).SetThrowSource(source, line);
	throw Exception(desc).SetThrowSource(source, line);
}

}

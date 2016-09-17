#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <stdarg.h>
#include "StringUtil.h"

namespace fm{

std::string StringUtil::ConvertIntToString(int value)
{
	std::string ret = boost::lexical_cast<std::string>(value);
	return ret;
}

std::string StringUtil::ConvertFloatToString(float value)
{
	std::string ret = boost::lexical_cast<std::string>(value);
	return ret;
}

std::string StringUtil::ConvertDoubleToString(double value)
{
	std::string ret = boost::lexical_cast<std::string>(value);
	return ret;
}
int StringUtil::ConvertStringToInt(std::string value)
{
	int ret = boost::lexical_cast<int>(value);
	return ret;
}
float StringUtil::ConvertStringToFloat(std::string value)
{
	float ret = boost::lexical_cast<float>(value);
	return ret;
}
double StringUtil::ConvertStringToDouble(std::string value)
{
	double ret = boost::lexical_cast<double>(value);
	return ret;
}

void StringUtil::Format(std::string& str,  const char* fmt , ...)
{
	int bufSize   = 256;
	int retryTime = 1;
	char* szBuffer = NULL;

	va_list ap;  
	va_start(ap, fmt); 

	while (true){
		szBuffer = new char[bufSize * retryTime];
		memset(szBuffer, 0, bufSize * retryTime);

		if (vsnprintf(szBuffer, bufSize * retryTime, fmt, ap) < 0){
			retryTime++;
			delete []szBuffer;
			szBuffer = NULL;
		} else {
			break;
		}
	}
		   
	va_end(ap); 
	if (szBuffer){
		str = szBuffer;  
		delete []szBuffer;
		szBuffer = NULL;
	}
	
	return;  
}

void StringUtil::MakeUpper(std::string& str)
{
	boost::to_upper(str);
}

void StringUtil::MakeLower(std::string& str)
{
	boost::to_lower(str);
}

bool StringUtil::IsEqual(const std::string& lhs, const std::string& rhs, bool bNoCase)
{
	if (bNoCase)
		return boost::equals(lhs, rhs);
	else
		return boost::iequals(lhs, rhs);
}
}
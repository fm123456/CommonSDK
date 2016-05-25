#include <boost/lexical_cast.hpp>
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
}
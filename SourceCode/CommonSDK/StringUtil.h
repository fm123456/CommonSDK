#ifndef _FM_SDK_STRINGUTIL_H_
#define _FM_SDK_STRINGUTIL_H_

#include "SystemExport.h"

namespace fm {

/**
* @brief 通用字符串处理类
*
* StringUtil 提供了日常编程中经常用到的字符串处理函数
*/
class LIB_SDK StringUtil
{
	/** 
	* @brief  32位有符号整型转成字符串
	*
	* @param  value 整型值
	* @return 返回转换后的字符串值
	*/
	static std::string ConvertIntToString(int value);

	/**
	* @brief  float类型转成字符串
	*
	* @param  value float类型值
	* @return 返回转换后的字符串值
	*/
	static std::string ConvertFloatToString(float value);

	/**
	* @brief  double类型转成字符串
	*
	* @param  value double类型值
	* @return 返回转换后的字符串值
	*/
	static std::string ConvertDoubleToString(double value);

	/**
	* @brief  字符串转换成Int
	*
	* @param  value 需要转换的字符串
	* @return 返回转换后的int值
	*/
	static int ConvertStringToInt(std::string value);

	/**
	* @brief  字符串转换成Int
	*
	* @param  value 需要转换的字符串
	* @return 返回转换后的float值
	*/
	static float ConvertStringToFloat(std::string value);

	/**
	* @brief  字符串转换成Int
	*
	* @param  value 需要转换的字符串
	* @return 返回转换后的double值
	*/
	static double ConvertStringToDouble(std::string value);
};

}

#endif
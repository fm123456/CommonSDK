#ifndef _FM_SDK_DATE_TIME_H_
#define _FM_SDK_DATE_TIME_H_

#include <string>
#include <ostream>
#include "SystemExport.h"

namespace fm {

/**
 * @brief 通用日期对象。
 *
 * Date 类实现了通用日期对象，包含了日期相关的常用操作，如信息查询和格式化等。
 */
class LIB_SDK Date
{
public:
	/**
	 * @brief 默认构造函数。
	 *
	 * @note 使用默认构造函数构造的日期对象不是一个有效的日期。
	 */
	Date();

	/**
	 * @brief 构造函数。
	 *
	 * @param year 日期中的年。
	 * @param month 日期中的月。
	 * @param day 日期中的日。
	 */
	Date(unsigned short year, unsigned short month, unsigned short day);

	/**
	 * @brief 构造函数。
	 *
	 * @param date_str 形如“YYYY-MM-DD”的日期文本。
	 */
	Date(const std::string& date_str);
	
	/**
	 * @brief 拷贝构造函数。
	 */
	 Date(const Date& other);

	 /**
	 * @brief 获取当前的日期。
	 *
	 * @return 当前的日期。
	 */
	 static Date Now();

	/**
	 * @brief 将日期格式化为字符串。
	 *
	 * @return 形如“YYYY-MM-DD”的字符串表示。
	 */
	std::string FormatString() const;

	/**
	 * @brief 获取日。
	 *
	 * @return 表示的月中的第几天。 
	 */
	inline unsigned short GetDay() const
	{
		return day;
	}

	/**
	 * @brief 获取月。
	 *
	 * @return 表示的年中的第几月。 
	 */
	inline unsigned short GetMonth() const
	{
		return month;
	}
	
	/**
	 * @brief 获取年。
	 *
	 * @return 对象表示的年。 
	 */
	inline unsigned short GetYear() const
	{
		return year;
	}

	/**
	 * @brief 判断是否为有效日期。
	 *
	 * @return 有效日期返回true，否则返回false。
	 */
	bool IsValid() const;

	/**
	 * @brief =操作符重载函数。
	 */
	Date& operator=(const Date& other);

	/**
	 * @brief 设置日。
	 *
	 * @param day 要设置的日。
	 */
	inline void SetDay(unsigned short day)
	{
		this->day = day;
	}

	/**
	 * @brief 设置月。
	 *
	 * @param month 要设置的月。
	 */
	inline void SetMonth(unsigned short month)
	{
		this->month = month;
	}

	/**
	 * @brief 设置年。
	 *
	 * @param year 要设置的年。
	 */
	inline void SetYear(unsigned short year)
	{
		this->year = year;
	}

private:
	unsigned short year;
	
	unsigned short month;

	unsigned short day;
};

/**
 * @brief 通用时间对象。
 *
 * Time 类实现了通用时间对象，包含了时间相关的常用操作，如信息查询和格式化等。
 */
class LIB_SDK Time
{
public:
	/**
	 * @brief 构造函数。
	 */
	Time();

	/**
	 * @brief 构造函数。
	 *
	 * @param year 时间中的年。
	 * @param month 时间中的月。
	 * @param day 时间中的日。
	 * @param hour 时间中的时。
	 * @param minute 时间中的分。
	 * @param second 时间中的秒。
	 */
	Time(unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short second);

	/**
	 * @brief 拷贝构造函数。
	 */
	Time(const Time& other);

	/**
	 * @brief 构造函数。
	 *
	 * @param time_str 形如“YYYY-MM-DD 24h:mm:ss”的时间文本。
	 */
	Time(const std::string& time_str);

	 /**
	 * @brief 获取当前的时间。
	 *
	 * @return 当前的时间。
	 */
	 static Time Now();

	/**
	 * @brief 将时间格式化为字符串。
	 *
	 * @return 形如“YYYY-MM-DD 24h:mm:ss”的字符串表示。 
	 */
	std::string FormatString() const;

	/**
	 * @brief 设为字符串表达的时间。
	 *
	 * @param time_str 形如“YYYY-MM-DD 24h:mm:ss”的时间文本。
	 */
	void FromString(const std::string& time_str);

	/**
	 * @brief 判断是否为有效时间。
	 *
	 * @return 有效时间返回true，否则返回false。
	 */
	bool IsValid() const;

	/**
	 * @brief 赋值操作符重载。
	 */
	Time& operator=(const Time& other);

	/**
	 * @brief 赋值操作符重载。
	 */
	Time& operator=(time_t t);

	/**
	 * @brief 类型转换操作符。
	 */
	operator time_t() const;

	/**
	 * @brief 赋值操作符重载。
	 */
	Time& operator=(const struct tm& time);

	/**
	 * @brief 获取表示时间的日期。
	 *
	 * @return 表示的日期。 
	 */
	inline const Date& GetDate() const
	{
		return date;
	}

	/**
	 * @brief 获取表示时间的小时。
	 *
	 * @return 表示的小时。 
	 */
	inline unsigned short GetHour() const
	{
		return hour;
	}

	/**
	 * @brief 获取表示时间的分钟。
	 *
	 * @return 表示的分钟。 
	 */
	inline unsigned short GetMinute() const
	{
		return minute;
	}

	/**
	 * @brief 获取表示时间的秒。
	 *
	 * @return 表示的秒。 
	 */
	inline unsigned short GetSecond() const
	{
		return second;
	}

	/**
	 * @brief 设置表示的日期。
	 *
	 * @param date 待设置的日期。
	 */
	inline void SetDate(const Date& date)
	{
		this->date = date;
	}

	/**
	 * @brief 设置表示的小时。
	 *
	 * @param hour 待设置的小时。
	 */
	inline void SetHour(unsigned short hour)
	{
		this->hour = hour;
	}

	/**
	 * @brief 设置表示的分钟。
	 *
	 * @param m 待设置的分钟。
	 */
	inline void SetMinute(unsigned short minute)
	{
		this->minute = minute;
	}

	/**
	 * @brief 设置表示的秒。
	 *
	 * @param s 待设置的秒。
	 */
	inline void SetSecond(unsigned short second)
	{
		this->second = second;
	}

	LIB_SDK friend bool operator==(const Time& lhs, const Time& rhs);

	LIB_SDK friend bool operator!=(const Time& lhs, const Time& rhs);

	LIB_SDK friend bool operator>(const Time& lhs,  const Time& rhs);

	LIB_SDK friend bool operator>=(const Time& lhs, const Time& rhs);

	LIB_SDK friend bool operator<(const Time& lhs,  const Time& rhs);

	LIB_SDK friend bool operator<=(const Time& lhs, const Time& rhs);

	LIB_SDK friend std::ostream& operator<<(std::ostream &os, const Time &rhs);

private:
	Date date;

	unsigned short hour;
	
	unsigned short minute;

	unsigned short second;
};

}

#endif

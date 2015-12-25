#include <boost/date_time.hpp>
#include "DateTime.h"
using namespace std;

namespace fm {

Date::Date() : year(0), month(0), day(0)
{
}

Date::Date(unsigned short year, unsigned short month, unsigned short day)
{
	this->year = year;
	this->month = month;
	this->day = day;
}

Date::Date(const std::string& date_str)
{
	int year, month, day;
	if (sscanf(date_str.c_str(), "%4d-%02d-%02d", &year, &month, &day) != 3)
		return;
	this->year  = year;
	this->month = month;
	this->day   = day;
}

Date::Date(const Date& other)
{
	year  = other.year;
	month = other.month;
	day   = other.day;
}

Date Date::Now()
{
	boost::gregorian::date dt = boost::posix_time::second_clock::local_time().date();
	return Date(dt.year(), dt.month(), dt.day());
}

std::string Date::FormatString() const
{
	return (boost::format("%4d-%02d-%02d") % year % month % day).str();
}

bool Date::IsValid() const
{
	if(year < 1753 || year > 9999)
		return false;
	if(month < 1 || month > 12)
		return false;
	if(day < 1) return false;
	switch( month )
	{
	case 1:
		if(day > 31) return false;
		break;
	case 2:
		if(year % 4 == 0 && day > 29)
			return false;
		if(year % 4 != 0 && day > 28)
			return false;
		break;
	case 3:
		if(day > 31) return false;
		break;
	case 4:
		if(day > 30) return false;
		break;
	case 5:
		if(day > 31) return false;
		break;
	case 6:
		if(day > 30) return false;
		break;
	case 7:
		if(day > 31) return false;
		break;
	case 8:
		if(day > 31) return false;
		break;
	case 9:
		if(day > 30) return false;
		break;
	case 10:
		if(day > 31) return false;
		break;
	case 11:
		if(day > 30) return false;
		break;
	case 12:
		if(day > 31) return false;
		break;
	}
	return true;
}

Date& Date::operator=(const Date& other)
{
	if(this == &other)
		return *this;
	year  = other.year;
	month = other.month;
	day   = other.day;
	return *this;
}

Time::Time() : hour(-1), minute(-1), second(-1)
{
}

Time::Time(unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute, unsigned short second) : date(year, month, day)
{
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

Time::Time(const Time& other) : date(other.date)
{
	hour = other.hour;
	minute = other.minute;
	second = other.second;
}

Time::Time(const std::string& time_str) : hour(0), minute(0), second(0)
{
	int year, month, day, hour, minute, second;
	if (sscanf(time_str.c_str(), "%4d-%02d-%02d %02d:%02d:%02d", &year, &month, &day,
			&hour, &minute, &second) != 6)
		return;
	date.SetYear(year);
	date.SetMonth(month);
	date.SetDay(day);
	this->hour   = hour;
	this->minute = minute;
	this->second = second;
}

Time Time::Now()
{
	boost::posix_time::ptime lt(boost::posix_time::second_clock::local_time());
	boost::gregorian::date dt = lt.date();
	boost::posix_time::time_duration td = lt.time_of_day();
	return Time(dt.year(), dt.month(), dt.day(), td.hours(), td.minutes(), td.seconds());
}

std::string Time::FormatString() const
{
	return (boost::format("%4d-%02d-%02d %02d:%02d:%02d") % date.GetYear() % date.GetMonth() % date.GetDay() % hour % minute % second).str();
}

void Time::FromString(const std::string& time_str)
{
	*this = Time(time_str);
}

bool Time::IsValid() const
{
	if( !date.IsValid() )
      return false;
	if( hour < 0 || hour > 23)
	  return false;
	if( minute < 0 || minute > 59)
	  return false;
	if( second < 0 || second > 59)
	  return false;
	return true;
}

Time& Time::operator=(const Time& other)
{
	if(this == &other)
		return *this;
	date   = other.date;
	hour   = other.hour;
	minute = other.minute;
	second = other.second;
	return *this;
}

Time& Time::operator=(time_t t)
{
	if (t == 0) {
		date = Date();
		hour = minute = second = 0;
		return *this;
	}

	tm tmp;
#ifdef _WINDOWS
	localtime_s(&tmp, &t);
#else
	localtime_r(&t, &tmp);
#endif
	this->date.SetYear(tmp.tm_year+1900);
	this->date.SetMonth(tmp.tm_mon+1);
	this->date.SetDay(tmp.tm_mday);
	this->hour   = tmp.tm_hour;
	this->minute = tmp.tm_min;
	this->second = tmp.tm_sec;
	return *this;
}

Time::operator time_t() const
{
	struct tm t;
	t.tm_year = date.GetYear() - 1900;
	t.tm_mon  = date.GetMonth() - 1;
	t.tm_mday = date.GetDay();
	t.tm_hour = hour;
	t.tm_min  = minute;
	t.tm_sec  = second;
	t.tm_isdst = 0;
	return mktime(&t);
}

Time& Time::operator=(const struct tm& time)
{
	date.SetYear(time.tm_year+1900);
	date.SetMonth(time.tm_mon+1);
	date.SetDay(time.tm_mday);
	this->hour   = time.tm_hour;
	this->minute = time.tm_min;
	this->second = time.tm_sec;
	return *this;
}

bool operator==(const Time& lhs, const Time& rhs)
{
	return lhs.date.GetYear()==rhs.date.GetYear() && lhs.date.GetMonth()==rhs.date.GetMonth() && lhs.date.GetDay()==rhs.date.GetDay()
			&& lhs.hour==rhs.hour && lhs.minute==rhs.minute && lhs.second==rhs.second;
}

bool operator!=(const Time& lhs, const Time& rhs)
{
	return !(lhs==rhs);
}

bool operator<(const Time& lhs, const Time& rhs)
{
	if(lhs.date.GetYear() != rhs.date.GetYear() )
		return lhs.date.GetYear() < rhs.date.GetYear();
	else {
		if(lhs.date.GetMonth() != rhs.date.GetMonth() )
			return lhs.date.GetMonth() < rhs.date.GetMonth();
		else {
			if(lhs.date.GetDay() != rhs.date.GetDay())
				return lhs.date.GetDay() < rhs.date.GetDay();
			else {
				if(lhs.hour != rhs.hour)
					return lhs.hour < rhs.hour ;
				else {
					if(lhs.minute != rhs.minute)
						return lhs.minute < rhs.minute;
					else {
						if(lhs.second != rhs.second)
							return lhs.second < rhs.second;
						else
							return false;
					}
				}
			}
		}
	}
}

bool operator>(const Time& lhs, const Time& rhs)
{
	return !(rhs < lhs) && (rhs != lhs);
}

bool operator<=(const Time& lhs, const Time& rhs)
{
	return !(rhs > lhs);
}

bool operator>=(const Time& lhs, const Time& rhs)
{
	return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream &os, const Time& time)
{
	os<<time.FormatString();
	return os;
}

}

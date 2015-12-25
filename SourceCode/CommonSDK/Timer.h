#ifndef _FM_SDK_TIMER_H_
#define _FM_SDK_TIMER_H_

#include "SystemExport.h"

namespace fm {

/**
 * @brief 高精度计时器。
 *
 * Timer 类支持精确到纳秒级别的高精度计时。
 */
class LIB_SDK Timer
{
public:
	/**
	 * @brief 构造函数，设定计时的起始时间。
	 */
	Timer();

	/**
	 * @brief 重设计时器，设定计时的起始时间。
	 */
	void Reset();

	/**
	 * @brief 获取从起始时间到当前逝去的纳秒数。
	 *
	 * @return 逝去的纳秒数。
	 */
	long long Interval() const;

	/**
	 * @brief 获取从起始时间到当前逝去的秒数。
	 *
	 * @return 逝去的秒数。
	 */
	double Seconds() const;

	/**
	 * @brief 获取当前的纳秒级时间值。
	 *
	 * @return 当前时间值。
	 */
	static long long Now();

private:
	long long value;
};

}

#endif

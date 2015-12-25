#ifndef _FM_SDK_TIMER_H_
#define _FM_SDK_TIMER_H_

#include "SystemExport.h"

namespace fm {

/**
 * @brief �߾��ȼ�ʱ����
 *
 * Timer ��֧�־�ȷ�����뼶��ĸ߾��ȼ�ʱ��
 */
class LIB_SDK Timer
{
public:
	/**
	 * @brief ���캯�����趨��ʱ����ʼʱ�䡣
	 */
	Timer();

	/**
	 * @brief �����ʱ�����趨��ʱ����ʼʱ�䡣
	 */
	void Reset();

	/**
	 * @brief ��ȡ����ʼʱ�䵽��ǰ��ȥ����������
	 *
	 * @return ��ȥ����������
	 */
	long long Interval() const;

	/**
	 * @brief ��ȡ����ʼʱ�䵽��ǰ��ȥ��������
	 *
	 * @return ��ȥ��������
	 */
	double Seconds() const;

	/**
	 * @brief ��ȡ��ǰ�����뼶ʱ��ֵ��
	 *
	 * @return ��ǰʱ��ֵ��
	 */
	static long long Now();

private:
	long long value;
};

}

#endif

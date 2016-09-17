#ifndef _FM_SDK_THREADTASK_H_
#define _FM_SDK_THREADTASK_H_

#include "SystemExport.h"

namespace fm{

/**
 * @brief 线程执行任务基类
 */

class LIB_SDK ThreadTask
{
public:
	/**
	 * @brief 线程任务执行函数
     */
	virtual void Execute() = 0;
};

typedef boost::shared_ptr<ThreadTask> ThreadTaskPtr;

}

#endif
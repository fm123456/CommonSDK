#ifndef _FM_SDK_THREADPOOL_H_
#define _FM_SDK_THREADPOOL_H_

#include "SystemExport.h"
#include "ThreadTask.h"

namespace fm{

/**
 * @brief 线程池基类
 */

class LIB_SDK ThreadPool
{
public:
	/**
     * @brief 终止线程池内的所有线程，调用该方法后调用 Join()等待线程结束。请确保当前正在执行的任务能够收到
	 *        结束通知
     */
	virtual void Terminate() = 0;

	/**
     * @brief 等待线程池内的所有线程结束
	 *        
     */
	virtual void Join() = 0;

	/**
     * @brief 调整线程池内线程大小
	 *   
	 * @param[in] threadNum 调整后线程的大小（必须是大于正整数）
     */
	virtual void AdjustThreadNum(int threadNum) = 0;

	/**
     * @brief 增加线程任务
	 *   
	 * @param[in] task 线程任务
     */
	virtual void PushTask(ThreadTaskPtr task) = 0;

	/**
    * @brief 获取任务，如果任务队列为空则阻塞
	*
	* @return 返回下一个任务对象
    */
	virtual ThreadTaskPtr PopTask() = 0;
};

typedef boost::shared_ptr<ThreadPool> ThreadPoolPtr;

/**
 * @brief 线程池创建的对外统一接口
 *   
 * @param[in] threadNum 线程池的大小
 *
 * @return 返回线程池对象
 */
LIB_SDK ThreadPoolPtr CreateThreadPool(int threadNum);

}

#endif
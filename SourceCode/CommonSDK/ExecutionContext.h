#ifndef _FM_SDK_EXECUTION_CONTEXT_H_
#define _FM_SDK_EXECUTION_CONTEXT_H_

#include <string>
#include <vector>
#include "SystemExport.h"

namespace fm {

const int RESOURCE_UNLIMITED = -1;

/**
 * @brief 执行环境信息。
 *
 * ExecutionContext 类描述了算法或程序执行的环境信息，为它们的优化执行
 * 提供相关的基本信息依据。
 */
class LIB_SDK ExecutionContext
{
public:
	struct ResourceLimit
	{
		ResourceLimit();
		~ResourceLimit();

		int  limited_size;
		int* resource_id;
	};

	/**
	 * @brief 构造函数。
	 */
	ExecutionContext();

	/**
	 * @brief 析构函数。
	 */
	~ExecutionContext();

	/**
	 * @brief 获取当前设定的执行环境。
	 *
	 * @return 当前执行环境
	 */
	static ExecutionContext& GetCurrent();

	/**
	 * @brief 将当前环境设定为桌面/工作站执行环境。
	 */
	static void UseDesktopContext(bool enable_gpus = false);

	/**
	 * @brief 将当前环境设定为专用计算节点执行环境。
	 */
	static void UseComputeContext(bool exclusive = false, 
		                          const ResourceLimit* rcpu = NULL,
								  const ResourceLimit* rgpu = NULL,
								  const ResourceLimit* rmem = NULL);

	bool multi_threaded;

	bool io_thread_allowed;

	bool big_memory;

	ResourceLimit rlimit_cpu;

	ResourceLimit rlimit_gpu;

	ResourceLimit rlimit_mem;
};

}

#endif

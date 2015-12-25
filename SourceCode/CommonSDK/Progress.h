#ifndef _FM_SDK_PROGRESS_H_
#define _FM_SDK_PROGRESS_H_

#include "SystemExport.h"

namespace fm {

/**
 * @brief 通用进度接口。
 *
 * Progress 接口定义了通用的进度报告接口，可用于程序不同模块之间的进度通知。
 * 通常界面层或表示层实现该接口，底层处理函数调用该接口报告处理进度。
 */
class LIB_SDK Progress
{
public:
	virtual ~Progress() { };

	/**
	 * @brief 获取当前默认的进度接口。
     * 
	 * @return 默认进度接口，可能为NULL
     */
	static Progress* GetDefault();

	/**
	 * @brief 设置当前默认的进度接口。
     * 
	 * @param prog 默认进度接口
     */
	static void SetDefault(Progress* prog);

	/**
	 * @brief 启动进度。
     * 
	 * @param id 进度编号。由接口实现者分配，用于区分多个进度报告
	 * @param caption 进度标题
	 * @param cancellable 是否允许过程中中断
     */
	virtual void CreateProgress(int* id, const std::string& caption, bool cancellable) = 0;

	/**
	 * @brief 设定进度的总步数。
     * 
	 * @param id 分配的进度编号
	 * @param steps 进度总步数
     */
	virtual void SetTotalSteps(int id, long long steps) = 0;

	/**
	 * @brief 设定进度的总阶段数。
     * 
	 * @param id 分配的进度编号
	 * @param loops 进度阶段数
     */
	virtual void SetTotalLoops(int id, unsigned int loops) = 0;
	/**
	 * @brief 设定进度为步数未知类型。
     * 
	 * @param id 分配的进度编号
     */
	virtual void SetUnknownSteps(int id) = 0;

	/**
	 * @brief 设定进度的提示文本。
     * 
	 * @param id 分配的进度编号
	 * @param tip 提示文本内容
     */
	virtual void SetTipText(int id, const std::string& tip) = 0;

	/**
	 * @brief 进度前进一个单位。
     * 
	 * @param id 分配的进度编号
	 * @return 是否应该停止。由接口实现部分根据情况返回
     */
	virtual bool StepIt(int id) = 0;

	/**
	 * @brief 进度结束。
     * 
	 * @param id 分配的进度编号
	 * @param completed 进度是否正常结束
     */
	virtual void StopProgress(int id, bool completed) = 0;

	/**
	 * @brief 通用进度包装。
	 *
	 * Wrapper是一个辅助类，包装了通用进度接口，免除了处理代码每次调用进度
	 * 接口汇报状态前必须判断进度指针是否为空的工作。
	 */
	class Wrapper
	{
	public:
		Wrapper(Progress* prog) : id(0), progress(prog)
		{
			if (progress == NULL)
				progress = Progress::GetDefault();
		};

		inline void Create(const std::string& caption, unsigned int steps = 0, bool cancellable = false)
		{
			if( progress ) {
				progress->CreateProgress(&id, caption, cancellable);
				if (steps != 0)
					progress->SetTotalSteps(id, steps);
			}
		}

		inline void SetTotalSteps(long long steps)
		{
			if( progress )
				progress->SetTotalSteps(id, steps);
		}

		inline void SetTotalLoops(unsigned int loops)
		{
			if( progress )
				progress->SetTotalLoops(id, loops);
		}

		inline void SetUnknownSteps()
		{
			if( progress )
				progress->SetUnknownSteps(id);
		}

		inline void SetTipText(const std::string& tip)
		{
			if( progress )
				progress->SetTipText(id, tip);
		}

		inline bool Step()
		{
			if( progress )
				return progress->StepIt(id);
			return true;
		}

		inline void Stop(bool completed)
		{
			if( progress )
				return progress->StopProgress(id, completed);
		}

	private:
		int id;
		Progress* progress;
	};
};

}

#endif

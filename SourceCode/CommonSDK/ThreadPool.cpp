#include <boost/thread.hpp>
#include "ThreadPool.h"
#include "Logging.h"

namespace fm
{

class WorkThread;
typedef boost::shared_ptr<boost::thread> ThreadPtr;
typedef std::list<ThreadTaskPtr>         TaskList;
typedef boost::shared_ptr<WorkThread>    WorkThreadPtr;
typedef std::vector<WorkThreadPtr>       WorkThreadVec;

class WorkThread : public boost::enable_shared_from_this<WorkThread>
{
public:
	WorkThread()
	{

	}

	~WorkThread()
	{

	}

	void Initialize()
	{
		m_thread = ThreadPtr(new boost::thread(boost::bind(WorkThread::TaskThread, shared_from_this())));
	}

	void Join()
	{
		if (m_thread)
		{
			m_thread->join();
		}	
	}

	void SetThreadPool(ThreadPoolPtr threadpool)
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_threadpool = threadpool;
	}

	static void TaskThread(WorkThreadPtr workThread)
	{
		LOG_INFO("thread id:"<<workThread->m_thread->get_id()<<" is start");
		while(true)
		{
			ThreadTaskPtr task = workThread->m_threadpool->PopTask();

			if (!task)
			{
				break;
			}
			task->Execute();
		}
		LOG_INFO("thread id:"<<workThread->m_thread->get_id()<<" is finished" );
	}
private:
	ThreadPtr m_thread;

	ThreadPoolPtr m_threadpool;

	boost::mutex m_mutex;
}; 


class ThreadPoolImpl : public boost::enable_shared_from_this<ThreadPoolImpl> , public ThreadPool
{
public:
	ThreadPoolImpl(int threadNum):
	  m_threadNum(threadNum)
	{
	
	}

	~ThreadPoolImpl()
	{

	}

	void Initialize()
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		for (int i = 0; i < m_threadNum; i++)
		{
			WorkThreadPtr workThread = WorkThreadPtr(new WorkThread());
			workThread->Initialize();
			workThread->SetThreadPool(shared_from_this());

			m_theadStack.push_back(workThread);
		}
	}

	void Terminate()
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_bTerminate = true;
		for (int i = 0; i < m_threadNum; i++)
		{
			PushTask(ThreadTaskPtr());
		}
	}

	void Join()
	{
		WorkThreadVec::iterator it = m_theadStack.begin();
		while(it != m_theadStack.end())
		{
			(*it)->Join();
			{
				boost::unique_lock<boost::mutex> lock(m_mutex);
				m_theadStack.erase(it);
				it = m_theadStack.begin();
			}
		}
	}

	void AdjustThreadNum(int threadNum)
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);

		if (m_bTerminate || threadNum < 0 || m_threadNum == threadNum)
		{
			return;
		}
		else if (m_threadNum < threadNum)
		{
			for (int i = 0; i < threadNum - m_threadNum; i++)
			{
				WorkThreadPtr workThread = WorkThreadPtr(new WorkThread());
				workThread->Initialize();
				workThread->SetThreadPool(shared_from_this());

				m_theadStack.push_back(workThread);
			}
		}
		else 
		{
			for (int i = 0; i < m_threadNum - threadNum; i++)
			{
				boost::unique_lock<boost::mutex> lock(m_task_mutex);
				m_taskStack.push_front(ThreadTaskPtr());
				m_condition.notify_one();
			}
		}
		m_threadNum = threadNum;
	}

	void PushTask(ThreadTaskPtr task)
	{
		boost::unique_lock<boost::mutex> lock(m_task_mutex);
		m_taskStack.push_back(task);

		m_condition.notify_one();
	}

	virtual ThreadTaskPtr PopTask()
	{
		boost::unique_lock<boost::mutex> lock(m_task_mutex);
		if (m_taskStack.size() == 0)
		{
			m_condition.wait(lock);
			std::cout<<"condition is notify"<<std::endl;
		}

		ThreadTaskPtr task = m_taskStack.front();
		m_taskStack.pop_front();
		return task;
	}
	
private:
	int m_threadNum;

	TaskList m_taskStack;

	WorkThreadVec m_theadStack;

	boost::mutex m_task_mutex;

	boost::mutex m_mutex;

	boost::condition_variable m_condition;

	bool m_bTerminate;
};


LIB_SDK ThreadPoolPtr CreateThreadPool(int threadNum)
{
	boost::shared_ptr<ThreadPoolImpl> threadPoolimpl(new ThreadPoolImpl(threadNum));
	threadPoolimpl->Initialize();
	return threadPoolimpl;
}

}
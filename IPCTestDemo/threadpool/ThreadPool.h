#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../Singleton.h"
#include <string>
#include <vector>
#include <deque>
#include <mutex>
#include <memory>
#include <thread>
#include <condition_variable>

class ThreadPool
{
public:
	typedef std::function<void(int id)> Callback;
	typedef std::mutex MutexLock;
	typedef std::unique_lock<MutexLock> MutexLockGuard;
	typedef std::condition_variable Condition;

	class WorkItem
	{
	public:
		WorkItem(Callback cb, int id)
		{
			_cb = cb;
			_id = id;
		}

		void Invoke()
		{
			_cb(_id);
		}
	private:
		Callback _cb;
		int      _id;
	};
	typedef std::shared_ptr<WorkItem> TPtrWorkItem;

public:
	explicit ThreadPool(const std::string& name = std::string(), int iMaxThreads = -1);
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void AddWorkItem(Callback cb, int id = -1);
	void CreateThread(int numThreads);
	void DeleteThread();

private:
	void RunInThread();

	TPtrWorkItem GetWorkItem();

private:
	MutexLock mutex_;
	Condition cond_;
	std::string name_;
	std::vector<std::shared_ptr<std::thread>> threads_;
	std::deque<TPtrWorkItem> queue_;
	bool running_;
};

#define CpcThreadPool CSingleton<ThreadPool>::GetInstance()
#endif
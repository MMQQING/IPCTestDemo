#include "ThreadPool.h"
using namespace std;
ThreadPool::ThreadPool(const string& name,int iMaxThreads)
	: name_(name)
	, running_(false)
{
	CreateThread(iMaxThreads);
}

ThreadPool::~ThreadPool()
{
	if (running_)
	{
		DeleteThread();
	}
}

void ThreadPool::AddWorkItem(Callback cb, int id/* = -1*/)
{
	if (!running_) return;
	if (threads_.empty())
	{
		cb(id);
	}
	else
	{
		TPtrWorkItem workItem = std::make_shared<WorkItem>(cb, id);
		MutexLockGuard lock(mutex_);
		queue_.push_back(workItem);
		cond_.notify_one();
	}
}

void ThreadPool::CreateThread(int numThreads)
{
	if (numThreads < 0)
	{
		numThreads = std::thread::hardware_concurrency() + 2;
	}

	running_ = true;
	threads_.reserve(numThreads);
	for (int i = 0; i < numThreads; ++i)
	{
		char id[32];
		sprintf_s(id, sizeof id, "%d", i);
		threads_.push_back(std::make_shared<std::thread>(std::bind(&ThreadPool::RunInThread, this), name_+id));
	}
}

void ThreadPool::DeleteThread()
{
	{
		MutexLockGuard lock(mutex_);
		running_ = false;
		cond_.notify_all();
	}
	for (auto& thr : threads_)
	{
		thr->join();
	}
}

void ThreadPool::RunInThread()
{
	try
	{
		while (running_)
		{
			TPtrWorkItem work = GetWorkItem();
			if (work)
			{
				work->Invoke();
			}
		}
	}
	catch (const std::exception& ex)
	{
		fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
		fprintf(stderr, "reason: %s\n", ex.what());
		abort();
	}
	catch (...)
	{
		fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
		throw; // rethrow
	}
}

ThreadPool::TPtrWorkItem ThreadPool::GetWorkItem()
{
	MutexLockGuard lock(mutex_);
	while (queue_.empty() && running_)
	{
		cond_.wait(lock);
	}
	TPtrWorkItem workItem;
	if (!queue_.empty())
	{
		workItem = queue_.front();
		queue_.pop_front();
	}
	return workItem;
}


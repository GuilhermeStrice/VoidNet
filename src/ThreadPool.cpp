#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int threads) :
	terminate(false),
	stopped(false)
{
	for (int i = 0; i < threads; i++)
		threadPool.emplace_back(std::thread(&ThreadPool::Invoke, this));
}

void ThreadPool::Enqueue(std::function<void()> f)
{
	std::unique_lock<std::mutex> lock(tasksMutex);
	tasks.push(f);
	condition.notify_one();
}

void ThreadPool::Invoke() {

	std::function<void()> task;
	while (true)
	{
		std::unique_lock<std::mutex> lock(tasksMutex);
		condition.wait(lock, [this] { return !tasks.empty() || terminate; });
		if (terminate && tasks.empty())
		{
			return;
		}
		task = tasks.front();
		tasks.pop();
		task();
	}
}

void ThreadPool::Shutdown()
{
	std::unique_lock<std::mutex> lock(tasksMutex);
	terminate = true;
	condition.notify_all();

	for (std::thread &thread : threadPool)
		thread.join();

	threadPool.empty();
	stopped = true;
}

const ThreadPool &ThreadPool::operator=(ThreadPool &pool)
{
	return pool;
}

ThreadPool::~ThreadPool()
{
	if (!stopped)
		Shutdown();
}
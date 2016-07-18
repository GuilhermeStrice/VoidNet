#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool(int threads);
	~ThreadPool();
	void Enqueue(std::function<void()> f);
	void Shutdown();
	const ThreadPool &operator=(ThreadPool &pool);

private:
	std::vector<std::thread> threadPool;
	std::queue<std::function<void()>> tasks;
	std::mutex tasksMutex;
	std::condition_variable condition;
	bool terminate;
	bool stopped;
	void Invoke();
};

#endif
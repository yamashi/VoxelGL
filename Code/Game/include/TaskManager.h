#pragma once

#include "Task.h"
#include "Log.h"

#include <queue>
#include <mutex>
#include <memory>
#include <thread>

#include <condition_variable>

class TaskManager
{
public:

	TaskManager();
	~TaskManager();

	void AddBackground(Task* apTask);
	void AddMainthread(Task* apTask);

	void Run(uint32_t aMillisecondsAllocated);

	static void Initialize()
	{
		if (s_instance != nullptr)
		{
			Crash("TaskManager already exists.");
		}
		s_instance = new TaskManager();
	}

	static void Shutdown()
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
		}
	}

	static TaskManager& GetInstance()
	{
		return *s_instance;
	}

private:

	void RunBackground();

	using TaskQueue = std::queue < std::unique_ptr<Task> > ;
	using ThreadQueue = std::queue < std::thread > ;

	std::mutex m_backgroundLock;
	std::mutex m_mainthreadLock;

	std::mutex m_conditionLock;
	std::condition_variable m_condition;

	TaskQueue m_backgroundTasks;
	TaskQueue m_mainthreadTasks;

	ThreadQueue m_threads;

	volatile bool m_running{ true };

	static TaskManager* s_instance;
};
#include "TaskManager.h"

#include <iostream>

TaskManager* TaskManager::s_instance = nullptr;

TaskManager::TaskManager()
{
	auto threadCount = std::thread::hardware_concurrency();

	for (unsigned int i = 0; i < threadCount; ++i)
	{
		m_threads.push(std::thread([this](){ RunBackground(); return; }));
	}
}

TaskManager::~TaskManager()
{
	m_running = false;

	m_condition.notify_all();

	while (m_threads.empty() == false)
	{
		std::thread t = std::move(m_threads.front());

		if (t.joinable())
		{
			t.join();
		}
		m_threads.pop();
	}
}

void TaskManager::AddBackground(Task* apTask)
{
	{
		std::lock_guard<std::mutex> _(m_backgroundLock);

		m_backgroundTasks.push(std::unique_ptr<Task>(apTask));
	}
	m_condition.notify_one();
}

void TaskManager::AddMainthread(Task* apTask)
{
	std::lock_guard<std::mutex> _(m_backgroundLock);

	m_mainthreadTasks.push(std::unique_ptr<Task>(apTask));
}

void TaskManager::Run(uint32_t aMillisecondsAllocated)
{
	const std::chrono::duration<uint32_t, std::milli> cTimeSlice{ aMillisecondsAllocated };

	auto start = std::chrono::high_resolution_clock::now();
	auto end = start;
	while (end - start < cTimeSlice)
	{
		std::unique_ptr<Task> pTask;
		{
			std::lock_guard<std::mutex> _(m_mainthreadLock);
			if (!m_mainthreadTasks.empty())
			{
				pTask = std::move(m_mainthreadTasks.front());
				m_mainthreadTasks.pop();
			}
			else
			{
				return;
			}
		}

		if (pTask)
		{
			auto state = pTask->DoMainthreadWork();
			if (state == Task::KNeedBackgroundTime)
			{
				std::lock_guard<std::mutex> _(m_backgroundLock);
				m_backgroundTasks.push(std::move(pTask));
			}
			else if (state == Task::KNeedMainthreadTime)
			{
				std::lock_guard<std::mutex> _(m_mainthreadLock);
				m_mainthreadTasks.push(std::move(pTask));
			}
		}

		end = std::chrono::high_resolution_clock::now();
	}
}

void TaskManager::RunBackground()
{
	std::unique_lock<std::mutex> loopLock(m_conditionLock);

	while (m_running)
	{
		std::unique_ptr<Task> pTask;
		{
			std::lock_guard<std::mutex> _(m_backgroundLock);
			if (!m_backgroundTasks.empty())
			{
				pTask = std::move(m_backgroundTasks.front());
				m_backgroundTasks.pop();
			}
		}

		if (pTask)
		{
			auto state = pTask->DoBackgroundWork();
			if (state == Task::KNeedBackgroundTime)
			{
				std::lock_guard<std::mutex> _(m_backgroundLock);
				m_backgroundTasks.push(std::move(pTask));
			}
			else if (state == Task::KNeedMainthreadTime)
			{
				std::lock_guard<std::mutex> _(m_mainthreadLock);
				m_mainthreadTasks.push(std::move(pTask));
			}
		}
		else
		{
			m_condition.wait(loopLock);
		}
	}
}
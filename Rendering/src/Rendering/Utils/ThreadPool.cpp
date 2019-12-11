#include <stdafx.h>
#include <Rendering/Utils/ThreadPool.h>

Rendering::Utils::ThreadPool::~ThreadPool()
{
	for (auto& worker : m_workers)
	{
		worker.join();
	}
}

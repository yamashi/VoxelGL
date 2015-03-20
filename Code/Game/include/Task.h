#pragma once

class Task
{
public:

	enum State
	{
		KNeedBackgroundTime,
		KNeedMainthreadTime,
		KFinished
	};

	virtual ~Task(){};

	// Returning true means there is more background work to do
	virtual State DoBackgroundWork() = 0;
	// Return true means there is more mainthread work to do
	virtual State DoMainthreadWork() = 0;
};
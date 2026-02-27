#include "DataManager.h"

static DataManager* instance_ = nullptr;

void DataManager::CreateInstance(void)
{
	if (!instance_)
	{
		instance_ = new DataManager();

		instance_->Reset();
	}
}

DataManager& DataManager::GetInstance(void)
{
	if (!instance_)
	{
		CreateInstance();
	}
	return *instance_;
}

void DataManager::Destroy(void)
{
	if (instance_)
	{
		delete instance_;

		instance_ = nullptr;
	}
}

void DataManager::Reset(void)
{
	currentYear_ = START_YEAR;

	sankinCount_ = 0;

	finalYearReached_ = START_YEAR;
}

void DataManager::AddCount(void)
{
	sankinCount_++;

	if (sankinCount_ >= 2)
	{
		if (currentYear_ < MAX_YEAR)
		{
			currentYear_++;
		}

		sankinCount_ = 0;
	}
}

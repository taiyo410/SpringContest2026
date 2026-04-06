#include "InputManager.h"
#include "DataBank.h"


void DataBank::Init(void)
{
	playerNum_ = 0;
	maxPlayerNum_ = GetJoypadNum();
	isFullScreen_ = false;
	bgmVol_ = 100;
	seVol_ = 100;
}

bool DataBank::SetPlayerNum(int _num)
{
	//maxPlayerNum_ = GetJoypadNum();
	//if (num <= maxPlayerNum_)
	//{
		playerNum_ = _num;
		return true;
	//}
	return false;
}

int DataBank::GetPlayerNum(void)
{
	return playerNum_;
}

void DataBank::SetIsFullScreen(const bool _isFullScreen)
{
	isFullScreen_ = _isFullScreen;
    ChangeWindowMode(!isFullScreen_);
}


DataBank::DataBank(void)
{
	maxPlayerNum_ = 0;
	playerNum_ = 0;
}

DataBank::~DataBank(void)
{
}

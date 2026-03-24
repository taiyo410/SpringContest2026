#pragma once
#include"../Template/Singleton.h"

class DataRegistry : public Singleton<DataRegistry>
{
public:

	//全外部ファイルを生成
	void CreateAll(void);
};
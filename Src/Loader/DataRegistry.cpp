#include"../pch.h"
#include"LoaderManager.h"
#include"../Object/Charactor/Daimyo/DaimyoImport.h"
#include"DataRegistry.h"

// シングルトンインスタンスの初期化
DataRegistry* DataRegistry::instance_ = nullptr;

void DataRegistry::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DataRegistry();
	}
}

DataRegistry& DataRegistry::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}

	return *instance_;
}

void DataRegistry::CreateAll(void)
{	
	//大名の情報
	LoaderManager<DaimyoImport>::CreateInstance();
}

void DataRegistry::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

DataRegistry::DataRegistry(void)
{
}

DataRegistry::~DataRegistry(void)
{
}

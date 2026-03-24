#include"../pch.h"
#include"LoaderManager.h"
#include"../Object/Character/Daimyo/DaimyoImport.h"
#include"DataRegistry.h"

void DataRegistry::CreateAll(void)
{	
	//ステージの情報
	LoaderManager<DaimyoImport>::CreateInstance();
}

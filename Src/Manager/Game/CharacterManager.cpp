#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Loader/LoaderManager.h"
#include "../Object/Character/Base/CharacterBase2D.h"
#include "../Object/Character/Daimyo/Daimyo.h"
#include "CharacterManager.h"

void CharacterManager::Load(void)
{
	daimyoImportData_ = LoaderManager<DaimyoImport>::GetInstance().GetfileData(UtilityCommon::GetStringFromWString(Application::PATH_JSON + L"Daimyo.json"));
}

void CharacterManager::Init(void)
{
	//各オブジェクトの生成
	CreateEdo();
	CreateDaimyo();
}

void CharacterManager::Update(void)
{
	//キャラクターの更新
	for (auto& chara : characters_)
	{
		chara->Update();
	}
}

void CharacterManager::Draw(void)
{
	//キャラクターの描画
	for (auto& chara : characters_)
	{
		chara->Draw();
	}
}

void CharacterManager::Release(void)
{
	//キャラクターの描画
	for (auto& chara : characters_)
	{
		chara->Release();
	}
}

CharacterManager::CharacterManager(void)
{

}
CharacterManager::~CharacterManager(void)
{
}

void CharacterManager::CreateEdo(void)
{
	
}

void CharacterManager::CreateDaimyo(void)
{
	//人数分
	for (auto& daimyoImport : daimyoImportData_)
	{
		//生成
		std::unique_ptr<Daimyo> daimyo = std::make_unique<Daimyo>(daimyoImport);
		
		//読み込みと初期化
		daimyo->Load();
		daimyo->Init();

		//格納
		characters_.push_back(std::move(daimyo));
	}
}

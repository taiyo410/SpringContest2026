#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Loader/LoaderManager.h"
#include "../Object/Character/Base/CharacterBase2D.h"
#include "../Object/Character/Cursor/Cursor.h"
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
	CreateCursor();
}

void CharacterManager::Update(void)
{
	//キャラクターの更新
	cursor_->Update();
	cursor_->Sweep();

	for (auto& daimyo : daimyo_)
	{
		daimyo->Update();
		daimyo->Sweep();
	}
}

void CharacterManager::Draw(void)
{
	//キャラクターの描画
	cursor_->Draw();
	
	for (auto& daimyo : daimyo_)
	{
		daimyo->Draw();
	}
}

void CharacterManager::Release(void)
{
	//キャラクターの更新
	cursor_->Release();

	for (auto& daimyo : daimyo_)
	{
		daimyo->Release();
	}
}

const bool CharacterManager::IsMoneyMax(void)
{
	for (auto& daimyo : daimyo_)
	{
		if (daimyo->IsMoneyMax())
		{
			return true;
		}
	}

	return false;
}

CharacterManager::CharacterManager(void)
{

}
CharacterManager::~CharacterManager(void)
{
}

void CharacterManager::CreateCursor(void)
{
	//生成
	cursor_ = std::make_unique<Cursor>();

	//読み込みと初期化
	cursor_->Load();
	cursor_->Init();
}

void CharacterManager::CreateEdo(void)
{
	
}

void CharacterManager::CreateDaimyo(void)
{
	int a = 0;

	//人数分
	for (auto& daimyoImport : daimyoImportData_)
	{
		if (a >= 3)break;

		//生成
		std::unique_ptr<Daimyo> daimyo = std::make_unique<Daimyo>(daimyoImport);
		
		//読み込みと初期化
		daimyo->Load();
		daimyo->Init();

		//格納
		daimyo_.push_back(std::move(daimyo));

		a++;
	}
}

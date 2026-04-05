#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Manager/Resource/ResourceManager.h"
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

	//arrow_->Init();
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
	for (auto& daimyo : daimyo_)
	{
		daimyo->Draw();
	}

	cursor_->Draw();

	DrawCircle(EDO_POS.x, EDO_POS.y, 50.0f, 0xFF0000, true);
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
	arrowStartPos_ = {};
	arrowEndPos_ = { 100.0f,100.0f };
	arrowPer_ = 0.5f;
	arrow_=std::make_unique<ArrowController>(
		ResourceManager::SRC::ARROW_GAUGE, arrowStartPos_, arrowEndPos_, 100.0f, arrowPer_, arrowCol_);
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

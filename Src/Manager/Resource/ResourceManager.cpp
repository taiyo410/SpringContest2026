#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	static std::wstring PATH_IMG = Application::PATH_IMAGE;
	static std::wstring PATH_MDL = Application::PATH_MODEL;
	static std::wstring PATH_ANIM_PLAYER = Application::PATH_ANIM_PLAYER;
	static std::wstring PATH_ANIM_ENEMY = Application::PATH_ANIM_ENEMY;
	static std::wstring PATH_EFF = Application::PATH_EFFECT;
	static std::wstring PATH_SHADER = Application::PATH_SHADER;

	std::unique_ptr<ResourceData> res;

	//モデル登録


	//アニメーション登録

	//ピクセルシェーダ登録
	

	//画像登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"Map.png");
	resourcesMap_.emplace(SRC::MAP, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"Castle.png");
	resourcesMap_.emplace(SRC::CASTLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"Gauge.png");
	resourcesMap_.emplace(SRC::GAUGE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ArrowGauge.png");
	resourcesMap_.emplace(SRC::ARROW_GAUGE, std::move(res));

	//複数画像
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMGS, PATH_IMG + L"XboxControllerBotton128.png",
		CONTROLLER_UI_NO_X, CONTROLLER_UI_NO_Y, CONTROLLER_UI_SIZE_X, CONTROLLER_UI_SIZE_Y);
	resourcesMap_.emplace(SRC::CONTROLLER_UI_IMGS, std::move(res));


	//エフェクト登録
}

void ResourceManager::SceneChangeRelease(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Release(void)
{
	SceneChangeRelease();
	resourcesMap_.clear();
}

const ResourceData& ResourceManager::Load(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	loadedMap_.clear();
	resourcesMap_.clear();
}

ResourceData& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}

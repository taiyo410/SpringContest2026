#include "../../Pch.h"
#include"ResourceManager.h"
#include"../../Application.h"
#include"Resource.h"

// シングルトンのインスタンス初期化
ResourceManager* ResourceManager::instance_ = nullptr;

// インスタンス生成（初回のみ）＋初期化呼び出し
void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

// インスタンス参照を返す
ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

// 共通初期化処理（今は空）
void ResourceManager::Init(void)
{
	Resource res;
}
// タイトルシーン用リソースの初期化
void ResourceManager::InitTitle(void)
{
	Resource res;

	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "AS_1528820.mp3");
	resourcesMap_.emplace(SRC::BGM_TITLE, res);
}

// ゲームシーン用リソースの初期化
void ResourceManager::InitGame(void)
{
	Resource res;

	// プレイヤー関連リソースの初期化
	ResourcePlayer();

	// 敵関連リソースの初期化
	ResourceEnemy();
}

// ゲームオーバーシーン用リソースの初期化
void ResourceManager::InitGameOver(void)
{
	Resource res;

	// ゲームオーバーロゴ画像を登録
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "");
	resourcesMap_.emplace(SRC::GAMEOVER_LOGO, res);
}

// ゲームクリアシーン用リソースの初期化
void ResourceManager::InitGameClear(void)
{
	Resource res;

	// ゲームクリアロゴ画像を登録
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "");
	resourcesMap_.emplace(SRC::GAMECLERA_LOGO, res);
}


// プレイヤー用リソース初期化
void ResourceManager::ResourcePlayer(void)
{
	Resource res;

}

// 敵用リソース初期化
void ResourceManager::ResourceEnemy(void)
{
	Resource res;
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Castle.png");
	resourcesMap_.emplace(SRC::IMG_DAIMYO, res);
}

// 全リソースの解放処理
void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release(); // リソース解放
		delete p.second;     // メモリ解放
	}

	loadedMap_.clear();     // ロード済みリソースマップをクリア
	resourcesMap_.clear();  // 登録済みリソースマップをクリア
}

// インスタンス破棄処理
void ResourceManager::Destroy(void)
{
	Release();        // リソース解放
	delete instance_; // インスタンス削除
}

// リソースの読み込み（読み込み済みなら再利用）
Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource(); // 空のリソースを返す
	}
	return *res; // コピーして返す
}

// モデルの複製を行い、複製IDを返す
int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (!res || res->handleId_ == -1)
		return -1;

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

int ResourceManager::GetHandle(SRC src)
{
	auto it = resourcesMap_.find(src);

	if (it == resourcesMap_.end()) { return -1; }

	// モデルならロードしてなければロード
	return it->second.GetHandle();
}

// コンストラクタ
ResourceManager::ResourceManager(void)
{
}

// 内部リソース読み込み処理
Resource* ResourceManager::_Load(SRC src)
{
	// すでに読み込み済みか確認
	auto itLoaded = loadedMap_.find(src);
	if (itLoaded != loadedMap_.end())
		return itLoaded->second;

	// 登録済みリソースか確認
	auto itRes = resourcesMap_.find(src);
	if (itRes == resourcesMap_.end())
		return nullptr;

	// リソース読み込み（handleId_ がセットされる）
	itRes->second.Load();

	// loadedMap_ にコピーして保持
	Resource* newRes = new Resource(itRes->second);

	// ここで loadedMap に追加
	loadedMap_.emplace(src, newRes);

	return newRes;
}

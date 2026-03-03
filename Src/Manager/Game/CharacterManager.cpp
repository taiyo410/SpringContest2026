#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/UIManager.h"
#include"../Manager/Generic/Camera.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/Enemy.h"

#include "CharacterManager.h"

void CharacterManager::Load(void)
{

	player_ = std::make_shared<Player>();
	player_->Load();

	enemy_ = std::make_shared<Enemy>();
	enemy_->Load();

	SceneManager::GetInstance().GetCamera().lock()->SetTarget(&enemy_->GetTransform());
}

void CharacterManager::Init(void)
{

}

void CharacterManager::Update(void)
{
	//プレイヤーの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
}

void CharacterManager::DirectionUpdate(void)
{
	//if()

	//enemy_->UpdateDirection();
	//player_->UpdateDirection();
}

void CharacterManager::Draw(void)
{
	enemy_->Draw();
	player_->Draw();
}

void CharacterManager::Draw2D(void)
{
	enemy_->Draw2D();
	player_->Draw2D();
}

void CharacterManager::Release(void)
{
}

void CharacterManager::ChangeCharacterNormalUpdate(void)
{
	player_->ChangeDirectToNormal();
	enemy_->ChangeDirectToNormal();
}

CharacterManager::CharacterManager(void)
{

}
CharacterManager::~CharacterManager(void)
{
}

#include<algorithm>
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility2D.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Object/Common/AnimationController.h"
#include"../Base/CharacterOnHitBase.h"
#include "Player.h"

Player::Player(void)
{
	characterType_ = CHARACTER_TYPE::PLAYER;
}

Player::~Player(void)
{
	collider_.clear();
}

void Player::Load(void)
{
}

void Player::Init(void)
{
}

void Player::MakeColliderGeometry(void)
{
}


void Player::UpdateNormal(void)
{
}

void Player::Draw(void)
{

}
void Player::Draw2D(void)
{

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}
void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}

#ifdef _DEBUG
void Player::DrawDebug(void)
{
}

#endif // _DEBUG


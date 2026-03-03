#include"../Utility/Utility2D.h"
#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include "../../../Application.h"
#include "../Player/Player.h"
#include "../Base/CharacterOnHitBase.h"
#include "../Object/Common/AnimationController.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/DataBank.h" 
#include "../Object/Common/EffectController.h"

#include "Enemy.h"

Enemy::Enemy(void)
{


}

Enemy::~Enemy(void)
{
	collider_.clear();
}
void Enemy::Load(void)
{

}

void Enemy::Init(void)
{

}

void Enemy::Draw(void)
{
	//í èÌï`âÊ
	MV1DrawModel(trans_.modelId);
}
void Enemy::Draw2D(void)
{

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}
void Enemy::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}

void Enemy::MakeColliderGeometry(void)
{

}
void Enemy::UpdateNormal(void)
{

}

#ifdef _DEBUG
void Enemy::DrawDebug(void)
{

}
#endif // _DEBUG

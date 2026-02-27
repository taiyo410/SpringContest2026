#include "../../Pch.h"
#include "CollisionController.h"
#include "../../Object/ObjectBase.h"
#include "../../Utility/Utility.h"
#include "../Generic/SceneManager.h"

CollisionController* CollisionController::instance_ = nullptr;

void CollisionController::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new CollisionController();
        instance_->Init();
    }
}

CollisionController& CollisionController::GetInstance(void)
{
    return *instance_;
}

void CollisionController::Destroy(void)
{
    delete instance_;
    instance_ = nullptr;
}

CollisionController::CollisionController(void)
    : updateTimer_(0.0f)
{
}

CollisionController::~CollisionController(void)
{
}

void CollisionController::Init(void)
{
    objects_.clear();
}

void CollisionController::RegisterObject(ObjectBase* obj) 
{
    if (!obj) return;
    objects_.push_back(obj);
}

void CollisionController::UnregisterObject(ObjectBase* obj)
{
    objects_.erase(std::remove(objects_.begin(), objects_.end(), obj), objects_.end());
}

void CollisionController::Update(void)
{
    updateTimer_ += SceneManager::GetInstance().GetDeltaTime();

    if (updateTimer_ >= UPDATE_INTERVAL)
    {
        updateTimer_ = 0.0f;
        UpdateCollisionPairs();
    }
}

// コライダペアの更新処理を分離
void CollisionController::UpdateCollisionPairs(void)
{
    if (objects_.size() < 2) { return; }

    for (size_t i = 0; i < objects_.size(); ++i)
    {
        auto* objA = objects_[i];

        objA->ClearHitCollider();

        for (size_t j = i + 1; j < objects_.size(); ++j)
        {
            auto* objB = objects_[j];

            // 両方のオブジェクトが持つコライダ同士を総当たり
            for (auto const& [keyA, colA] : objA->GetOwnColliders())
            {
                for (auto const& [keyB, colB] : objB->GetOwnColliders())
                {
                    if (!IsCollisionEnabled(colA->GetTag(), colB->GetTag())) continue;

                    CollisionInfo info;

                    if (CheckCircleVsCircle(colA, colB, info))
                    {
                        objA->AddHitCollider(colB);
                        
                        objB->AddHitCollider(colA);
                    }
                }
            }
        }
    }
}

bool CollisionController::CheckCircleVsCircle(const ColliderBase* colA, const ColliderBase* colB, CollisionInfo& outInfo)
{
    // 2D座標(X, Y)のみで距離計算
    VECTOR posA = colA->GetWorldPos();
    VECTOR posB = colB->GetWorldPos();

    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;
    float distSq = dx * dx + dy * dy;

    float radiusSum = colA->GetRadius() + colB->GetRadius();

    if (distSq < radiusSum * radiusSum) {
        float dist = sqrtf(distSq);
        outInfo.hitNormal = (dist > 0) ? VGet(dx / dist, dy / dist, 0) : VGet(1, 0, 0);
        outInfo.penetration = radiusSum - dist;
        return true;
    }
    return false;
}

bool CollisionController::IsCollisionEnabled(ColliderBase::TAG tagA, ColliderBase::TAG tagB)
{
    // 2Dゲームで必要な組み合わせのみ true を返す
    if ((tagA == ColliderBase::TAG::PLAYER && tagB == ColliderBase::TAG::ENEMY) ||
        (tagA == ColliderBase::TAG::ENEMY && tagB == ColliderBase::TAG::PLAYER))
    {
        return true;
    }

    return false;
}

void CollisionController::Clear(void)
{
    objects_.clear();
}
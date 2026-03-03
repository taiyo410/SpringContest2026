#include "../Pch.h"
#include "Circle.h"
#include "BoxGeo.h"

BoxGeo::BoxGeo(const Vector2F _pos, const Vector2F _movedPos, const Vector2F _min, Vector2F _max)
    : Geometry2D(_pos,_movedPos)
{
    obb_.vMin = _min;
    obb_.vMax = _max;
}

BoxGeo::BoxGeo(const Vector2F _pos, const Vector2F _movedPos, const Vector2F _halfSize)
    : Geometry2D(_pos, _movedPos)
{
    obb_.vMin = _halfSize * -1.0f;
    obb_.vMax = _halfSize;
}

BoxGeo::~BoxGeo(void)
{
}

void BoxGeo::Draw(unsigned const int _color)
{
    Vector2F minPos = pos_ + obb_.vMin;
    Vector2F maxPos = pos_ + obb_.vMax;

    DrawBox(minPos.x, minPos.y, maxPos.x, maxPos.y, _color, true);
}

const bool BoxGeo::IsHit(Geometry2D& _partnerGeo)
{
    return _partnerGeo.IsHit(*this);
}

const bool BoxGeo::IsHit(Circle& _partnerGeo)
{
    return _partnerGeo.IsHit(*this);
}

const bool BoxGeo::IsHit(BoxGeo& _partnerGeo)
{
    //自身の箱情報
    float myMinX = pos_.x + obb_.vMin.x;
    float myMinY = pos_.y + obb_.vMin.y;
    float myMaxX = pos_.x + obb_.vMax.x;
    float myMaxY = pos_.y + obb_.vMax.y;

    //相手の情報
    Vector2F pPos = _partnerGeo.GetPos();
    OBB_2D pObb = _partnerGeo.GetObb();
    float pMinX = pPos.x + pObb.vMin.x;
    float pMinY = pPos.y + pObb.vMin.y;
    float pMaxX = pPos.x + pObb.vMax.x;
    float pMaxY = pPos.y + pObb.vMax.y;

    //重なっている量
    float overlapX = std::min(myMaxX, pMaxX) - std::max(myMinX, pMinX);
    float overlapY = std::min(myMaxY, pMaxY) - std::max(myMinY, pMinY);

    //当たっているか
    if (overlapX <= 0.0f || overlapY <= 0.0f)return false;

    //中心値
    float myCenter_X = (myMinX + myMaxX) * 0.5f;
    float pCenter_X = (pMinX + pMaxX) * 0.5f;
    float myCenter_Y = (myMinY + myMaxY) * 0.5f;
    float pCenter_Y = (pMinY + pMaxY) * 0.5f;

    //ヒット情報
    float depth;
    Vector2F normal;
    Vector2F point;

    //相手側
    HitResult partnerResult;

    //どっちの方向にめり込んでいるのか
    if (overlapX < overlapY)
    {
        //X方向にめり込んでいる
        depth = overlapX;
        normal = myCenter_X < pCenter_X ? Vector2F{ -1.0f, 0.0f } : Vector2F{ 1.0f, 0.0f };
    }
    else
    {
        //Y方向にめり込んでいる
        depth = overlapY;
        normal = pos_.y < pPos.y ? Vector2F{ 0.0f, -1.0f } : Vector2F{ 0.0f, 1.0f };
    }

    //重なった中心点を衝突点とする
    point = {
        (std::max(myMinX,pMinX) + std::min(myMaxX,pMaxX)) * 0.5f,
        (std::max(myMinY,pMinY) + std::min(myMaxY,pMaxY)) * 0.5f
    };

    //自身のヒット情報
    hitResult_.depth = depth;
    hitResult_.normal = normal;
    hitResult_.point = point;

    //相手側
    partnerResult.depth = depth;
    partnerResult.normal = normal * -1.0f;
    partnerResult.point = point;

    return true;
}

void BoxGeo::HitAfter(void)
{
}

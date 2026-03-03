#pragma once
#include"../Object/ObjectBase.h"

class ActionController;
class CharacterBase;

class CharacterOnHitBase
{
public:
	struct HIT_POINT
	{
		bool isDown = false;
		bool isOverHead = false;
		bool isSide = false;
	};

	CharacterOnHitBase(CharacterBase& _chara, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam);
	virtual ~CharacterOnHitBase(void);

	/// @brief  初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 当たった時のSEなどの読み込み
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 当たった処理の更新
	/// @param _hitCol 
	void OnHitUpdate(const std::weak_ptr<Collider> _hitCol);


	/// @brief 当たり判定をする前の初期化
	/// @param  
	void InitHit(void);

	/// @brief ターゲットと当たったかどうかの取得
	/// @param  
	/// @return ターゲットと当たったかどうか
	const bool GetIsHitTarget(void)const { return isHitTarget_; }


#ifdef _DEBUG
	/// @brief デバッグ表示
/// @param  
	virtual void DrawDebug(void) = 0;
#endif // _DEBUG




protected:

	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;
	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 3.0f;
	//押し出す移動量
	static constexpr float POSITION_OFFSET = 0.1f;

	//相手の身体を当たったかどうか
	bool isHitTarget_;

	//当たり判定ごとの更新
	std::map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>>colUpdates_;
	//当たり判定関係
	std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& colParam_;
	Collider::TAG tag_;	//プレイヤーの当たり判定タグ
	//キャラクターの情報
	CharacterBase& charaObj_;


	//モデルの当たった時の共通処理
	void HitModelCommon(const std::weak_ptr<Collider> _hitCol);

	//オブジェクト毎の当たった時にされる処理
	void CollNone(void);												//当たっても何もしない(プレイヤー側で何も起きない)
	void CollStage(const std::weak_ptr<Collider> _hitCol);				//ステージ
	virtual void CollChara(const std::weak_ptr<Collider> _hitCol) = 0;				//キャラクター同士
private:
};


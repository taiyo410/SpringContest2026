#pragma once
#include<map>
#include "../Manager/Resource/SoundManager.h"
#include "../Base/CharacterOnHitBase.h"
#include "../UIData/CharacterUIData.h"
#include "../Object/ObjectBase.h"

class AnimationController;
class EffectController;
class UIManager;
class ActionController;
class InputBase;
class Capsule;
class HpUIBase;
class LogicBase;
class PlayerOnHit;
class EnemyOnHit;
class EnemyRock;



class CharacterBase :public ObjectBase
{
public:

	//ヒットストップ止めるフレーム数
	static constexpr int HIT_STOP_FRAME = 5;

	//アニメーション速度
	static constexpr float ANIM_SPEED = 40.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
	//最大ＨＰ
	static constexpr float HP_MAX = 200.0f;
	//転がるアニメーション速度
	static constexpr float ROLL_ANIM_SPEED = 20.0f;

	//当たり判定
	static constexpr int CUPSULE_COL_NO = 0;
	static constexpr int MOVE_LINE_COL_NO = 1;
	static constexpr int UP_DOWN_LINE = 2;
	static constexpr int ATK_COL_NO = 3;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE,				//何もしない
		IDLE,				//アイドル
		RUN,				//走る
		REACT,				//ダメージリアクション
		DEATH,				//死亡
		ATTACK_1_MIDDLE,	//中距離攻撃
		ATTACK_1_SHORT,		//近距離攻撃
		ATTACK_2,			//攻撃2段目
		ATTACK_3,			//攻撃3段目
		JUMP,				//ジャンプ
		DODGE,				//回避
		CARD_RELOAD,		//カードリロード
		//敵
		SWIP_ATK,			//ひっかき
		ROAR_ATK,			//咆哮
		JUMP_ATK,			//ジャンプ攻撃
		RUSH_ATK			//突進

	};


	enum class ACTION_TYPE
	{
		IDLE,		//何もしてない
		MOVE,		//移動
		DASHMOVE,	//ダッシュ
		REACT,	//パンチされた状態
		JUMP,		//ジャンプ
		CARD_ACTION	//カードアクション
	};

	enum class UPDATE_PHASE
	{
		NONE,
		NORMAL,
		HIT_STOP
	};
	/// @brief コンストラクタ
	/// @param  
	CharacterBase(void);

	/// @brief デストラクタ
	/// @param  
	virtual ~CharacterBase(void)override = 0;

	/// @brief 読み込み
	/// @param  
	virtual void Load(void)override = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void)override = 0;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	virtual void Draw(void)override = 0;

	//2D関連の描画
	virtual void Draw2D(void) = 0;

	/// @brief 当たった時の処理
	/// @param _hitCol 
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol)override = 0;
	
	/// @brief 攻撃の当たり判定生成
	/// @param _charaTag 自身のタグ
	/// @param _atkPos 作りたい攻撃の座標
	/// @param _radius 当たり判定の半径
	virtual void MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius);

	
	/// @brief キャラタグの取得
	/// @param  
	/// @return 
	const Collider::TAG& GetCharaTag(void)const { return tag_; }
	
	/// @brief キャラタイプの取得
	/// @param  
	/// @return 
	const CHARACTER_TYPE& GetCharaType(void)const { return characterType_; }


	/// @brief ターゲットと当たったかどうかの取得
	/// @param  
	/// @return ターゲットと当たったかどうか
	const bool GetIsHitTarget(void)const;

	/// @brief 遷移先の更新フェーズ設定
	/// @param _phase どの更新フェーズにするか
	void ChangeUpdatePhase(const UPDATE_PHASE _phase);

	/// @brief 演出から通常時に移行する時に初期化するもの
	/// @param  
	void ChangeDirectToNormal(void);

protected:

	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = - 1.0f;
	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE =  1.5f;
	//中心からのZオフセット
	static constexpr float CENTER_POS_Z_OFFSET = 600.0f;

	// アニメーション
	std::unique_ptr<AnimationController>animationController_;
	//当たった時の処理
	std::unique_ptr<CharacterOnHitBase>onHit_;

	//更新フェーズ
	UPDATE_PHASE updatePhase_;

	//更新フェーズ変更
	std::map <UPDATE_PHASE, std::function<void(void)>>changeUpdate_;

	//更新フェーズの更新
	std::function<void(void)>phazeUpdate_;

	//UIマネージャ
	UIManager& uiMng_;

	//サウンドマネージャ
	SoundManager& soundMng_;

	//エフェクト
	std::unique_ptr<EffectController>effect_;

	//キャラ種別
	CHARACTER_TYPE characterType_;

	//ヒットストップ用カウンタ(フレーム)
	int hitStopFrame_;

	//移動制限
	void MoveLimit(const VECTOR& _stagePos, const VECTOR& _stageSize);

	//コライダ作成
	virtual void MakeColliderGeometry(void) = 0;

	//更新フェーズ	
	void UpdateNone(void);							//何もしない
	virtual void UpdateNormal(void) = 0;			//通常更新
	void UpdateHitStop(void);						//ヒットストップ更新


	//遷移先の更新フェーズ
	void ChangeUpdateNone(void);				//何もしない
	void ChangeUpdateNormal(void);				//通常
	void ChangeUpdateHitStop(void);				//ヒットストップ
private:

};


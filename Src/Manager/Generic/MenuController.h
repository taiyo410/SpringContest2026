#pragma once
#include "../Manager/Resource/SoundManager.h"
#include "../Common/Vector2.h"
#include "../Common/Easing.h"
#include "../Utility/UtilityCommon.h"

#include "../Object/Character/Base/UIBase2D.h"

class FontController;
class SceneManager;
class Easing;

class MenuController:public UIBase2D
{
public:

	/// @brief 
	/// @param  
	MenuController(const int _menuNum, const std::wstring _menu, const Vector2 _pos, int& _fontHandle,bool _isMakeCollider);
	~MenuController(void);

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 中心座標をもとに描画
	/// @param  
	void DrawCenter(void);
	template<typename T>
	void DrawFormat(const T _format);

	/// @brief 解放
	/// @param  
	void Release(void)override;

	/// @brief 演出更新
	/// @param _goalPosX 終端座標X
	/// @param _easeTime 補完時間
	void UpdateDirection(const float _goalPosX, const float _easeTime);

	/// @brief 選択時の更新(選択中のみイージングで動かす)
	/// @param _currentCnt 
	/// @param _localPos 
	/// @param _easeTime 
	/// @param _easeType 
	void SelectUpdate(const float _selectNum,const float _currentCnt, const Vector2 _localPos, const float _easeTime, const Easing::EASING_TYPE _easeType);

	/// @brief 演出を終了させる
	/// @param  
	void SetEase(void);

	/// @brief 当たった時の処理
	/// @param _partner パートナー
	void OnHit(const std::weak_ptr<Collider2D> _partner)override;

	/// @brief 書式付き文字列かどうか
	/// @param _str 調べたい文字列
	/// @return true:書式付き文字列である
	const bool IsHasFormat(void)const;

	/// @brief メニュー番号の取得
	/// @param  
	/// @return 
	const int GetMenuNum(void)const { return menuNum_; }

	/// @brief 演出状態が終了したか
	/// @param  
	/// @return 
	const bool GetIsEndDirectionEase(void)const { return isEndDirectEase_; }

	/// @brief メニューの文字列の取得
	/// @param  
	/// @return 
	const std::wstring& GetMenuButtonString(void)const { return btnStr_; }

	/// @brief 現在の座標を取得
	/// @param  
	/// @return 
	const Vector2& GetCurrentPos(void)const { return curPos_; }

	/// @brief イージング中の取得
	/// @param  
	/// @return 
	const bool GetIsEase(void)const { return isEase_ || isEndDirectEase_; }

	/// @brief 当たり判定の取得
	/// @param  
	/// @return 
	const bool GetIsHit(void)const { return isHit_; }

private:

	//選択中の文字の色
	static constexpr unsigned int SELECT_COL = UtilityCommon::RED;

	//選択していない文字の色
	static constexpr unsigned int UNSELECT_COL = UtilityCommon::WHITE;

	//シーンマネージャ
	SceneManager& scnMng_;

	//フォントハンドル
	int& fontHandle_;

	//Vector2F型
	Vector2F curPosF_;

	//ボタン情報
	int menuNum_;						//メニュー番号
	std::wstring btnStr_;				//ボタンの文字
	Vector2 startPos_;					//イージング前の座標
	Vector2 curPos_ ;					//現在座標
	float directionEaseCnt_;			//イージング時間
	bool isEase_;						//イージング中か
	bool isEndDirectEase_;				//演出イージングが終わったか
	unsigned int color_;				//カラー
	bool isHit_;						//当たり判定
	bool isMakeCollider_;					//コライダを作成するか
};

template<typename T>
inline void MenuController::DrawFormat(const T _format)
{
	DrawFormatStringToHandle(
		curPos_.x, curPos_.y, color_, fontHandle_, btnStr_.c_str(), _format);
}

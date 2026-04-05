#pragma once
#include "../Manager/Resource/SoundManager.h"
#include "../Common/Vector2.h"
#include "../Common/Easing.h"
#include "../Utility/UtilityCommon.h"

class InputManager;
class InputManagerS;
class SoundManager;
class FontController;
class Easing;
class MenuController;
class MenuManager
{
public:

	enum class MENU_STATE
	{
		DIRECTION,		//メニューが出てくる演出
		NORMAL,			//通常
	};

	//yes/noの選択肢
	enum class YES_NO
	{
		YES,
		NO,
		MAX
	};

	struct BTN_INFO
	{
		std::wstring btnStr;				//ボタンの文字
		//TITLE_BTN btnType;				//何のボタンか
		Vector2 startPos;					//イージング前の座標
		Vector2 curPos = startPos;			//現在座標
		float directionEaseCnt = 0.0f;					//イージング時間
		bool isEase = false;				//イージング中か
		bool isEndDirectEase = false;		//演出イージングが終わったか
	};

	/// @brief コンストラクタ
	/// @param  
	MenuManager(void);

	/// @brief デストラクタ
	/// @param  
	~MenuManager(void);

	/// @brief フォントのロード
	/// @param
	void LoadFont(const std::wstring _fontType, const int _size);

	/// @brief メニューの追加
	/// @param  
	
	/// @brief メニューの追加
	/// @param _arrayNum メニュー番号
	/// @param _menu メニューの文字列
	/// @param _pos 初期座標
	/// @param _isMakeCollider コライダを作成するか
	void AddMenu(const int _arrayNum, const std::wstring _menu, const Vector2 _pos,bool _isMakeCollider);

	/// @brief メニュー初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief サイズイージングのフォントハンドルの取得(サイズを変えるためにはフォントを作り直す必要があるため)
	/// @param _arrayNum 配列番号
	/// @param _startSize1 初期サイズ　
	/// @param _goalSize1 最終的なサイズ
	/// @param _easeTime イージング時間
	/// @param _easeType イージング種類
	/// @return サイズ変更後のフォントハンドル
	const int GetSizeEasingFontHandle(const int _arrayNum, const int _startSize, const int _goalSize, const float _easeTime, Easing::EASING_TYPE _easeType);

	/// @brief メニュー演出の更新
	/// @param _disSpawn 出てくる間隔時間
	/// @param _easeTime イージング時間
	/// @param _goalPosX 最終的なX座標
	/// @return true:演出終了	false:演出中
	void UpdateDirection(const float _disSpawn, const float _easeTime, const int _goalPosX);

	/// @brief 通常更新
	/// @param  
	void NormalUpdate(const Vector2 _localPos, const float _easeTime, const Easing::EASING_TYPE _easeType);
	/// @brief イージングなしの通常更新
	/// @param  
	void NormalUpdate(void);

	/// @brief すべてのメニューの演出イージングが終わっているかの取得
	/// @param  
	/// @return 
	const bool IsAllDirectEaseEnd(void) { return isAllDirectEaseEnd_; }

	/// @brief はいフラグの取得
	/// @param  
	/// @return 
	const bool GetIsYes(void) { return yesNoState_ == YES_NO::YES; }

	/// @brief メニューの描画
	/// @param  
	void Draw(void);

	/// @brief 中心座標で描画
	/// @param  
	void DrawCenter(void);

	/// @brief 書式付き文字列の描画
	/// @param
	template<typename T>
	void DrawFormat(std::vector<T> _format);


	/// @brief メニューの選択
	/// @param _src 選択時のボタン
	void SelectMenu(const SoundManager::SRC _src = SoundManager::SRC::NONE);

	/// @brief 選択中のメニュー番号のセット
	/// @param _num セットしたい番号
	void SetSelectMenuNum(const int _num) { selectMenuNum_ = _num; }

	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void AddSelectMenuNum(void);
	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void SubSelectMenuNum(void);

	/// @brief 選択中のメニュー番号の取得
	/// @param  
	/// @return 
	const int GetSelectMenuNum(void) { return selectMenuNum_; }

	/// @brief メニューリストの取得
	/// @param  
	/// @return メニューリストの情報
	const std::vector<std::unique_ptr<MenuController>>& GetMenuList(void)const { return menuList_; }

	/// @brief 文字列の中心座標の取得
	/// @param _str 調べたい文字列
	/// @return 
	const Vector2 GetMenuCenterPos(const std::wstring _str)const;

private:

	//通常メニューのイージング時間
	static constexpr float SELECT_EASING_TIME = 0.5f;

	//ゲーム終了確認メニューの大きさ
	static constexpr int CHECK_EXIT_MENU_SIZE_X = 600;
	static constexpr int CHECK_EXIT_MENU_SIZE_Y = 200;

	//はい、いいえの文字間隔
	static constexpr int YES_NO_DISTANCE_X = 70;
	static constexpr int YES_NO_DISTANCE_Y = 100;

	//終了するかの文字の確認メニューとの間隔
	static constexpr int QUESTION_OFFSET = 30;

	//はい/いいえの選択肢の数
	static constexpr int YES_NO_NUM = 2;

	//選択中の文字の色
	static constexpr unsigned int SELECT_COL = UtilityCommon::RED;

	//選択していない文字の色
	static constexpr unsigned int UNSELECT_COL = UtilityCommon::WHITE;

	//デフォルトのフォントサイズ
	int defaultFontSize_;

	//フォントハンドル
	std::wstring defaultFontHandle_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//メニューのフォントハンドル
	int fontHandle_;

	//サイズイージングの時間カウント
	float sizeEaseCnt_;

	//演出でメニューが出てくるときの間隔カウント
	float disSpawnCnt_;

	//状態遷移
	std::unordered_map<MENU_STATE, std::function<void(void)>>changeState_;

	//はい/いいえの文字列
	std::wstring yesNoStrTable_[YES_NO_NUM];

	//現在の状態
	MENU_STATE currentState_;

	//更新関数
	std::function<void(void)>updateFunc_;

	//メニュー表
	//std::unordered_map<int, BTN_INFO> menuList_;

	std::vector<std::unique_ptr<MenuController>>menuList_;

	//サイズイージングのフォントハンドルのテーブル
	std::unordered_map<int, int> sizeEasingFontHandleTable_;

	//全体の演出が終わったか
	bool isAllDirectEaseEnd_;

	//選択中のメニュー
	int selectMenuNum_;

	//はい/いいえ状態
	//bool isYes_;
	YES_NO yesNoState_;

	//入力
	InputManager& ins_;
	InputManagerS& insS_;

	//フォントマネージャ
	std::unique_ptr<FontController> fontController_;

	//サウンド
	SoundManager& soundMng_;

	/// @brief 選択中のカラーを決める
	/// @param _unselectCol 
	/// @param _selectCol 
	unsigned int DecideColor(const int btnNum);



};

template<typename T>
void MenuManager::DrawFormat(std::vector<T> _format)
{
	unsigned int color = UtilityCommon::WHITE;
	int i = 0;
	for (auto& menu : menuList_)
	{
		//選択中のメニューはサイズイージングして赤色で描画
		//color = DecideColor(menu.first);
		if (menu->IsHasFormat())
		{
			menu->DrawFormat(_format[i]);
			i++;
		}
		else
		{
			//DrawStringToHandle(
			//	menu.second.curPos.x, menu.second.curPos.y, menu.second.btnStr.c_str(), color, fontHandle_);
			menu->Draw();
		}

	}
}



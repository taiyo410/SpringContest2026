#pragma once
#include<unordered_map>
#include<string>
#include"../Base/CharacterBase2D.h"

class CursorOnHit;
class FontController;

class Cursor : public CharacterBase2D
{
public:

	//説明の種類
	enum class EXPLAN
	{
		NO_IMAGE,			//画像なし
		DAIMYO,				//大名
		DAIMYO_ALTERNATE,	//参勤
		ENHANCEMENT,		//強化
		DETAILS,			//詳細
		DISSATISFACTION,	//不満
	};

	//コンストラクタ
	Cursor(void);

	//コンストラクタ
	~Cursor(void);

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider2D> _partner)override;

	//説明吹き出しの変更
	void ChangeExplan(const EXPLAN _type);

private:

	//吹き出し相対座標
	static constexpr Vector2F SPEECH_LOCAL_POS = { 120.0f,-120.0f };
	static constexpr int FONT_SIZE = 24;
	static constexpr int FONT_TICKNESS = 24;

	//説明の種類
	EXPLAN explanType_;

	//吹き出し画像
	int speechBubbleImg_;

	//吹き出し座標
	Vector2F speechPos_;

	//説明文字列
	std::unordered_map<EXPLAN, std::wstring> explanStr_;

	//説明画像
	std::unordered_map<EXPLAN, int> explanImg_;

	//ヒット処理
	std::unique_ptr<CursorOnHit> onHit_;
	
	//フォント
	std::unique_ptr<FontController> font_;
	int fontHandle_;
};


#pragma once
#include "../Character/Base/UIBase2D.h"
#include "../Manager/Resource/ResourceManager.h"
class GaugeController:public UIBase2D
{
public:

	/// @brief コンストラクタ
	/// @param _maskSrc マスクで使用するリソース
	/// @param _pos 参照座標
	/// @param _per 参照割合(0.0f~1.0f)
	/// @param _col1 左側のカラー
	/// @param _col2 右側のカラー(右から左へ補完するための色)
	GaugeController(ResourceManager::SRC _maskSrc, Vector2F& _pos,Vector2F& _size,float& _per,FLOAT4& _col1,FLOAT4& _col2);

	/// @brief デストラクタ
	/// @param  
	~GaugeController(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 解放
	/// @param  
	void Release(void)override;

	/// @brief ヒット処理
	/// @param _partner 
	void OnHit(const std::weak_ptr<Collider2D> _partner)override{}

private:

	//定数バッファ
	static constexpr int GAUGE_CONST_BUF = 3;
	//カラー1の定数バッファ番号
	static constexpr int COLER1_BUF_NUM = 0;
	//カラー2の定数バッファ番号
	static constexpr int COLER2_BUF_NUM = 1;
	//現在の割合以上のゲージの色
	static constexpr FLOAT4 GAUGE_OVER_COLOR = { 0.5f,0.5f,0.5f,1.0f };

	//ゲージ割合の定数バッファ番号
	static constexpr int PERCENT_BUF_NUM = 2;
	//座標
	Vector2F& pos_;
	//サイズ
	Vector2F& size_;
	//割合
	float& per_;
	//マスク画像
	int maskImg_;
	//マスク画像種類
	ResourceManager::SRC maskSrc_;

	//カラー
	FLOAT4& col1_;
	FLOAT4& col2_;

};


#pragma once

class Easing;

class SceneUi
{
public:
    struct FontData
    {
        std::string message;  // 表示する文字列
    };

    SceneUi(void);
    ~SceneUi(void);

    void Draw(int baseYOverride = -1);
    void FontBlinking(void);
    void DrawFont(int baseYOverride);

    void AddCharctor(const char* _char);
    void SetCurrentIndex(int index);
    int GetCurrentIndex(void) const;
    int GetMaxIndex(void) const;

private:
	static constexpr int FONT_DEFAULT_SIZE = 42;
	static constexpr int FONT_BLINK_SIZE = 48;
	static constexpr float EASING_DURATION = 0.3f; // 点滅の周期（フレーム数）

	// イージング
	std::unique_ptr<Easing> easing_;

    //サイズ
	int fontSize_;
	// フレームカウンタ
    int frameCount_;
	//イージングの時間
	float easingTime_;

	// 点滅の間隔（フレーム数）
    const int blinkInterval_ = 30;
	// 点滅状態
    bool isBlinking_;
	// フォントデータのリスト
    std::vector<FontData> fontList_;
    // 現在選択中のインデックス
    int currentIndex_;  
};


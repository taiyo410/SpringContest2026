#pragma once
class DataManager
{
public:
	
	static void CreateInstance(void);

	static DataManager& GetInstance(void);

	static void Destroy(void);

	void Reset(void);

	void AddCount(void);

	int GetCurrentYear(void) const { return currentYear_; }

	void SaveFinalRecord(void) { finalYearReached_ = currentYear_; }

	int GetFinalRecord(void) const { return finalYearReached_; }

private:

	DataManager(void) = default;

	~DataManager(void) = default;

	DataManager(const DataManager&) = default;

	DataManager& operator = (const DataManager&) = default;

	static constexpr int START_YEAR = 1635;

	static constexpr int MAX_YEAR = 2100;

	// 年代データ関連
	int currentYear_;        // 現在の年
	int sankinCount_;        // 1年に進むための内部カウント
	int finalYearReached_;   // 最終年
};


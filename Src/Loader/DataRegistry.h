#pragma once

class DataRegistry
{
public:

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static DataRegistry& GetInstance(void);

	//全外部ファイルを生成
	void CreateAll(void);

	//削除
	void Destroy(void);

protected:

	//インスタンス
	static DataRegistry* instance_;

	//コンストラクタ
	DataRegistry(void);
	~DataRegistry(void);

	//コピー禁止
	DataRegistry(const DataRegistry& _copy) = delete;
	DataRegistry& operator=(const DataRegistry& _copy) = delete;
};
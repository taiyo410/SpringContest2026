#pragma once
#include<unordered_map>
#include"ILoader.h"
#include"JsonLoader.h"
#include"CsvLoader.h"

template<typename T>
class LoaderManager
{
public:

    //インスタンス生成
    static void CreateInstance(void)
    {
        if (instance_ == nullptr)
        {
            instance_ = new T();
        }
    }

    //インスタンス取得
    static T& GetInstance(void)
    {
        if (instance_ == nullptr)
        {
            CreateInstance();
        }

        return *instance_;
    }

    //削除
    void Destroy(void)
    {
        delete instance_;
        instance_ = nullptr;
    }

    /// @brief ファイルの全データを取得
    /// @param _fileName ファイル名 
    /// @return データ
    const std::vector<T>& GetfileData(const std::string& _fileName)
    {
        //ロード済みか
        if (!isFileDataLoaded_[_fileName])
        {
            //ロード
            FileLoad(_fileName);
        }

        return fileDataList_[_fileName];
    }

    /// @brief ファイル内のnameでデータを取得
    /// @param _filename ファイル名
    /// @param _dataName データ名
    /// @return データ
    const std::vector<T>& GetNameToData(const std::string& _fileName, const std::string& _dataName)
    {
        //ロード済みか
        if (!isNameToDataLoaded_[_dataName])
        {
            //ロード
            NameToDataLoad(_fileName);
        }

        //名前に対応したデータが存在するか
        auto it = nameToDataList_.find(_dataName);
        if (it == nameToDataList_.end()) {
            throw std::runtime_error("データ内に次の名前は見つかりませんでした: " + _dataName);
        }
        return it->second;
    }

private:

    //インスタンス
    static T* instance_;

    //拡張子をstring型で格納
    const std::string JSON = "json";
    const std::string CSV = "csv";

    //ファイルごとのデータ
    std::unordered_map<std::string, std::vector<T>> fileDataList_;
    std::unordered_map<std::string, bool> isFileDataLoaded_;

	//ファイル内データ名対応データ
	std::unordered_map<std::string, std::vector<T>> nameToDataList_;
	std::unordered_map<std::string, bool> isNameToDataLoaded_;

    //コンストラクタ
    LoaderManager(void) {};
    ~LoaderManager(void) {};

    //コピー禁止
    LoaderManager(const LoaderManager& _copy) = delete;
    LoaderManager& operator=(const LoaderManager& _copy) = delete;

    /// @brief ロード
    /// @param filename ファイル名
    void FileLoad(const std::string& _fileName)
    {
        namespace fs = std::filesystem;
        fs::path path(_fileName);
        std::string ext = path.extension().string();

        // もし ext に ".json" が入るので、先頭の '.' を除く
        if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);

        std::cout << "[DEBUG] ext = [" << ext << "]" << std::endl;

        //ローダー
        std::unique_ptr<ILoader<T>> loader;

        //Json
        if (ext.c_str() == JSON) {
            loader = std::make_unique<JsonLoader<T>>();
        }
        //csv
        else if (ext.c_str() == CSV) {
            loader = std::make_unique<CsvLoader<T>>();
        }
        //対応なし
        else {
            throw std::runtime_error("以下の拡張子は対応していません->" + ext);
        }

        //各ローダーに命令
        fileDataList_[_fileName] = loader->Load(_fileName);
    }

    //オブジェクトの名前に対応したロード
    void NameToDataLoad(const std::string& _fileName)
    {
        // 拡張子を取得
        auto pos = _fileName.find_last_of('.');
        std::string ext = (pos == std::string::npos) ? "" : _fileName.substr(pos + 1);

        //ローダー
        std::unique_ptr<ILoader<T>> loader;

        //Json
        if (ext == "json") {
            loader = std::make_unique<JsonLoader<T>>();
        }
        //csv
        else if (ext == "csv") {
            loader = std::make_unique<CsvLoader<T>>();
        }
        //対応なし
        else {
            throw std::runtime_error("次の拡張子は対応していません:" + ext);
        }

        //各ローダーに命令
        auto dataList = loader->Load(_fileName);

        //取得データを保存
        for (const auto& data : dataList)
        {
            //保存
            nameToDataList_[data.name] = dataList;
            
            //ロードした
            isNameToDataLoaded_[data.name] = true;
        }
    }
};

//静的インスタンスの初期化
template<typename T>
T* LoaderManager<T>::instance_ = nullptr;
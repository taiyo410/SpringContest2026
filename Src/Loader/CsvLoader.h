#pragma once
#include<iostream>
#include "ILoader.h"

template<typename T>
class CsvLoader : public ILoader<T>
{
public:

    /// @brief ロード
    /// @param _filename csvファイル名
    /// @return データ
    std::vector<T> Load(const std::string& _filename) override {
        std::ifstream ifs(_filename);
        std::vector<T> result;
        std::string line;

        while (std::getline(ifs, line)) {
            std::stringstream ss(line);
            T data{};
            FromCsv(ss, data);
            result.push_back(data);
        }
        return result;
    }
};


#pragma once

template<typename T>
class ILoader
{
public:

    //コンストラクタ
    ILoader(void) = default;

    //デストラクタ
    virtual ~ILoader(void) = default;
  
    //ロード
    virtual std::vector<T> Load(const std::string& filename) = 0;
};


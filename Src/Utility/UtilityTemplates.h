#pragma once
#include <vector>
#include <algorithm>

// テンプレート専用ユーティリティクラス
class UtilityTemplates
{
public:

    /// <summary>
    /// 配列内に値があるか調べる
    /// </summary>
    /// <typeparam name="T">配列内の要素の型</typeparam>
    /// <param name="vec">検索対象の配列</param>
    /// <param name="value">検索する値</param>
    /// <returns>値が見つかればtrue,なければfalseを返す</returns>
    template<typename T>
    static bool ContainsValue(const std::vector<T>& vec, const T& value) 
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }

    /// <summary>
    /// 動的配列の空になった配列を消す
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name=""></param>
    /// <returns></returns>
    template<typename T>
    static int EraseVectorArray(std::vector<T>& vec)
    {
        int ret= static_cast<int>(std::erase_if(vec, [](auto& a) {return a == nullptr; }));
        return ret;
    }

    /// @brief 値を増やし、範囲内に収める
    /// @param index 増やしたい値
    /// @param max 上限値
    /// @param addIndex 増やし幅(デフォルトで1)
    /// @return 増やした後の値
    template<typename T>
    static void AddIndex(T& index, T max, const T addIndex = 1)
    {
        index += addIndex;
        if (index >= max)
        {
            index = max;
        }
    }

    /// @brief 値を減らし、範囲内に収める
    /// @param index 減らしたい値
    /// @param min 下限値 
    /// @param subIndex 減らし幅
    /// @return 減らした後の値
    template<typename T>
    static void SubIndex(T& index, T min, const T subIndex = 1)
    {
        index -= subIndex;
        if (index <= min)
        {
            index = min;
        }
    }

};
#include <random>
#include "UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"

int UtilityCommon::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> UtilityCommon::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;

    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double UtilityCommon::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float UtilityCommon::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int UtilityCommon::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double UtilityCommon::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float UtilityCommon::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int UtilityCommon::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

double UtilityCommon::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double UtilityCommon::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int UtilityCommon::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int UtilityCommon::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > 180.0f)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -180.0f)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}


bool UtilityCommon::IsTimeOver(float& totalTime, const float& waitTime)
{
    //デルタタイム
    auto delta = SceneManager::GetInstance().GetDeltaTime();
    totalTime += delta;

    //待機時間を超過しているか判断
    if (totalTime >= waitTime)
    {
        return true;
    }

    return false;
}

int UtilityCommon::WrapIndex(int index, int max)
{
    return (index % max + max) % max;
}

std::vector<std::vector<int>> UtilityCommon::LoadCSVData(const std::wstring& filePath)
{
    std::vector<std::vector<int>> csvData;

    //ファイルの準備
    std::wifstream ifs = std::wifstream(filePath);
    // ifs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // UTF-8対応
    if (!ifs)
    {
        OutputDebugString(L"ステージのifstream準備失敗\n");
        return csvData;
    }

    std::wstring line;

    // CSVデータの読み込み
    while (getline(ifs, line))
    {
        std::wstringstream lineStream(line);
        std::wstring cell;
        std::vector<int> row;

        // 各セルをカンマで区切って取得し、数値に変換して行に追加
        while (getline(lineStream, cell, L',')) {
            try
            {
                row.push_back(std::stoi(cell));
            }
            catch (...)
            {
                row.push_back(0); // 数値変換失敗時は0を入れる
            }
        }
        csvData.push_back(row); // 行を追加
    }

    ifs.close();

    return csvData;
}

std::wstring UtilityCommon::GetWStringFromString(const std::string& str)
{
    //根幹数を正しく使うには2回呼び出す必要がある
    std::wstring ret = L"";
    //最初の呼び出しは返される文字列の文字列数
    auto length = MultiByteToWideChar(
        CP_ACP,									//現在のコードページ
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,	//エラーしたら情報を返す
        str.c_str(),							//元の文字列へのポインタ
        static_cast<int>(str.length()),							//元の文字列の長さ
        nullptr,								//変換先の文字列のバッファ
        0										//最後の引数をnullptr,0にすることで
    );											//wstringに必要な文字列数を返している

    if (length <= 0)
    {
        return L"";
    }
    ret.resize(length);	//必要なメモリを確保

    //二回目の呼び出しは実際の文字列を得る
    MultiByteToWideChar(
        CP_ACP,									// 文字コード
        MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
        str.c_str(),							// 変換元の文字列
        static_cast<int>(str.length()),							// 得られたワイド文字列を入れるアドレス
        ret.data(),								// 得られたワイド文字列を入れるメモリサイズ
        static_cast<int>(ret.size())								// 変換先の文字列のバッファのサイズ
    );

    return ret;
}

std::string UtilityCommon::GetStringFromWString(const std::wstring& wstr)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
    return result;
}

int UtilityCommon::GetRandomValue(int _min, int _max)
{
    //ランダムエンジン
    static std::mt19937 mt(std::random_device{}());

    // min ～ max の範囲で一様分布
    std::uniform_int_distribution<int> dist(_min, _max);

    //ランダムの値
    return dist(mt);
}

void UtilityCommon::CountUp(float& count, const float& max, const bool isLoop, const float& delta)
{
	if (delta < 0)
    {
        count += FIXED_DELTA_TIME;
    }
    else
    {
        count += delta;
    }

	if (count > max)
    {
		isLoop ? count -= max : count = max;
    }
}

void UtilityCommon::CountDown(float& count, const float& min, const bool isLoop, const float& delta)
{
    if (delta < 0.0f)
    {
        count -= FIXED_DELTA_TIME;
    }
    else
    {
        count -= delta;
    }
    if (count < min)
    {
        isLoop ? count += min : count = min;
	}
}

Vector2 UtilityCommon::GetStringSizeToHandle(const int fontHandle, const std::wstring _str)
{
    Vector2 size = {};
    GetDrawStringSizeToHandle(
        &size.x,
        &size.y,
        NULL,
        _str.c_str(),
        static_cast<int>(wcslen(_str.c_str())),
        fontHandle);
    return size;
}

const bool UtilityCommon::IsHasFormat(const std::wstring _str)
{
    if (_str.find(L"%d") != std::wstring::npos || _str.find(L"%f") != std::wstring::npos)
    {
        return true;
    }
    return false;
}

const bool UtilityCommon::EqualFloat4(const FLOAT4& _value1, const FLOAT4& _value2)
{
    if (_value1.x == _value2.x && _value1.y == _value2.y && _value1.z == _value2.z && _value1.w == _value2.w)
    {
        return true;
    }
	return false;
}

const FLOAT4 UtilityCommon::GetColorF(const unsigned int color)
{

	// カラーコードからRGB成分を抽出し、0.0f～1.0fの範囲に正規化してFLOAT4に格納
    FLOAT4 ret = {};
    ret.x = ((color >> 16) & 0xFF) / 255.0f; // 赤
    ret.y = ((color >> 8) & 0xFF) / 255.0f;  // 緑
    ret.z = (color & 0xFF) / 255.0f;         // 青
    ret.w = 1.0f;                            // アルファ（不透明）
	return ret;
}

#pragma once
#include <functional>
#include <string>

class MoneyManager {
public:
    // 建構、重設
    MoneyManager(int initial = 0);

    // 取得目前餘額
    int GetBalance() const;

    // 增加金錢
    void Add(int amount);

    // 扣除金錢，若餘額不足可回傳 false
    bool Spend(int amount);

    // 更新顯示文字 (呼叫外部的 UI 更新函式)
    void SetOnChangeCallback(std::function<void(int)> callback);

private:
    int m_Balance;
    std::function<void(int)> m_Callback;
};
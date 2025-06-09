#pragma once
#include <functional>
#include <string>

class MoneyManager {
public:
    MoneyManager(int initial = 0);

    // 取得金額
    int GetBalance() const;

    // 增加金錢
    void Add(int amount);

    // 扣除金錢
    bool Spend(int amount);

    // 更新文字
    void SetOnChangeCallback(std::function<void(int)> callback);

private:
    int m_Balance;
    std::function<void(int)> m_Callback;
};
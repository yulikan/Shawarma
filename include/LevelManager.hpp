#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <random>
// 定義單個客人的設定
struct CustomerConfig {
    std::string customerImage;  // 客人的圖片路徑或識別字串
    glm::vec2 position;         // 客人的出現位置
    std::string foodRequest;    // 客人要求的食物（例如 "fries", "roll", "sauce"）
    std::string foodIcon;      //客人頭上要有食物要求
    std::vector<std::string> requiredToppings;
};

// 定義整個關卡的設定
struct LevelData {
    std::string backgroundImage;              // 背景圖片路徑
    std::vector<CustomerConfig> customers;    // 關卡中所有客人的設定
    int passThreshold = 0;  // 【新增】該關的通關標準金額

};

class LevelManager {
public:
    LevelManager();
    ~LevelManager();

    // 載入所有關卡資料（這裡可以用硬編碼或讀取檔案）
    void LoadLevels();

    // 取得目前關卡資料
    const LevelData& GetCurrentLevel() const;

    // 切換到下一個關卡
    void NextLevel();

    // 重置關卡（例如回到第一關）
    void Reset();

    // ----- 新增這些函式 -----
    // 切換到新關卡時呼叫：初始化 pendingConfigs、索引歸零
    void StartLevel();

    // 場上還能補新的客人嗎？
    bool HasPendingCustomer() const;

    // 取出下一個客人設定（並把索引往後推）
    CustomerConfig GetNextCustomer();

    // 本關所有客人都已經出完了嗎？
    bool IsLevelFinished() const;
    size_t GetCurrentLevelIndex() const;
    int GetCurrentLevelNumber() const;

    // 新增這兩行 ↓↓↓
    std::vector<CustomerConfig> SpawnCustomers(int currentActiveCount) ;
    int GetMaxActiveCustomers() const { return m_MaxActiveCustomers; }
    void SetLevelIndex(size_t index);  // 新增：手動設定要跳的關卡
    int GetTotalLevelCount() const;

    const std::vector<LevelData>& GetLevels() const { return m_Levels; }

private:
    std::vector<LevelData> m_Levels;
    size_t m_CurrentLevelIndex;
    std::unordered_map<std::string, int> m_foodRevenue = {
        {"FrenchFries", 30},
        {"Soda",        20},
        {"Cola",        25},
        {"Juice",       15}
    };

    // 2) 客製化捲餅各「配料」單價，還要有一個 baseRollPrice
    //    例如：baseRoll 本身 10 元，之後每一種配料（肉、醬、醃黃瓜、薯條）單獨計價
    std::unordered_map<std::string, int> m_toppingPrice = {
        {"shaved_meat", 20},
        {"sauce",        5},
        {"pickle",       5},
        {"fries",       10}
    };
    int m_baseRollPrice = 10; // 沒有任何配料時，最便宜的捲餅價 10 元

    // 3) 隨機亂數引擎
    std::random_device m_rd;
    std::mt19937       m_gen;

    // 助手函式：計算「某組配料向量」的實際捲餅售價
    int CalcRollPrice(const std::vector<std::string>& toppings) const {
        int price = m_baseRollPrice;
        for (auto &t : toppings) {
            auto it = m_toppingPrice.find(t);
            if (it != m_toppingPrice.end()) {
                price += it->second;
            }
        }
        return price;
    }
    // 新增
    std::vector<CustomerConfig> m_PendingConfigs; // 本關剩下還沒出場的客人設定
    int m_NextCustomerIdx;                         // 下一位要出場的索引
    int m_MaxActiveCustomers = 3;                  // 同一關最多同時在場的客人數
};

#endif // LEVEL_MANAGER_HPP
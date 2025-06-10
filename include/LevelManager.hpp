#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <random>
// 客人的設定
struct CustomerConfig {
    std::string customerImage;  // 客人的圖片路徑或識別字串
    glm::vec2 position;         // 客人的出現位置
    std::string foodRequest;    // 客人要求的食物
    std::string foodIcon;      // 客人頭上要有食物要求
    std::vector<std::string> requiredToppings;
};

// 定義整個關卡的設定
struct LevelData {
    std::string backgroundImage;              // 背景圖片路徑
    std::vector<CustomerConfig> customers;    // 關卡中所有客人的設定
    int passThreshold = 0;  // 通關標準金額

};

class LevelManager {
public:
    LevelManager();
    ~LevelManager();

    void LoadLevels();

    const LevelData& GetCurrentLevel() const;

    void NextLevel();

    void Reset();

    void StartLevel();

    // 場上還能補新的客人嗎
    bool HasPendingCustomer() const;

    // 取出下一個客人設定
    CustomerConfig GetNextCustomer();

    // 本關所有客人都已經出完了嗎
    bool IsLevelFinished() const;
    size_t GetCurrentLevelIndex() const;
    int GetCurrentLevelNumber() const;

    std::vector<CustomerConfig> SpawnCustomers(int currentActiveCount) ;
    int GetMaxActiveCustomers() const { return m_MaxActiveCustomers; }
    void SetLevelIndex(size_t index);
    int GetTotalLevelCount() const;

    const std::vector<LevelData>& GetLevels() const { return m_Levels; }
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
private:
    std::vector<LevelData> m_Levels;
    size_t m_CurrentLevelIndex;
    std::unordered_map<std::string, int> m_foodRevenue = {
        {"FrenchFries", 30},
        {"Soda",        20},
        {"Cola",        25},
        {"Juice",       15}
    };

    // 客製化捲餅各「配料」單價
    std::unordered_map<std::string, int> m_toppingPrice = {
        {"shaved_meat", 20},
        {"sauce",        5},
        {"pickle",       5},
        {"fries",       10}
    };
    int m_baseRollPrice = 10; // 純餅皮 10 元

    // 隨機亂數引擎
    std::random_device m_rd;
    std::mt19937       m_gen;



    std::vector<CustomerConfig> m_PendingConfigs;
    int m_NextCustomerIdx;
    int m_MaxActiveCustomers = 3;
};

#endif // LEVEL_MANAGER_HPP
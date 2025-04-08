#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include <vector>
#include <string>
#include <glm/vec2.hpp>

// 定義單個客人的設定
struct CustomerConfig {
    std::string customerImage;  // 客人的圖片路徑或識別字串
    glm::vec2 position;         // 客人的出現位置
    std::string foodRequest;    // 客人要求的食物（例如 "fries", "roll", "sauce"）
    std::string foodIcon;      //客人頭上要有食物要求
};

// 定義整個關卡的設定
struct LevelData {
    std::string backgroundImage;              // 背景圖片路徑
    std::vector<CustomerConfig> customers;    // 關卡中所有客人的設定
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

private:
    std::vector<LevelData> m_Levels;
    size_t m_CurrentLevelIndex;
};

#endif // LEVEL_MANAGER_HPP

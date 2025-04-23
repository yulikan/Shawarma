#include "LevelManager.hpp"

LevelManager::LevelManager() : m_CurrentLevelIndex(0) {
    LoadLevels();
}

LevelManager::~LevelManager() {}

void LevelManager::LoadLevels() {
    // 以下示例中直接硬編碼關卡數據，實際可以考慮從 JSON 或其他格式讀取

    // 關卡 1
    LevelData level1;
    level1.backgroundImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/background/restaurant.png";
    CustomerConfig cust1;
    cust1.customerImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer1.png";
    cust1.position = glm::vec2(100.0f, 85.0f);
    cust1.foodIcon = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/FrenchFries.png";
    cust1.foodRequest = "FrenchFries";
    level1.customers.push_back(cust1);

    // 關卡 2
    LevelData level2;
    level2.backgroundImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/background/restaurant.png";
    CustomerConfig cust2;
    cust2.customerImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer2.png";
    cust2.position = glm::vec2(100.0f, 55.0f);
    cust2.foodIcon = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/Roll.png";
    cust2.foodRequest = "Roll";
    level2.customers.push_back(cust2);
    CustomerConfig cust3;
    cust3.customerImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer3.png";
    cust3.position = glm::vec2(300.0f, 55.0f);
    cust3.foodIcon = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/FrenchFries.png";
    cust3.foodRequest = "FrenchFries";
    level2.customers.push_back(cust3);

    m_Levels.push_back(level1);
    m_Levels.push_back(level2);
}

const LevelData& LevelManager::GetCurrentLevel() const {
    return m_Levels[m_CurrentLevelIndex];
}

void LevelManager::NextLevel() {
    m_CurrentLevelIndex = (m_CurrentLevelIndex + 1) % m_Levels.size();
}

void LevelManager::Reset() {
    m_CurrentLevelIndex = 0;
}

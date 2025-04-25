#include "LevelManager.hpp"

LevelManager::LevelManager() : m_CurrentLevelIndex(0) {
    LoadLevels();
    // 載入完所有關卡設定後，初始化第一關的 pendingConfigs、索引
    StartLevel();
}

LevelManager::~LevelManager() {}

void LevelManager::LoadLevels() {
    // 以下示例中直接硬編碼關卡數據，實際可以考慮從 JSON 或其他格式讀取

    // 關卡 1
    LevelData level1;
    level1.backgroundImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 5 個客人的圖檔、位置和請求
    std::vector<std::string> images = {
        "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer3.png",
        "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer2.png"
    };
    std::vector<glm::vec2> positions = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
        {-200.0f, 85.0f},
        {-100.0f, 55.0f}
    };
    // 這邊示範前兩位要薯條，後三位要卷餅
    std::vector<std::string> requests = {
        "FrenchFries", "FrenchFries",
        "Roll", "Roll", "Roll"
    };

    for (int i = 0; i < 5; ++i) {
        CustomerConfig cust;
        cust.customerImage = images[i];
        cust.position      = positions[i];
        cust.foodRequest   = requests[i];
        cust.foodIcon      = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/"
                             + requests[i] + ".png";
        level1.customers.push_back(cust);
    }

    // 關卡 2
    LevelData level2;
    level2.backgroundImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/background/restaurant.png";
    CustomerConfig cust2;
    cust2.customerImage = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Customer/customer2.png";
    cust2.position = glm::vec2(100.0f, 55.0f);
    cust2.foodIcon = "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/Roll.png";
    cust2.foodRequest = "Roll";
    level2.customers.push_back(cust2);

    // 關卡3
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
    StartLevel();
}

void LevelManager::Reset() {
    m_CurrentLevelIndex = 0;
    StartLevel();
}

// ==== 新增方法實作 ====

void LevelManager::StartLevel() {
    // 複製當前關卡所有客人設定到待補清單
    m_PendingConfigs   = m_Levels[m_CurrentLevelIndex].customers;
    m_NextCustomerIdx  = 0;
}

bool LevelManager::HasPendingCustomer() const {
    // 還有尚未出場的客人？
    return m_NextCustomerIdx < static_cast<int>(m_PendingConfigs.size());
}

CustomerConfig LevelManager::GetNextCustomer() {
    // 取出下一個 customer 設定，並推進索引
    return m_PendingConfigs[m_NextCustomerIdx++];
}

bool LevelManager::IsLevelFinished() const {
    // 所有客人都已取光了
    return m_NextCustomerIdx >= static_cast<int>(m_PendingConfigs.size());
}

// ==== 新增：根據當前 active 數量，自動算出本幀要 spawn 哪些客人 ====
std::vector<CustomerConfig> LevelManager::SpawnCustomers(int currentActiveCount)  {
    std::vector<CustomerConfig> toSpawn;
    // 還能補幾位客人
    int canSpawn = m_MaxActiveCustomers - currentActiveCount;
    while (canSpawn > 0 && HasPendingCustomer()) {
        toSpawn.push_back(GetNextCustomer());
        --canSpawn;
    }
    return toSpawn;
}
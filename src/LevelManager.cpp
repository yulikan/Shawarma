#include "LevelManager.hpp"

LevelManager::LevelManager() : m_CurrentLevelIndex(0) {
    LoadLevels();
    // 載入完所有關卡設定後，初始化第一關的 pendingConfigs、索引
    StartLevel();
}

LevelManager::~LevelManager() {}

void LevelManager::LoadLevels() {
    // 關卡 1
    LevelData level1;
    level1.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 5 個客人的圖檔、位置和請求
    std::vector<std::string> images = {
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer3.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer4.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer5.png"
    };
    std::vector<glm::vec2> positions = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
        {-200.0f, 50.0f},
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
        cust.foodIcon      = "C:/Users/yello/Shawarma/Resources/Image/Food/"
                             + requests[i] + ".png";
        level1.customers.push_back(cust);
    }
    m_Levels.push_back(level1);

    // 關卡 2
    LevelData level2;
    level2.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 6 個客人的圖檔、位置和請求
    std::vector<std::string> images2 = {
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer3.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer4.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer5.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer6.png",
    };
    std::vector<glm::vec2> positions2 = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
        {-200.0f, 50.0f},
        {-100.0f, 55.0f},
        {-350.0f, 55.0f},
    };
    // 這邊示範前兩位要薯條，後三位要卷餅
    std::vector<std::string> requests2 = {
        "FrenchFries", "FrenchFries",
        "Roll", "Roll", "Roll", "Roll"
    };

    for (int i = 0; i < 6; ++i) {
        CustomerConfig cust;
        cust.customerImage = images2[i];
        cust.position      = positions2[i];
        cust.foodRequest   = requests2[i];
        cust.foodIcon      = "C:/Users/yello/Shawarma/Resources/Image/Food/"
                             + requests2[i] + ".png";
        level2.customers.push_back(cust);
    }
    m_Levels.push_back(level2);

    // 關卡3
    LevelData level3;
    level3.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 7 個客人的圖檔、位置和請求
    std::vector<std::string> images3 = {
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer3.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer4.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer5.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer6.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer7.png",
    };
    std::vector<glm::vec2> positions3 = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
        {-200.0f, 50.0f},
        {-100.0f, 55.0f},
        {100.0f, 85.0f},
        {0.0f, 55.0f},
    };
    // 這邊示範前兩位要薯條，後三位要卷餅
    std::vector<std::string> requests3 = {
        "FrenchFries", "FrenchFries",
        "Roll", "Roll", "Roll", "Roll", "FrenchFries"
    };

    for (int i = 0; i < 7; ++i) {
        CustomerConfig cust;
        cust.customerImage = images3[i];
        cust.position      = positions3[i];
        cust.foodRequest   = requests3[i];
        cust.foodIcon      = "C:/Users/yello/Shawarma/Resources/Image/Food/"
                             + requests3[i] + ".png";
        level3.customers.push_back(cust);
    }
    m_Levels.push_back(level3);


    // 關卡4
    LevelData level4;
    level4.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 7 個客人的圖檔、位置和請求
    std::vector<std::string> images4 = {
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer3.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer4.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer5.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer6.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer7.png",
    };
    std::vector<glm::vec2> positions4 = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
        {-200.0f, 50.0f},
        {-100.0f, 55.0f},
        {100.0f, 85.0f},
        {0.0f, 55.0f},
    };
    // 這邊示範前兩位要薯條，後三位要卷餅
    std::vector<std::string> requests4 = {
        "FrenchFries", "FrenchFries",
        "Roll", "Roll", "Roll", "Roll", "FrenchFries"
    };

    for (int i = 0; i < 7; ++i) {
        CustomerConfig cust;
        cust.customerImage = images4[i];
        cust.position      = positions4[i];
        cust.foodRequest   = requests4[i];
        cust.foodIcon      = "C:/Users/yello/Shawarma/Resources/Image/Food/"
                             + requests4[i] + ".png";
        level4.customers.push_back(cust);
    }
    m_Levels.push_back(level4);

    // 關卡5
    LevelData level5;
    level5.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

    // 先準備 7 個客人的圖檔、位置和請求
    std::vector<std::string> images5 = {
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer1.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer2.png",
        "C:/Users/yello/Shawarma/Resources/Image/Customer/customer3.png",
    };
    std::vector<glm::vec2> positions5 = {
        {100.0f, 85.0f},
        {0.0f, 55.0f},
        {300.0f, 40.0f},
    };
    // 這邊示範前兩位要薯條，後三位要卷餅
    std::vector<std::string> requests5 = {
        "FrenchFries", "FrenchFries",
        "Roll",
    };

    for (int i = 0; i < 3; ++i) {
        CustomerConfig cust;
        cust.customerImage = images5[i];
        cust.position      = positions5[i];
        cust.foodRequest   = requests5[i];
        cust.foodIcon      = "C:/Users/yello/Shawarma/Resources/Image/Food/"
                             + requests5[i] + ".png";
        level5.customers.push_back(cust);
    }
    m_Levels.push_back(level5);
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

size_t LevelManager::GetCurrentLevelIndex() const {
    return m_CurrentLevelIndex;
}

int LevelManager::GetCurrentLevelNumber() const {
    // 把 internal 的 0-based index 轉成玩家習慣看到的第 1、2、3…
    return static_cast<int>(m_CurrentLevelIndex) + 1;
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
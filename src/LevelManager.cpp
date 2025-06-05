#include "LevelManager.hpp"

LevelManager::LevelManager() : m_CurrentLevelIndex(0) {
    LoadLevels();
    // 載入完所有關卡設定後，初始化第一關的 pendingConfigs、索引
    StartLevel();
}
void LevelManager::SetLevelIndex(size_t index) {
    if (index < m_Levels.size()) {
        m_CurrentLevelIndex = index;
        StartLevel();  // 重設 pending customer 列表等
    }
}
LevelManager::~LevelManager() {}

// LevelManager.cpp

#include "LevelManager.hpp"

// 方便生成位置
static glm::vec2 CalcPosition(int index, int perRow = 5, float startX = -200.0f, float startY = 85.0f, float offsetX = 100.0f, float offsetY = -40.0f) {
    int row = index / perRow;
    int col = index % perRow;
    return glm::vec2(startX + col * offsetX, startY + row * offsetY);
}

void LevelManager::LoadLevels() {
    m_Levels.clear();

    // 客製化卷餅配料與圖示對應
    std::vector<std::vector<std::string>> customToppings = {
        {"shaved_meat", "sauce"},                  // roll_no_pickle_fries
        {"shaved_meat", "pickle"},                 // roll_no_sauce_fries
        {"shaved_meat", "sauce", "fries"},         // roll_no_pickle
        {"shaved_meat", "pickle", "fries"},        // roll_no_sauce
        {"shaved_meat", "pickle", "sauce", "fries"}// full_roll
    };
    std::vector<std::string> customIcons = {
        "roll_no_pickle_fries.png",
        "roll_no_sauce_fries.png",
        "roll_no_pickle.png",
        "roll_no_sauce.png",
        "roll.png"
    };

    const int totalLevels = 30;
    for (int lvl = 1; lvl <= totalLevels; ++lvl) {
        LevelData level;
        level.backgroundImage = "C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png";

        // 每關客人數 = 5 + (lvl - 1)
        int custCount = 5 + (lvl - 1);
        for (int i = 0; i < custCount; ++i) {
            CustomerConfig cust;
            // 圖片輪流使用 customer1..customer6
            int imgIdx = i % 6 + 1;
            cust.customerImage = "C:/Users/yello/Shawarma/Resources/Image/Customer/customer" + std::to_string(imgIdx) + ".png";
            cust.position = CalcPosition(i);
            cust.foodRequest = "Roll";  // 預設捲餅，後面再改

            // 1-5 關：只要薯條和捲餅
            if (lvl <= 5) {
                // 前半客人薯條、後半捲餅
                if (i < custCount/2) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
                }
                // 無 requiredToppings
            }
            // 6-10 關：新增耐心值，但行為同 1-5
            else if (lvl < 11) {
                if (i < custCount/2) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
                }
            }
            // 11-15 關：需要補充配料，但無自動 refill，需要 same as above icons
            else if (lvl < 16) {
                if (i < custCount/2) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
                }
            }
            // 16-20 關：新增汽水與可樂訂單
            else if (lvl < 21) {
                int rem = i % 4;
                if (rem == 0) {
                    cust.foodRequest = "Soda";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_soda_full.png";
                } else if (rem == 1) {
                    cust.foodRequest = "Cola";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_cola_full.png";
                } else if (rem == 2) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
                }
            }
            // 21-24 關：新增果汁
            else if (lvl < 25) {
                int rem = i % 5;
                if (rem == 0) {
                    cust.foodRequest = "Juice";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/juice.png";
                } else if (rem == 1) {
                    cust.foodRequest = "Soda";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_soda_full.png";
                } else if (rem == 2) {
                    cust.foodRequest = "Cola";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_cola_full.png";
                } else if (rem == 3) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
                }
            }
            // 25-30 關：客製化卷餅
            else {
                // 均分五種 customToppings
                int typeIdx = i % customToppings.size();
                cust.foodRequest = "Roll";
                cust.requiredToppings = customToppings[typeIdx];
                cust.foodIcon = "C:/Users/yello/Shawarma/Resources/Image/Food/" + customIcons[typeIdx];
            }

            level.customers.push_back(cust);
        }

        m_Levels.push_back(level);
    }
}


// 其餘函式保持不變…

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
int LevelManager::GetTotalLevelCount() const {
    return static_cast<int>(m_Levels.size());
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
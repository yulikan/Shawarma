#include "LevelManager.hpp"
#include <random>    // 新增
static std::random_device rd;
static std::mt19937       gen(rd());

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

        // 預先定義可用的圖片路徑，以方便後面一律隨機挑選
        std::vector<std::string> friesIcon = { "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png" };
        std::vector<std::string> rollIcon  = { "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png"       };
        std::vector<std::string> sodaIcon  = { "C:/Users/yello/Shawarma/Resources/Image/Food/cup_soda_full.png"};
        std::vector<std::string> colaIcon  = { "C:/Users/yello/Shawarma/Resources/Image/Food/cup_cola_full.png"};
        std::vector<std::string> juiceIcon = { "C:/Users/yello/Shawarma/Resources/Image/Food/juice.png"       };

        // --- 針對這一關的每一位客人，隨機產生訂單 ---
        for (int i = 0; i < custCount; ++i) {
            CustomerConfig cust;

            // 圖片依序輪流使用 customer1..customer6
            int imgIdx = i % 6 + 1;
            cust.customerImage = "C:/Users/yello/Shawarma/Resources/Image/Customer/customer"
                                 + std::to_string(imgIdx) + ".png";
            cust.position = CalcPosition(i);

            // 隨機分配訂單（下方分區塊敘述的關卡範圍）
            if (lvl <= 5) {
                // 1-5 關：全部隨機從「薯條、捲餅」二選一
                std::uniform_int_distribution<> dist(0, 1);
                int choice = dist(gen);
                if (choice == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIcon[0];
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIcon[0];
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 11) {
                // 6-10 關：行為同 1-5，但啟用耐心值（由 App 端自行處理）
                std::uniform_int_distribution<> dist(0, 1);
                int choice = dist(gen);
                if (choice == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIcon[0];
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIcon[0];
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 16) {
                // 11-15 關：同樣隨機薯條、捲餅；配料補充機制由外層再出發
                std::uniform_int_distribution<> dist(0, 1);
                int choice = dist(gen);
                if (choice == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIcon[0];
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIcon[0];
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 21) {
                // 16-20 關：新增汽水、可樂，但隨機分配四種：Soda、Cola、FrenchFries、Roll
                std::uniform_int_distribution<> dist(0, 3);
                int choice = dist(gen);
                switch (choice) {
                    case 0:
                        cust.foodRequest = "Soda";
                        cust.foodIcon    = sodaIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 1:
                        cust.foodRequest = "Cola";
                        cust.foodIcon    = colaIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 2:
                        cust.foodRequest = "FrenchFries";
                        cust.foodIcon    = friesIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 3:
                    default:
                        cust.foodRequest = "Roll";
                        cust.foodIcon    = rollIcon[0];
                        cust.requiredToppings.clear();
                        break;
                }
            }
            else if (lvl < 25) {
                // 21-24 關：加入果汁選項，隨機五種：Juice、Soda、Cola、FrenchFries、Roll
                std::uniform_int_distribution<> dist(0, 4);
                int choice = dist(gen);
                switch (choice) {
                    case 0:
                        cust.foodRequest = "Juice";
                        cust.foodIcon    = juiceIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 1:
                        cust.foodRequest = "Soda";
                        cust.foodIcon    = sodaIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 2:
                        cust.foodRequest = "Cola";
                        cust.foodIcon    = colaIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 3:
                        cust.foodRequest = "FrenchFries";
                        cust.foodIcon    = friesIcon[0];
                        cust.requiredToppings.clear();
                        break;
                    case 4:
                    default:
                        cust.foodRequest = "Roll";
                        cust.foodIcon    = rollIcon[0];
                        cust.requiredToppings.clear();
                        break;
                }
            }
            else {
                // 25-30 關：「客製化捲餅」與「其他食物並行」：
                // 我們先決定：這位客人的訂單究竟是「客製化捲餅」還是「一般食物」
                // 例如：讓 60% 機率出捲餅（需依 customToppings），40% 機率出其他種類
                std::uniform_real_distribution<> prob(0.0, 1.0);
                double p = prob(gen);

                if (p < 0.6) {
                    // 60% 機率 → 客製化捲餅
                    std::uniform_int_distribution<> distCt(0, static_cast<int>(customToppings.size()) - 1);
                    int typeIdx = distCt(gen);
                    cust.foodRequest      = "Roll";
                    cust.requiredToppings = customToppings[typeIdx];
                    cust.foodIcon         = "C:/Users/yello/Shawarma/Resources/Image/Food/" + customIcons[typeIdx];
                } else {
                    // 40% 機率 → 從「Soda、Cola、FrenchFries」中隨機挑
                    std::uniform_int_distribution<> distOther(0, 2);
                    int choice = distOther(gen);
                    switch (choice) {
                        case 0:
                            cust.foodRequest = "Soda";
                            cust.foodIcon    = sodaIcon[0];
                            cust.requiredToppings.clear();
                            break;
                        case 1:
                            cust.foodRequest = "Cola";
                            cust.foodIcon    = colaIcon[0];
                            cust.requiredToppings.clear();
                            break;
                        case 2:
                        default:
                            cust.foodRequest = "FrenchFries";
                            cust.foodIcon    = friesIcon[0];
                            cust.requiredToppings.clear();
                            break;
                    }
                }
            }

            // 把這筆客人設定推進本關清單
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
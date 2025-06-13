#include "LevelManager.hpp"
#include <random>    // 新增
static std::random_device rd;
static std::mt19937 gen(rd());

LevelManager::LevelManager() : m_CurrentLevelIndex(0) {
    LoadLevels();
    StartLevel();
}

// 讀關卡數
void LevelManager::SetLevelIndex(size_t index) {
    if (index < m_Levels.size()) {
        m_CurrentLevelIndex = index;
        StartLevel();
    }
}
LevelManager::~LevelManager() {}

static glm::vec2 CalcPosition(int index, int perRow = 5, float startX = -200.0f, float startY = 85.0f, float offsetX = 100.0f, float offsetY = -40.0f) {
    int row = index / perRow;
    int col = index % perRow;
    return glm::vec2(startX + col * offsetX, startY + row * offsetY);
}

void LevelManager::LoadLevels() {
    m_Levels.clear();

    std::vector<std::vector<std::string>> customToppings = {
        {"shaved_meat", "sauce"},
        {"shaved_meat", "pickle"},
        {"shaved_meat", "sauce", "fries"},
        {"shaved_meat", "pickle", "fries"},
        {"shaved_meat", "pickle", "sauce", "fries"}
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
        level.backgroundImage = RESOURCE_DIR "/Image/background/restaurant.png";;

        int custCount = 5 + (lvl - 1);

        std::string friesIconPath = RESOURCE_DIR "/Image/Food/FrenchFries.png";
        std::string rollIconPath  = RESOURCE_DIR "/Image/Food/roll.png";
        std::string sodaIconPath  = RESOURCE_DIR "/Image/Food/cup_soda_full.png";
        std::string colaIconPath  = RESOURCE_DIR "/Image/Food/cup_cola_full.png";
        std::string juiceIconPath = RESOURCE_DIR "/Image/Food/juice.png";

        for (int i = 0; i < custCount; ++i) {
            CustomerConfig cust;

            int imgIdx = (i % 6) + 1;
            cust.customerImage = RESOURCE_DIR "/Image/Customer/customer"
                                 + std::to_string(imgIdx) + ".png";
            cust.position = CalcPosition(i);

            if (lvl <= 5) {
                // 1-5 關：二選一 (Fries、Roll)
                std::uniform_int_distribution<> dist(0, 1);
                if (dist(m_gen) == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIconPath;
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIconPath;
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 11) {
                // 6-10 關：同 1-5 (啟用耐心值)
                std::uniform_int_distribution<> dist(0, 1);
                if (dist(m_gen) == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIconPath;
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIconPath;
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 16) {
                // 11-15 關：同 1-5 (配料補充由 App 端另行處理)
                std::uniform_int_distribution<> dist(0, 1);
                if (dist(m_gen) == 0) {
                    cust.foodRequest = "FrenchFries";
                    cust.foodIcon    = friesIconPath;
                    cust.requiredToppings.clear();
                } else {
                    cust.foodRequest = "Roll";
                    cust.foodIcon    = rollIconPath;
                    cust.requiredToppings.clear();
                }
            }
            else if (lvl < 21) {
                // 16-20 關：四選一 (Soda, Cola, FrenchFries, Roll)
                std::uniform_int_distribution<> dist(0, 3);
                int choice = dist(m_gen);
                switch (choice) {
                    case 0:
                        cust.foodRequest = "Soda";
                        cust.foodIcon    = sodaIconPath;
                        cust.requiredToppings.clear();
                        break;
                    case 1:
                        cust.foodRequest = "Cola";
                        cust.foodIcon    = colaIconPath;
                        cust.requiredToppings.clear();
                        break;
                    case 2:
                        cust.foodRequest = "FrenchFries";
                        cust.foodIcon    = friesIconPath;
                        cust.requiredToppings.clear();
                        break;
                    default:
                        cust.foodRequest = "Roll";
                        cust.foodIcon    = rollIconPath;
                        cust.requiredToppings.clear();
                        break;
                }
            }
            else if (lvl < 25) {
                // 21-24 關：五選一 (Juice, Soda, Cola, FrenchFries, Roll)
                std::uniform_int_distribution<> dist(0, 4);
                int choice = dist(m_gen);
                switch (choice) {
                    case 0:
                        cust.foodRequest = "Juice";
                        cust.foodIcon    = juiceIconPath;
                        cust.requiredToppings.clear();
                        break;
                    case 1:
                        cust.foodRequest = "Soda";
                        cust.foodIcon    = sodaIconPath;
                        cust.requiredToppings.clear();
                        break;
                    case 2:
                        cust.foodRequest = "Cola";
                        cust.foodIcon    = colaIconPath;
                        cust.requiredToppings.clear();
                        break;
                    case 3:
                        cust.foodRequest = "FrenchFries";
                        cust.foodIcon    = friesIconPath;
                        cust.requiredToppings.clear();
                        break;
                    default:
                        cust.foodRequest = "Roll";
                        cust.foodIcon    = rollIconPath;
                        cust.requiredToppings.clear();
                        break;
                }
            }
            else {
                // 25-30 關：「60% → 客製捲餅、40% → 其他三種 (Juice, Soda, Cola, Fries)」
                std::uniform_real_distribution<> probDist(0.0, 1.0);
                double p = probDist(m_gen);

                if (p < 0.6) {
                    // 60%：客製化捲餅
                    std::uniform_int_distribution<> distCt(0, static_cast<int>(customToppings.size()) - 1);
                    int typeIdx = distCt(m_gen);
                    cust.foodRequest      = "Roll";
                    cust.requiredToppings = customToppings[typeIdx];
                    cust.foodIcon         = RESOURCE_DIR "/Image/Food/" + customIcons[typeIdx];
                } else {
                    // 40%：從「Juice、Soda、Cola、FrenchFries」中均等選其一
                    std::uniform_int_distribution<> distOther(0, 3);
                    int choice = distOther(m_gen);
                    switch (choice) {
                        case 0:
                            cust.foodRequest = "Juice";
                            cust.foodIcon    = juiceIconPath;
                            cust.requiredToppings.clear();
                            break;
                        case 1:
                            cust.foodRequest = "Soda";
                            cust.foodIcon    = sodaIconPath;
                            cust.requiredToppings.clear();
                            break;
                        case 2:
                            cust.foodRequest = "Cola";
                            cust.foodIcon    = colaIconPath;
                            cust.requiredToppings.clear();
                            break;
                        default:
                            cust.foodRequest = "FrenchFries";
                            cust.foodIcon    = friesIconPath;
                            cust.requiredToppings.clear();
                            break;
                    }
                }
            }

            level.customers.push_back(cust);
        }

        // 關卡機率調整
        double expectedPerCustomer = 0.0;
        double safetyFactor        = 1.0;

        if (lvl <= 5) {
            // 1-5 關：二選一 (Fries 30, Roll → 無配料的話就只有 baseRollPrice=10)
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"];
            double e_roll  = (0.5) * CalcRollPrice({});
            expectedPerCustomer = e_fries + e_roll;
            safetyFactor = 0.85;
        }
        else if (lvl < 11) {
            // 6-10 關：行為同 1-5
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"];
            double e_roll  = (0.5) * CalcRollPrice({});
            expectedPerCustomer = e_fries + e_roll;
            safetyFactor = 0.85;
        }
        else if (lvl < 16) {
            // 11-15 關：同 1-5（亂數還是 50% Fries、50% Roll_plain）
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"];
            double e_roll  = (0.5) * CalcRollPrice({});
            expectedPerCustomer = e_fries + e_roll;
            safetyFactor = 0.85;
        }
        else if (lvl < 21) {
            // 16-20 關：四選一 (Soda 20, Cola 25, Fries 30, Roll_plain 10) 各 1/4
            double sum = 0.0;
            sum += (1.0/4.0) * m_foodRevenue["Soda"];
            sum += (1.0/4.0) * m_foodRevenue["Cola"];
            sum += (1.0/4.0) * m_foodRevenue["FrenchFries"];
            sum += (1.0/4.0) * CalcRollPrice({});
            expectedPerCustomer = sum;
            safetyFactor = 0.80;
        }
        else if (lvl < 25) {
            // 21-24 關：五選一 (Juice 15, Soda 20, Cola 25, Fries 30, Roll_plain 10) 各 1/5
            double sum = 0.0;
            sum += (1.0/5.0) * m_foodRevenue["Juice"];
            sum += (1.0/5.0) * m_foodRevenue["Soda"];
            sum += (1.0/5.0) * m_foodRevenue["Cola"];
            sum += (1.0/5.0) * m_foodRevenue["FrenchFries"];
            sum += (1.0/5.0) * CalcRollPrice({});
            expectedPerCustomer = sum;
            safetyFactor = 0.80;
        }
        else {
            // 25-30 關：「60% → 客製捲餅、40% → 三種其他 (Juice 15, Soda 20, Cola 25, Fries 30)」
            double probRoll = 0.6;
            double avgRollPrice = 0.0;
            {
                // 售價加總
                double sumRollPrices = 0.0;
                for (auto &tlist : customToppings) {
                    sumRollPrices += CalcRollPrice(tlist);
                }
                avgRollPrice = sumRollPrices / static_cast<double>(customToppings.size());
            }
            double e_roll = probRoll * avgRollPrice;
            double probOtherBlock = 0.4;
            double avgOther = 0.0;
            {
                avgOther += (1.0/4.0) * m_foodRevenue["Juice"];
                avgOther += (1.0/4.0) * m_foodRevenue["Soda"];
                avgOther += (1.0/4.0) * m_foodRevenue["Cola"];
                avgOther += (1.0/4.0) * m_foodRevenue["FrenchFries"];
            }
            double e_other = probOtherBlock * avgOther;

            expectedPerCustomer = e_roll + e_other;
            safetyFactor = 0.75;
        }

        double expectedTotal = expectedPerCustomer * static_cast<double>(custCount);
        int threshold = static_cast<int>(std::ceil(expectedTotal * safetyFactor));

        level.passThreshold = threshold;
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
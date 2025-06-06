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

        // 各類食物的 icon 路徑（同你現有程式）
        std::string friesIconPath = "C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png";
        std::string rollIconPath  = "C:/Users/yello/Shawarma/Resources/Image/Food/roll.png";
        std::string sodaIconPath  = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_soda_full.png";
        std::string colaIconPath  = "C:/Users/yello/Shawarma/Resources/Image/Food/cup_cola_full.png";
        std::string juiceIconPath = "C:/Users/yello/Shawarma/Resources/Image/Food/juice.png";

        // --------- 1) 先把本關所有客人的訂單隨機產生 ---------
        for (int i = 0; i < custCount; ++i) {
            CustomerConfig cust;

            // 客人頭像輪流
            int imgIdx = (i % 6) + 1;
            cust.customerImage = "C:/Users/yello/Shawarma/Resources/Image/Customer/customer"
                                 + std::to_string(imgIdx) + ".png";
            cust.position = CalcPosition(i);

            // 隨機分配訂單 (依照 lvl 範圍)
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
                // 6-10 關：同 1-5 (啟用耐心值，不影響此處隨機)
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
                // 11-15 關：同 1-5 (配料補充機制由 App 端另行處理)
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
                    cust.foodIcon         = "C:/Users/yello/Shawarma/Resources/Image/Food/" + customIcons[typeIdx];
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

        // --------- 2) 本關「期望值計算」：預估每位客人的平均收益 ---------
        double expectedPerCustomer = 0.0;
        double safetyFactor        = 1.0; // 隨關卡難度調整

        if (lvl <= 5) {
            // 1-5 關：二選一 (Fries 30, Roll → 無配料的話就只有 baseRollPrice=10)
            // P( Fries ) = 0.5, P( Roll_plain ) = 0.5
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"]; // 0.5 * 30
            double e_roll  = (0.5) * CalcRollPrice({});            // 0.5 * 10
            expectedPerCustomer = e_fries + e_roll;                // = 15 + 5 = 20
            safetyFactor = 0.85;
        }
        else if (lvl < 11) {
            // 6-10 關：行為同 1-5
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"];
            double e_roll  = (0.5) * CalcRollPrice({});
            expectedPerCustomer = e_fries + e_roll;                // = 20
            safetyFactor = 0.85;
        }
        else if (lvl < 16) {
            // 11-15 關：同 1-5（亂數還是 50% Fries、50% Roll_plain）
            double e_fries = (0.5) * m_foodRevenue["FrenchFries"];
            double e_roll  = (0.5) * CalcRollPrice({});
            expectedPerCustomer = e_fries + e_roll;                // = 20
            safetyFactor = 0.85;
        }
        else if (lvl < 21) {
            // 16-20 關：四選一 (Soda 20, Cola 25, Fries 30, Roll_plain 10) 各 1/4
            double sum = 0.0;
            sum += (1.0/4.0) * m_foodRevenue["Soda"];          // 0.25 * 20 = 5
            sum += (1.0/4.0) * m_foodRevenue["Cola"];          // 0.25 * 25 = 6.25
            sum += (1.0/4.0) * m_foodRevenue["FrenchFries"];   // 0.25 * 30 = 7.5
            sum += (1.0/4.0) * CalcRollPrice({});              // 0.25 * 10 = 2.5
            expectedPerCustomer = sum;                         // = 21.25
            safetyFactor = 0.80;
        }
        else if (lvl < 25) {
            // 21-24 關：五選一 (Juice 15, Soda 20, Cola 25, Fries 30, Roll_plain 10) 各 1/5
            double sum = 0.0;
            sum += (1.0/5.0) * m_foodRevenue["Juice"];         // 0.2 * 15  = 3
            sum += (1.0/5.0) * m_foodRevenue["Soda"];          // 0.2 * 20  = 4
            sum += (1.0/5.0) * m_foodRevenue["Cola"];          // 0.2 * 25  = 5
            sum += (1.0/5.0) * m_foodRevenue["FrenchFries"];   // 0.2 * 30  = 6
            sum += (1.0/5.0) * CalcRollPrice({});              // 0.2 * 10  = 2
            expectedPerCustomer = sum;                         // = 20
            safetyFactor = 0.80;
        }
        else {
            // 25-30 關：「60% → 客製捲餅、40% → 三種其他 (Juice 15, Soda 20, Cola 25, Fries 30)」
            // 先算「捲餅部分」的期望：P=0.6，每種捲餅價格要看各 toppings（無法用單一值）
            // 但我們可以計算「客製捲餅的平均售價」：把 five 種 customToppings 都列進去求期望
            double probRoll = 0.6;
            double avgRollPrice = 0.0;
            {
                // 先把每一種 customToppings 的售價都加總
                double sumRollPrices = 0.0;
                for (auto &tlist : customToppings) {
                    sumRollPrices += CalcRollPrice(tlist);
                }
                // 這裡共有 5 種 toppings 組合
                avgRollPrice = sumRollPrices / static_cast<double>(customToppings.size());
                // 舉例：假設五種組合的價錢分別是 [ base+配料1, base+配料2, … ]，
                //      平均完大概會落在 10~50 之間某值，這裡用實際迴圈算值。
            }
            double e_roll = probRoll * avgRollPrice;

            // 再算「其他 40%」裡面的期望：先 0.4，再均分四種(若你想只三種就調整)
            // 但我們前面做的是「四種」(Juice,Soda,Cola,Fries)各 1/4，乘上 0.4
            double probOtherBlock = 0.4;
            double avgOther = 0.0;
            {
                // 五選一變成「四選一」，因為此處我們把「Roll」從選項剃除
                avgOther += (1.0/4.0) * m_foodRevenue["Juice"];       // = 3.75
                avgOther += (1.0/4.0) * m_foodRevenue["Soda"];        // = 5
                avgOther += (1.0/4.0) * m_foodRevenue["Cola"];        // = 6.25
                avgOther += (1.0/4.0) * m_foodRevenue["FrenchFries"]; // = 7.5
                // 以上 sum = 22.5
            }
            double e_other = probOtherBlock * avgOther; // = 0.4 * 22.5 = 9

            expectedPerCustomer = e_roll + e_other;
            // e_roll = 0.6 * (所有 5 種捲餅價格平均)
            // e_other = 9
            safetyFactor = 0.75;
        }

        // --------- 3) 計算本關「預期總收益」並套用安全係數，取整數 ---------
        double expectedTotal = expectedPerCustomer * static_cast<double>(custCount);
        int threshold = static_cast<int>(std::ceil(expectedTotal * safetyFactor));

        level.passThreshold = threshold;

        // 最後把這關推進 m_Levels
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
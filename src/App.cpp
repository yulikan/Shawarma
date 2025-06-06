#include "App.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <algorithm> // for std::find
#include "Roll.hpp"
#include "NextButton.hpp"
#include "PatienceText.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_Renderer = std::make_shared<Util::Renderer>();

    m_IgnoreNextMouseUp = false;

    // Initial UI & objects
    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ShopButton  = std::make_shared<ShopButton>();
    m_ReturnButton = std::make_shared<ReturnButton>();
    m_Meat        = std::make_shared<Meat>();
    m_Crust       = std::make_shared<Crust>();
    m_Knife       = std::make_shared<Knife>();
    m_Paper       = std::make_shared<Paper>();
    m_CurrentLevelPage = "LevelPage1";

    // Level complete screen
    m_LevelCompleteScreen = std::make_shared<Util::GameObject>(
        std::make_unique<Util::Image>(
            "C:/Users/yello/Shawarma/Resources/Image/background/nextLevel.png"),
        /*layer=*/7
    );
    m_LevelCompleteScreen->m_Transform.translation = glm::vec2(0.0f, 0.0f);
    m_LevelCompleteScreen->m_Transform.scale       = glm::vec2(0.7f, 0.7f);
    m_NextButton = std::make_shared<NextButton>();
    m_PoorMan = std::make_shared<PoorMan>();

    // Toppings & potato
    m_Fries       = std::make_shared<Fries>();
    m_Pickle      = std::make_shared<Pickle>();
    m_Sauce       = std::make_shared<Sauce>();
    m_ShavedMeat  = std::make_shared<ShavedMeat>();
    m_Potato      = std::make_shared<Potato>();

    // Frying counter text
    m_FryingCounterText = std::make_shared<FryingCounterText>();

    // Money text
    m_MoneyManager = MoneyManager(0);
    auto moneyTextDrawable = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/msyh.ttc", 30, "$0", Util::Color{255,255,255}
    );
    m_MoneyTextGO = std::make_shared<Util::GameObject>(moneyTextDrawable, 5);
    m_MoneyTextGO->m_Transform.translation = glm::vec2(460.0f, 325.0f);
    m_MoneyText = moneyTextDrawable;
    m_MoneyManager.SetOnChangeCallback([this](int newBal){
        m_MoneyText->SetText("$" + std::to_string(newBal));
    });
    // 1) 建立 Util::Text
    m_LevelText = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/msyh.ttc",  // 字體
        48,                           // 字號
        "00",                           // 初始文字留空
        Util::Color{0,0,0}        // 想要醒目一點就用黃色
    );
    // 2) 包成 GameObject，放在關卡畫面之上 (layer 8)
    m_LevelTextGO = std::make_shared<Util::GameObject>(m_LevelText, 8);
    // 設定文字位置（自行調整）
    m_LevelTextGO->m_Transform.translation = glm::vec2(-5.0f, -45.0f);
    // App::Start() 中初始化文字
    m_DayText = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/arial.ttf", 26,  // 字型、字號
        "1",                            // 初始文字
        Util::Color::FromName(Util::Colors::BLACK)
    );
    m_DayTextGO = std::make_shared<Util::GameObject>(m_DayText, 8);
    m_DayTextGO->m_Transform.translation = glm::vec2(-575.0f, 275.0f);  // 左上角位置

    m_CompleteMoneyText = std::make_shared<Util::Text>(
    "C:/Windows/Fonts/msyh.ttc",   // 字型
    48,                             // 字號
    "0",                            // 預設不能用 ""，改成 "0" 或 " "，避免 zero width
    Util::Color{0,0,0}              // 跟 LevelText 同色
);
    m_CompleteMoneyTextGO = std::make_shared<Util::GameObject>(m_CompleteMoneyText, 8);
    m_CompleteMoneyTextGO->m_Transform.translation = glm::vec2(
        m_LevelTextGO->m_Transform.translation.x + 50.0f,
        m_LevelTextGO->m_Transform.translation.y + 90.0f
    );
    // Add initial children
    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);

}

void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit())
        m_CurrentState = State::END;

    // Level complete phase
    if (m_CurrentPhase == phase::levelComplete) {
        // 1. 移除「第 N 天」的 DayText
        m_Renderer->RemoveChild(m_DayTextGO);

        // 2. 顯示「第 N 關」文字
        int levelNum = static_cast<int>(m_LevelManager.GetCurrentLevelIndex()) + 1;
        m_LevelText->SetText(std::to_string(levelNum));
        m_Renderer->AddChild(m_LevelTextGO);

        // 3. 顯示「結算金額」文字（先更新成非空字串）
        int currentBalance = m_MoneyManager.GetBalance();
        m_CompleteMoneyText->SetText("$" + std::to_string(currentBalance));
        m_Renderer->AddChild(m_CompleteMoneyTextGO);

        // 4. 如果點擊 NextButton，就把這些結算畫面物件都移除，進入下一關
        if (m_NextButton->IsClicked()) {
            m_Renderer->RemoveChild(m_LevelTextGO);
            m_Renderer->RemoveChild(m_CompleteMoneyTextGO);
            m_Renderer->RemoveChild(m_LevelCompleteScreen);
            m_Renderer->RemoveChild(m_NextButton);

            m_LevelManager.NextLevel();
            LoadLevel(m_LevelManager.GetCurrentLevel());
            m_CurrentPhase = phase::phase2;
        }

        m_Renderer->Update();
        return;
    }


    // Phase 2: check finished orders
    if (m_CurrentPhase == phase::phase2
         && m_LevelManager.IsLevelFinished()
         && m_Customers.empty())
    {
        // 依照金額判斷過關
        if (m_MoneyManager.GetBalance() >= 100) {
            // 過關：顯示結算畫面
            m_Renderer->AddChild(m_LevelCompleteScreen);
            m_Renderer->AddChild(m_NextButton);
            m_CurrentPhase = phase::levelComplete;
        }
        else {
            // 失敗：顯示失敗畫面與重試按鈕
            m_Renderer->RemoveChild(m_DayTextGO);
            m_FailureScreen = std::make_shared<Util::GameObject>(
                std::make_unique<Util::Image>(
                    "C:/Users/yello/Shawarma/Resources/Image/background/FailPage.png"
                ), 7
            );
            m_FailureScreen->m_Transform.translation = {0,0};
            m_FailureScreen->m_Transform.scale       = {0.7f,0.7f};
            m_Renderer->AddChild(m_FailureScreen);

            m_RetryButton = std::make_shared<NextButton>(
                "C:/Users/yello/Shawarma/Resources/Image/Object/retryBtn.png"
            );
            m_Renderer->AddChild(m_RetryButton);

            m_CurrentPhase = phase::failed;
        }
        return;
    }
    // Start game
    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LoadLevel(m_LevelManager.GetCurrentLevel());
    }
if (m_CurrentPhase == phase::phase3) {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    if (m_IgnoreNextMouseUp) {
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            // 放開事件已經被消耗，標記為 false，下次才會真正判斷選關
            m_IgnoreNextMouseUp = false;
        }
        // 無論是否真的檢測到放開，都跳出，不繼續檢查選關
        return;
    }
    // 按 N 換到 LevelPage2（只能從 LevelPage1 進）
    if (Util::Input::IsKeyUp(Util::Keycode::N) && m_CurrentLevelPage == "LevelPage1") {
        m_CurrentLevelPage = "LevelPage2";
        m_Background = std::make_shared<BackgroundImage>(
            "C:/Users/yello/Shawarma/Resources/Image/background/LevelPage2.png");
        m_Renderer = std::make_shared<Util::Renderer>(
            std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }

    // 按 P 回到 LevelPage1（只能從 LevelPage2 回）
    if (Util::Input::IsKeyUp(Util::Keycode::P) && m_CurrentLevelPage == "LevelPage2") {
        m_CurrentLevelPage = "LevelPage1";
        m_Background = std::make_shared<BackgroundImage>(
            "C:/Users/yello/Shawarma/Resources/Image/background/LevelPage1.png");
        m_Renderer = std::make_shared<Util::Renderer>(
            std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }

    // 滑鼠左鍵點擊關卡按鈕（只有在 LevelPage1 時才有反應）
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (m_CurrentLevelPage == "LevelPage1") {
            glm::vec2 levelSize(230.0f, 200.0f);
            std::vector<glm::vec2> levelCenters = {
                {-500.0f, 235.0f}, {-250.0f, 235.0f}, {0.0f, 235.0f},
{250.0f, 235.0f},  {500.0f, 235.0f},{-500.0f, 0.0f}, {-250.0f, 0.0f}, {0.0f, 0.0f},
{250.0f, 0.0f},  {500.0f, 0.0f},{-500.0f, -235.0f}, {-250.0f, -235.0f}, {0.0f, -235.0f},
{250.0f, -235.0f},  {500.0f, -235.0f},
            };

            for (size_t i = 0; i < levelCenters.size(); ++i) {
                glm::vec2 min = levelCenters[i] - levelSize * 0.5f;
                glm::vec2 max = levelCenters[i] + levelSize * 0.5f;

                if (mousePos.x >= min.x && mousePos.x <= max.x &&
                    mousePos.y >= min.y && mousePos.y <= max.y)
                {
                    size_t levelIndex = i;  // 第1～5關
                    if (levelIndex >= m_LevelManager.GetTotalLevelCount()) {
                        std::cout << "尚未實作關卡 index = " << levelIndex << std::endl;
                        continue;
                    }
                    m_LevelManager.SetLevelIndex(levelIndex);
                    LoadLevel(m_LevelManager.GetCurrentLevel());
                    m_CurrentPhase = phase::phase2;
                    return;
                }
            }
        }

        if (m_CurrentLevelPage == "LevelPage2") {
            glm::vec2 levelSize(230.0f, 200.0f);
            std::vector<glm::vec2> levelCenters = {
                {-500.0f, 235.0f}, {-250.0f, 235.0f}, {0.0f, 235.0f},
{250.0f, 235.0f},  {500.0f, 235.0f},{-500.0f, 0.0f}, {-250.0f, 0.0f}, {0.0f, 0.0f},
{250.0f, 0.0f},  {500.0f, 0.0f},{-500.0f, -235.0f}, {-250.0f, -235.0f}, {0.0f, -235.0f},
{250.0f, -235.0f},  {500.0f, -235.0f},
            };

            for (size_t i = 0; i < levelCenters.size(); ++i) {
                glm::vec2 min = levelCenters[i] - levelSize * 0.5f;
                glm::vec2 max = levelCenters[i] + levelSize * 0.5f;

                if (mousePos.x >= min.x && mousePos.x <= max.x &&
                    mousePos.y >= min.y && mousePos.y <= max.y)
                {
                    size_t levelIndex = i+15;  // 第16～30關
                    if (levelIndex >= m_LevelManager.GetTotalLevelCount()) {
                        std::cout << "尚未實作關卡 index = " << levelIndex << std::endl;
                        continue;
                    }
                    m_LevelManager.SetLevelIndex(levelIndex);
                    LoadLevel(m_LevelManager.GetCurrentLevel());
                    m_CurrentPhase = phase::phase2;
                    return;
                }
            }
        }
    }

    if (m_Renderer) m_Renderer->Update();  // 持續畫面
    return;
}

    // Phase 2 main logic
    if (m_CurrentPhase == phase::phase2) {
    // ---------------------------------------------------------
    // 1. 先更新耐心（若启用），并做逾时移除
    // ---------------------------------------------------------
    for (auto it = m_Customers.begin(); it != m_Customers.end();) {
        auto& customer = *it;

        // —— 1A. 更新耐心并处理逾时离开 ——
        if (m_EnablePatience) {
            if (auto pt = customer->GetPatienceText()) {
                pt->Update();
                if (pt->GetRemaining() <= 0) {
                    // 顾客耐心耗尽，移除 PatienceText、订单图示、顾客本体
                    m_Renderer->RemoveChild(pt);
                    customer->SetPatienceText(nullptr);
                    if (customer->GetOrderIcon())
                        m_Renderer->RemoveChild(customer->GetOrderIcon());
                    m_Renderer->RemoveChild(customer);

                    it = m_Customers.erase(it);
                    continue;
                }
            }
        }

        // —— 1B. 不论是否启用耐心，都要检查“是否吃完” ——
        const auto& eaten = customer->GetEatenFoods();
        const auto& requested = customer->GetRequestedFood();
        if (std::find(eaten.begin(), eaten.end(), requested) != eaten.end()) {
            // 顾客吃完，结算金钱
            if (requested == "FrenchFries") m_MoneyManager.Add(30);
            else if (requested == "sauce")       m_MoneyManager.Add(10);
            else if (requested == "Juice")       m_MoneyManager.Add(15);
            // …（根据实际的 RequestedFood 类型再补一行）

            // 移除 PatienceText（若存在）
            if (auto pt2 = customer->GetPatienceText()) {
                m_Renderer->RemoveChild(pt2);
                customer->SetPatienceText(nullptr);
            }
            // 移除订单图示（若存在）
            if (customer->GetOrderIcon())
                m_Renderer->RemoveChild(customer->GetOrderIcon());
            // 移除顾客本体
            m_Renderer->RemoveChild(customer);

            it = m_Customers.erase(it);
            continue;
        }

        // —— 如果既没逾时也没吃完，就推进迭代器 ——
        ++it;
    }



    // ---------------------------------------------------------
    // 2. 再來根據「剩下的客人數」去 Spawn 新客人，並把位置、Icon、文字統一以
    //    customer->m_Transform.translation 為基準
    // ---------------------------------------------------------

        std::array<bool, 3> occupied = { false, false, false };
    for (auto& customer : m_Customers) {
        glm::vec2 pos = customer->m_Transform.translation;
        // 與 kCustomerSlots 做「完全相等」比對即可，
        // 因為我們一開始就是把 customer->m_Transform.translation 設成 kCustomerSlots[...]。
        for (size_t i = 0; i < kCustomerSlots.size(); ++i) {
            if (pos == kCustomerSlots[i]) {
                occupied[i] = true;
                break;
            }
        }
    }

    // ---------------------------------------------------------
    // 3. 根據「還剩下幾個空位」，去 Spawn 新客人，把他們補到第一個 free 槽位
    // ---------------------------------------------------------
    // 注意：SpawnCustomers 需要傳入「目前活躍的客人數量」，因為它會決定要生成幾位新客人。
    // 此處傳 (int)m_Customers.size() 就代表「真正留在畫面上的客人有幾位」，跟執行 Remove 之後的大小一致。

    auto newCfgs = m_LevelManager.SpawnCustomers((int)m_Customers.size());
    for (auto& cfg : newCfgs) {
        // 3A. 找出「第一個未被佔用」的槽位 index
        size_t slotIndex = 0;
        while (slotIndex < occupied.size() && occupied[slotIndex]) {
            ++slotIndex;
        }
        if (slotIndex >= occupied.size()) {
            // 理論上不會跑到這裡，因為 SpawnCustomers 已經保證「最多 spawn 的數量 <= 空位數量」。
            slotIndex = occupied.size() - 1;
        }
        // 把這個槽位標記為已佔用
        occupied[slotIndex] = true;

        // 3B. 建立新客人，並把位置設成 kCustomerSlots[slotIndex]
        auto customer = std::make_shared<Customer>(cfg.customerImage);
        customer->m_Transform.translation = kCustomerSlots[slotIndex];
        customer->m_Transform.scale       = glm::vec2(0.5f, 0.5f);
        customer->SetRequestedFood(cfg.foodRequest);

        // 3C. 以 customer 位置為基準，建立 FoodIcon 與 PatienceText
        glm::vec2 custPos = customer->m_Transform.translation;

        // Food Icon
        auto foodIcon = std::make_shared<Util::GameObject>(
            std::make_unique<Util::Image>(cfg.foodIcon), 5);
        if (cfg.foodRequest == "Roll") {
            if (m_EnableCustomTopping) {
                customer->SetRequiredToppings(cfg.requiredToppings);
            } else {
                customer->SetRequiredToppings({});
            }
            foodIcon->m_Transform.scale       = glm::vec2(0.08f, 0.08f);
            foodIcon->m_Transform.translation = custPos + glm::vec2(0.0f, -55.0f);
        } else {
            foodIcon->m_Transform.scale       = glm::vec2(0.3f, 0.3f);
            foodIcon->m_Transform.translation = custPos + glm::vec2(0.0f, -60.0f);
        }
        customer->SetOrderIcon(foodIcon);

        // Patience Text
        auto patienceText = std::make_shared<PatienceText>();
        patienceText->SetPatience(30);
        patienceText->m_Transform.translation = custPos + glm::vec2(0.0f, -100.0f);
        customer->SetPatienceText(patienceText);

        // 3D. 把客人、FoodIcon、PatienceText 加到場景中

        m_Customers.push_back(customer);
        m_Renderer->AddChild(customer);
        m_Renderer->AddChild(foodIcon);
        if (m_EnablePatience) {
            m_Renderer->AddChild(patienceText);
        }
    }
        // Fries button
        if (!m_Fries->IsPlaced() && m_Fries->IsClicked()) {
            m_Fries->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_fries.png",
                "fries"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -214.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Sauce button
        if (!m_Sauce->IsPlaced() && m_Sauce->IsClicked()) {
            m_Sauce->SetPlaced(true);
            m_Sauce->DecreaseCount();
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_sauce.png",
                "sauce"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -248.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Pickle button
        if (!m_Pickle->IsPlaced() && m_Pickle->IsClicked()) {
            m_Pickle->SetPlaced(true);
            m_Pickle->DecreaseCount();  // 扣除一次
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_pickle.png",
                "pickle"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -195.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // --- 黃瓜補充 ---
        if (m_CucumberHand && m_EnableIngredientLimit && m_Pickle) {
            if (!m_IsPickleHandPressed && m_CucumberHand->IsClicked()) {
                m_IsPickleHandPressed = true;
            }
            if (m_IsPickleHandPressed && !m_CucumberHand->IsClicked() &&
                Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
                m_IsPickleHandPressed = false;
                m_Pickle->IncreaseCount();
                }
        }

        // --- 醬料補充 ---
        if (m_SauceHand && m_EnableIngredientLimit && m_Sauce) {
            if (!m_IsSauceHandPressed && m_SauceHand->IsClicked()) {
                m_IsSauceHandPressed = true;
            }
            if (m_IsSauceHandPressed && !m_SauceHand->IsClicked() &&
                Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
                m_IsSauceHandPressed = false;
                m_Sauce->IncreaseCount();
                }
        }


        // ShavedMeat button
        if (!m_ShavedMeat->IsPlaced() && m_ShavedMeat->IsClicked()) {
            m_ShavedMeat->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_meat.png",
                "shaved_meat"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -232.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Potato & frying logic
        if (!m_Potato->IsPlaced() && m_Potato->IsClicked()) {
            m_Potato->SetPlaced(true);
            m_Frying = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/frying.png",
                "potato"
            );
            m_Frying->m_Transform.translation = glm::vec2(480.0f, -40.0f);
            m_Renderer->AddChild(m_Frying);
            m_Pstate = 0;
        }
        if (m_Pstate == 0 && m_Frying) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) &&
                mousePos.x >= 400 && mousePos.x <= 550 &&
                mousePos.y >= -100 && mousePos.y <= 0
            ) {
                m_Pstate = 1;
            }
            if (m_Pstate == 1) {
                m_Pstate = 0;
                m_FryingCounter++;
                if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(m_FryingCounter);
                std::cout << "Frying topping clicked, count: " << m_FryingCounter << std::endl;
            }
        }
// —— 饮料杯子逻辑 （App.cpp）——

// 先更新所有杯子的状态
for (auto& cup : m_Cups)
    cup->Update();

// —— 1) 服务顾客：合并“点击FULL杯子”+“给客人喝”的逻辑，只做一次检测 ——
//    如果 cup 是 FULL、玩家点击了鼠标、并且cup在顾客的交互范围内，则视为“给顾客喝”。
//    同时只在 cup->GetType() 与 cust->GetRequestedFood() 相符时加钱，并把 FULL 杯替换成 EMPTY。
bool served = false;
for (auto& cust : m_Customers) {
    glm::vec2 custPos = cust->m_Transform.translation;

    for (size_t i = 0; i < m_Cups.size(); ++i) {
        auto& cup = m_Cups[i];

        // 只有 FULL 杯子才有可能“给顾客喝”
        if (cup->GetState() == CupState::FULL
            && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
        {
            float dist = glm::distance(custPos, cup->m_Transform.translation);
            float interactRadius = 50.0f * cup->m_Transform.scale.x;

            if (dist < interactRadius) {
                // —— 1.1) 只有当 “杯子类型 == 顾客要求” 时，才实际加钱并记录
                std::string req = cust->GetRequestedFood();  // 拿到顾客要求的字符串
                if ((cup->GetType() == DrinkType::SODA && req == "Soda") ||
                    (cup->GetType() == DrinkType::COLA && req == "Cola"))
                {
                    if (cup->GetType() == DrinkType::SODA) {
                        m_MoneyManager.Add(20);
                        cust->RecordFood("Soda");
                    }
                    else { // DrinkType::COLA
                        m_MoneyManager.Add(25);
                        cust->RecordFood("Cola");
                    }
                }
                // 如果“配错”，就不加钱，也不调用 RecordFood()

                // —— 1.2) 删掉这个 FULL 杯，补一个 EMPTY 杯
                DrinkType servedType = cup->GetType();
                m_Renderer->RemoveChild(cup);
                m_Cups.erase(m_Cups.begin() + i);

                glm::vec2 offset = (servedType == DrinkType::SODA
                    ? glm::vec2(-50, -50)
                    : glm::vec2( 50, -50));
                auto emptyCup = std::make_shared<Cup>(
                    servedType,
                    m_BevMachine->GetPosition() + offset * m_BevMachine->GetScale(),
                    CupState::EMPTY);
                m_Cups.push_back(emptyCup);
                m_Renderer->AddChild(emptyCup);

                served = true;
                break;  // 这个顾客已经被服务，跳出 cup 的循环
            }
        }
    }

    if (served) {
        // 如果有顾客已经在这一帧被服务，就不用再继续给下一个顾客加钱了
        served = false;
        break;
    }
}


// —— 2) 补充按钮逻辑：保持与之前相同 ——
//    按下 SODA 按钮时，找到第一个 EMPTY 的 SODA 杯并 Refill。
//    按下 COLA 按钮时，找到第一个 EMPTY 的 COLA 杯并 Refill。
if (m_BevMachine->IsButtonClicked(DrinkType::SODA)) {
    for (auto& cup : m_Cups) {
        if (cup->GetType() == DrinkType::SODA
         && cup->GetState() == CupState::EMPTY)
        {
            cup->Refill();
            cup->SetPosition(
                m_BevMachine->GetPosition()
                + glm::vec2(-50, -50) * m_BevMachine->GetScale());
            break;  // 只补一杯
        }
    }
}
if (m_BevMachine->IsButtonClicked(DrinkType::COLA)) {
    for (auto& cup : m_Cups) {
        if (cup->GetType() == DrinkType::COLA
         && cup->GetState() == CupState::EMPTY)
        {
            cup->Refill();
            cup->SetPosition(
                m_BevMachine->GetPosition()
                + glm::vec2(50, -50) * m_BevMachine->GetScale());
            break;  // 只补一杯
        }
    }
}

// …（後續邏輯）…



        // Customer <> FrenchFries interaction
        for (auto& customer : m_Customers) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end();) {
                auto& friesObj = *it;
                if (customer->IsNearFrenchFries(*friesObj)) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        customer->GetEatState() == Customer::EatState::READY_TO_EAT
                    ) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        customer->RecordFood("FrenchFries");
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (customer->GetEatState() != Customer::EatState::EATEN)
                        customer->SetEatState(Customer::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        // Refill fries when counter hits 5
        if (m_FryingCounter == 5) {
            auto newFries = std::make_shared<FrenchFries>();
            newFries->m_Transform.translation = glm::vec2(100.0f + m_FrenchFriesList.size()*60, -50.0f);
            m_FrenchFriesList.push_back(newFries);
            m_Renderer->AddChild(newFries);
            if (m_Frying) {
                m_Renderer->RemoveChild(m_Frying);
                m_Frying.reset();
            }
            m_FryingCounter = 0;
            if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(0);
            m_Potato->SetPlaced(false);
            m_Pstate = 2;
        }
        if (Util::Input::IsKeyUp(Util::Keycode::R)) {
            // 檢查目前畫面上的 roll 數量是否小於 3
            if (m_Rolls.size() < 3) {
                // 宣告 rollContents，用來儲存烤餅上的配料
                std::vector<std::string> rollContents;
                std::cout << "Roll contents:" << std::endl;
                for (auto& topping : toppings) {
                    std::string type = topping->GetType();
                    rollContents.push_back(type);
                    std::cout << type << std::endl;
                    // 從畫面上移除該配料
                    m_Renderer->RemoveChild(topping);
                }
                toppings.clear();  // 清空配料列表

                // 重置所有配料按鈕狀態
                m_Fries->SetPlaced(false);
                m_Sauce->SetPlaced(false);
                m_Pickle->SetPlaced(false);
                m_ShavedMeat->SetPlaced(false);
                m_CurrentDay = m_LevelManager.GetCurrentLevelNumber();
                if (m_DayText) {
                    m_DayText->SetText(std::to_string(m_CurrentDay));
                }
                m_Renderer->AddChild(m_DayTextGO);
                // 建立新的 Roll 物件
                auto roll = std::make_shared<Roll>(rollContents);

                // 調整 roll 的位置
                float baseX   = 145.0f;
                float baseY   = -130.0f;
                float offsetY = -40.0f * static_cast<float>(m_Rolls.size());
                roll->m_Transform.translation = glm::vec2(baseX, baseY + offsetY);

                // 加入清單與 Renderer
                m_Rolls.push_back(roll);
                m_Renderer->AddChild(roll);
            } else {
                std::cout << "no more than 3 roll" << std::endl;
            }

        }

        for (auto& customer : m_Customers) {
            if (customer->GetEatState() == Customer::EatState::READY_TO_EAT &&
                Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
            {
                for (auto it = m_Rolls.begin(); it != m_Rolls.end(); ++it) {
                    auto& rollObj = *it;
                    float distance = glm::distance(
                        customer->m_Transform.translation,
                        rollObj->m_Transform.translation
                    );
                    if (distance < 50.0f) {
                        // —— 找到一顆可以吃的 Roll ——

                        // 1. 先檢查這位顧客的 “RequestedFood” 是不是 "Roll"。
                        //    如果是，就用「配料數量」來算價錢：空餅皮 10、每個配料 +10，最高 50。
                        if (customer->GetRequestedFood() == "Roll") {
                            auto actual = rollObj->GetContents();
                            int toppingCount = static_cast<int>(actual.size());
                            int price = 10 + toppingCount * 10;
                            if (price > 50) price = 50;
                            m_MoneyManager.Add(price);

                            customer->RecordFood("Roll");
                            for (auto& ing : actual) {
                                customer->RecordFood(ing);
                            }
                        }
                        // 2. 更新顧客狀態為 EATEN
                        customer->SetEatState(Customer::EatState::EATEN);

                        // 3. 從場景移除這顆卷餅
                        m_Renderer->RemoveChild(rollObj);
                        m_Rolls.erase(it);
                        g_IsObjectDragging = false;

                        // 已吃完，結束本輪
                        break;
                    }
                }
            }

            // 如果距離 < 50 但鼠標未松開，設為 READY_TO_EAT
            for (auto& rollObj : m_Rolls) {
                float dist2 = glm::distance(
                    customer->m_Transform.translation,
                    rollObj->m_Transform.translation
                );
                if (dist2 < 50.0f) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    break;
                }
            }
        }

        // PoorMan <> Roll interaction
        if (m_PoorMan) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end();) {
                auto& rollObj = *it;
                float distance = glm::distance(m_PoorMan->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT
                    ) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN)
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        // PoorMan <> FrenchFries interaction
        if (m_PoorMan) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end();) {
                auto& friesObj = *it;
                if (m_PoorMan->IsNearFrenchFries(*friesObj)) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT
                    ) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN)
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        // 點擊 juicepack 生成 juice（場上只能一個）
        if (!m_Juice && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            float w = 300.0f * m_JuicePack->m_Transform.scale.x;
            float h = 300.0f * m_JuicePack->m_Transform.scale.y;
            glm::vec2 min = m_JuicePack->m_Transform.translation - glm::vec2(w/2, h/2);
            glm::vec2 max = m_JuicePack->m_Transform.translation + glm::vec2(w/2, h/2);
            if (mousePos.x >= min.x && mousePos.x <= max.x &&
                mousePos.y >= min.y && mousePos.y <= max.y) {
                m_Juice = std::make_shared<Juice>();
                m_Renderer->AddChild(m_Juice);
                }
        }

        // App.cpp 中：處理 juice 拖拉並檢查是否要給客人喝（修改後）
        if (m_Juice) {
            m_Juice->Update();
            // 檢查是否靠近某位客人
            // 注意：這裡要用 index based loop 或者 iterator 取代 range-based for，以利在迴圈內刪除客人
            for (auto it = m_Customers.begin(); it != m_Customers.end(); /*++it*/)
            {
                auto& cust = *it;
                float dist = glm::distance(
                    cust->m_Transform.translation,
                    m_Juice->m_Transform.translation
                );
                if (dist < 50.0f && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
                {
                    // 1. 先確認這位客人確實要喝果汁
                    if (cust->GetRequestedFood() == "Juice")
                    {
                        // (a) 標記客人已經吃到「Juice」
                        cust->RecordFood("Juice");

                        // (b) 刪除客人的 PatienceText
                        if (auto pt = cust->GetPatienceText()) {
                            m_Renderer->RemoveChild(pt);
                            cust->SetPatienceText(nullptr);
                        }
                        // (c) 刪除客人的 Order Icon
                        if (cust->GetOrderIcon()) {
                            m_Renderer->RemoveChild(cust->GetOrderIcon());
                        }
                        // (d) 把客人本體移出畫面
                        m_Renderer->RemoveChild(cust);
                        it = m_Customers.erase(it);

                        // (e) 加錢（15 元）
                        m_MoneyManager.Add(15);
                    }
                    else
                    {
                        // 如果他不是要「Juice」，可以額外處理：不給錢、或顯示客人不滿的畫面……
                        // 目前先不額外加扣分，改成「只把果汁收回，不移除客人」
                        // m_MoneyManager.Add(-？); // 若要扣分可以在這裡
                    }

                    // 最後，不論有沒有給正確的飲料，都把果汁移除
                    m_Renderer->RemoveChild(m_Juice);
                    m_Juice.reset();

                    // 結束這次內層迴圈，因為果汁已被使用或收回
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }


    }

    // Shop & return
    if (m_ShopButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase3;
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Shawarma/Resources/Image/background/LevelPage1.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
        m_IgnoreNextMouseUp = true;
        return;
    }
    if (m_ReturnButton->IsClicked() ) {
        m_CurrentPhase = phase::phase1;
        m_CurrentState = State::START;
        m_IgnoreNextMouseUp = true;
        return;
    }
    if (m_CurrentPhase == phase::failed && m_RetryButton && m_RetryButton->IsClicked()) {
        // 清除失敗 UI
        m_Renderer->RemoveChild(m_FailureScreen);
        m_Renderer->RemoveChild(m_RetryButton);
        m_FailureScreen.reset();
        m_RetryButton.reset();

        // 重新載入本關
        LoadLevel(m_LevelManager.GetCurrentLevel());
        m_CurrentPhase = phase::phase2;
        return;
    }
    // Final draw/update
    if (m_Renderer) m_Renderer->Update();
    for (auto& friesObj : m_FrenchFriesList) friesObj->Update();
    for (auto& roll : m_Rolls) roll->Update();
}

void App::LoadLevel(const LevelData& level) {
    m_Renderer = std::make_shared<Util::Renderer>();
    // 記得放在 m_Renderer = std::make_shared<...>() 之後
    m_CurrentDay = m_LevelManager.GetCurrentLevelNumber();
    m_DayText->SetText( std::to_string(m_CurrentDay));
    m_Renderer->AddChild(m_DayTextGO);

    m_MoneyManager = MoneyManager(0);
    m_MoneyManager.SetOnChangeCallback([this](int newBal){
        m_MoneyText->SetText("$" + std::to_string(newBal));
    });
    m_MoneyText->SetText("$0");
    // Clear previous
    m_Rolls.clear();
    m_FrenchFriesList.clear();
    toppings.clear();
    m_Cups.clear();
    m_Fries->SetPlaced(false);
    m_Sauce->SetPlaced(false);
    m_Pickle->SetPlaced(false);
    m_ShavedMeat->SetPlaced(false);

    if (m_Juice) {
        m_Renderer->RemoveChild(m_Juice);
        m_Juice.reset();
    }
    // App.cpp::LoadLevel :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}
    int currentIndex = m_LevelManager.GetCurrentLevelIndex();  // 0-based
    std::cout << "current level: " << currentIndex + 1 << std::endl;

    // 保持原有的「從第2關起才限量」機制
    m_EnableIngredientLimit = (currentIndex >= 10);
    m_Pickle->EnableLimit(m_EnableIngredientLimit);
    m_Sauce->EnableLimit(m_EnableIngredientLimit);

    // 新增：從第3關 (currentIndex >= 2) 起啟用「客製配料檢查」
    m_EnableCustomTopping = (currentIndex >= 25);

    if (currentIndex >= 5) {
        m_EnablePatience = true;
    }
    else {
        m_EnablePatience = false;
    }
    // Reset frying counter
    m_FryingCounter = 0;
    if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(0);

    // New renderer
    m_Renderer->AddChild(m_ReturnButton);
    m_Renderer->AddChild(m_Meat);
    m_Renderer->AddChild(m_Knife);
    m_Renderer->AddChild(m_Crust);
    m_Renderer->AddChild(m_Paper);
    m_Renderer->AddChild(m_Fries);
    m_Renderer->AddChild(m_Sauce);
    m_Renderer->AddChild(m_ShavedMeat);
    m_Renderer->AddChild(m_Pickle);

    m_BevMachine = std::make_shared<BeverageMachine>(
    glm::vec2(300.0f, -100.0f),
    glm::vec2(0.5f)   // 放大 150%
);
    m_Renderer->AddChild(m_BevMachine);

    glm::vec2 basePos = m_BevMachine->GetPosition();
    glm::vec2 scale   = m_BevMachine->GetScale();
    glm::vec2 off1(-50.0f, -50.0f), off2(50.0f, -50.0f);

    // 生成第一杯汽水
    auto sodaCup = std::make_shared<Cup>(
        DrinkType::SODA,
        basePos + off1 * scale,    // *scale
        CupState::EMPTY);
    m_Cups.push_back(sodaCup);
    m_Renderer->AddChild(sodaCup);

    // 生成第一杯可乐
    auto colaCup = std::make_shared<Cup>(
        DrinkType::COLA,
        basePos + off2 * scale,    // *scale
        CupState::EMPTY);
    m_Cups.push_back(colaCup);
    m_Renderer->AddChild(colaCup);

    m_JuicePack = std::make_shared<Util::GameObject>(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/juicepack.png"), 4);
    m_JuicePack->m_Transform.translation = glm::vec2(-420.0f, -210.0f);
    m_JuicePack->m_Transform.scale = glm::vec2(0.5f, 0.5f);
    m_Renderer->AddChild(m_JuicePack);

    if (m_EnableIngredientLimit) {
        m_CucumberHand = std::make_shared<CucumberHand>();
        m_SauceHand = std::make_shared<SauceHand>();
        m_Renderer->AddChild(m_CucumberHand);
        m_Renderer->AddChild(m_SauceHand);
    } else {
        m_CucumberHand = nullptr;
        m_SauceHand = nullptr;
    }



    m_Renderer->AddChild(m_PoorMan);
    m_Renderer->AddChild(m_Potato);
    m_Potato->SetPlaced(false);
    m_Renderer->AddChild(m_FryingCounterText);
    m_Renderer->AddChild(m_MoneyTextGO);

    m_Renderer->AddChild(m_Pickle->GetCounterObject());
    m_Pickle->EnableLimit(m_EnableIngredientLimit);
    m_Pickle->ResetCount();

    m_Renderer->AddChild(m_Sauce->GetCounterObject());
    m_Sauce->EnableLimit(m_EnableIngredientLimit);
    m_Sauce->ResetCount();

    m_Background = std::make_shared<BackgroundImage>(level.backgroundImage);
    m_Renderer->AddChild(m_Background);
    m_TotalCustomersThisLevel = (int)level.customers.size();
    m_PatienceFailures = 0;
    m_FailureScreen.reset();
    m_Customers.clear();
    m_LevelManager.StartLevel();
}

void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

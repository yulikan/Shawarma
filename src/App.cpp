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

    // Initial UI & objects
    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ShopButton  = std::make_shared<ShopButton>();
    m_ReturnButton = std::make_shared<ReturnButton>();
    m_Meat        = std::make_shared<Meat>();
    m_Crust       = std::make_shared<Crust>();
    m_Knife       = std::make_shared<Knife>();
    m_Paper       = std::make_shared<Paper>();
    m_PoorMan     = std::make_shared<PoorMan>();

    // Level complete screen
    m_LevelCompleteScreen = std::make_shared<Util::GameObject>(
        std::make_unique<Util::Image>(
            "C:/Users/yello/Shawarma/Resources/Image/background/nextLevel.png"),
        /*layer=*/7
    );
    m_LevelCompleteScreen->m_Transform.translation = glm::vec2(0.0f, 0.0f);
    m_LevelCompleteScreen->m_Transform.scale       = glm::vec2(0.7f, 0.7f);
    m_NextButton = std::make_shared<NextButton>();

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
        // 1. 先設定文字內容為現在是第幾關
        int levelNum = static_cast<int>(m_LevelManager.GetCurrentLevelIndex()) + 1;
        m_LevelText->SetText(std::to_string(levelNum));

        // 2. 把文字顯示物件加入 Renderer（如果還沒加過，也可以每次加，重複加入同一個指標不會多次）
        m_Renderer->AddChild(m_LevelTextGO);

        if (m_NextButton->IsClicked()) {
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
    if (m_CurrentPhase == phase::phase2 && !m_Customers.empty()) {
        for (auto it = m_Customers.begin(); it != m_Customers.end();) {
            auto& cust = *it;
            const auto& eaten = cust->GetEatenFoods();
            if (std::find(eaten.begin(), eaten.end(), cust->GetRequestedFood()) != eaten.end()) {
                // Add money
                if (cust->GetRequestedFood() == "Roll")          m_MoneyManager.Add(50);
                else if (cust->GetRequestedFood() == "FrenchFries") m_MoneyManager.Add(30);
                else if (cust->GetRequestedFood() == "sauce")    m_MoneyManager.Add(10);

                // Remove patience text
                if (auto pt = cust->GetPatienceText()) {
                    m_Renderer->RemoveChild(pt);
                    cust->SetPatienceText(nullptr);
                }
                // Remove icon & customer
                m_Renderer->RemoveChild(cust->GetOrderIcon());
                m_Renderer->RemoveChild(cust);

                it = m_Customers.erase(it);
                continue;
            }
            ++it;
        }
        if (m_Customers.empty()) {
            m_Renderer->AddChild(m_LevelCompleteScreen);
            m_Renderer->AddChild(m_NextButton);
            m_CurrentPhase = phase::levelComplete;
            return;
        }
    }

    // Start game
    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LoadLevel(m_LevelManager.GetCurrentLevel());
    }

    // Phase 2 main logic
    if (m_CurrentPhase == phase::phase2) {
        // Spawn new customers
        auto newCfgs = m_LevelManager.SpawnCustomers((int)m_Customers.size());
        for (auto& cfg : newCfgs) {
            auto customer = std::make_shared<Customer>(cfg.customerImage);
            customer->m_Transform.translation = cfg.position;
            customer->m_Transform.scale       = glm::vec2(0.5f, 0.5f);
            customer->SetRequestedFood(cfg.foodRequest);

            // Order icon
            auto foodIcon = std::make_shared<Util::GameObject>(
                std::make_unique<Util::Image>(cfg.foodIcon), 5);
            if (cfg.foodRequest == "Roll") {
                foodIcon->m_Transform.scale       = glm::vec2(0.08f, 0.08f);
                foodIcon->m_Transform.translation = cfg.position + glm::vec2(0.0f, -55.0f);
            } else {
                foodIcon->m_Transform.scale       = glm::vec2(0.3f, 0.3f);
                foodIcon->m_Transform.translation = cfg.position + glm::vec2(0.0f, -60.0f);
            }
            customer->SetOrderIcon(foodIcon);

            // Patience text
            auto patienceText = std::make_shared<PatienceText>();
            patienceText->SetPatience(60);
            patienceText->m_Transform.translation = cfg.position + glm::vec2(0.0f, -100.0f);
            customer->SetPatienceText(patienceText);
            m_Renderer->AddChild(patienceText);

            // Add to scene
            m_Customers.push_back(customer);
            m_Renderer->AddChild(customer);
            m_Renderer->AddChild(foodIcon);
        }

        // Update patience & handle timeouts
        for (auto it = m_Customers.begin(); it != m_Customers.end();) {
            auto& customer = *it;
            if (auto pt = customer->GetPatienceText()) {
                pt->Update();
                if (pt->GetRemaining() <= 0) {
                    m_Renderer->RemoveChild(pt);
                    customer->SetPatienceText(nullptr);
                    if (customer->GetOrderIcon()) m_Renderer->RemoveChild(customer->GetOrderIcon());
                    m_Renderer->RemoveChild(customer);
                    it = m_Customers.erase(it);
                    continue;
                }
            }
            ++it;
        }

        // Fries button
        if (!m_Fries->IsPlaced() && m_Fries->IsClicked()) {
            m_Fries->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_fries.png",
                "fries"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -174.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Sauce button
        if (!m_Sauce->IsPlaced() && m_Sauce->IsClicked()) {
            m_Sauce->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_sauce.png",
                "sauce"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -208.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Pickle button
        if (!m_Pickle->IsPlaced() && m_Pickle->IsClicked()) {
            m_Pickle->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_pickle.png",
                "pickle"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -155.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // ShavedMeat button
        if (!m_ShavedMeat->IsPlaced() && m_ShavedMeat->IsClicked()) {
            m_ShavedMeat->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Users/yello/Shawarma/Resources/Image/Food/topping_meat.png",
                "shaved_meat"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -192.0f);
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

                // 建立新的 Roll 物件
                auto roll = std::make_shared<Roll>(rollContents);

                // 調整 roll 的位置
                float baseX   = 300.0f;
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
    
        // Customer <> Roll interaction
        for (auto& customer : m_Customers) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end();) {
                auto& rollObj = *it;
                float distance = glm::distance(customer->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        customer->GetEatState() == Customer::EatState::READY_TO_EAT
                    ) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        customer->RecordFood("Roll");
                        for (const auto& ing : rollObj->GetContents())
                            customer->RecordFood(ing);
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
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
    }

    // Shop & return
    if (m_ShopButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase3;
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }
    if (m_ReturnButton->IsClicked()) {
        m_CurrentPhase = phase::phase1;
        m_CurrentState = State::START;
    }

    // Final draw/update
    if (m_Renderer) m_Renderer->Update();
    m_Knife->Update();
    for (auto& friesObj : m_FrenchFriesList) friesObj->Update();
    for (auto& roll : m_Rolls) roll->Update();
}

void App::LoadLevel(const LevelData& level) {
    // Clear previous
    m_Rolls.clear();
    m_FrenchFriesList.clear();
    toppings.clear();

    // Reset frying counter
    m_FryingCounter = 0;
    if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(0);

    // New renderer
    m_Renderer = std::make_shared<Util::Renderer>();
    m_Renderer->AddChild(m_ReturnButton);
    m_Renderer->AddChild(m_Meat);
    m_Renderer->AddChild(m_Knife);
    m_Renderer->AddChild(m_Crust);
    m_Renderer->AddChild(m_Paper);
    m_Renderer->AddChild(m_Fries);
    m_Renderer->AddChild(m_Sauce);
    m_Renderer->AddChild(m_ShavedMeat);
    m_Renderer->AddChild(m_Pickle);
    m_Renderer->AddChild(m_PoorMan);
    m_Renderer->AddChild(m_Potato);
    m_Potato->SetPlaced(false);
    m_Renderer->AddChild(m_FryingCounterText);
    m_Renderer->AddChild(m_MoneyTextGO);

    m_Background = std::make_shared<BackgroundImage>(level.backgroundImage);
    m_Renderer->AddChild(m_Background);

    m_Customers.clear();
    m_LevelManager.StartLevel();
}

void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

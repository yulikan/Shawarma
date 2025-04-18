#include "App.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <unordered_set>
#include "Roll.hpp"
#include "Util/Keycode.hpp"
#include <algorithm> // 為 std::find 引入
#include "App.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <unordered_set>
#include "Roll.hpp"
#include "Util/Keycode.hpp"
#include <algorithm> // 為 std::find 引入
std::vector<LevelData> levels = {
    // Level 1
    { "C:/.../homePage.png", { { "C:/.../Customer/customer1.png", glm::vec2(100, 150), "fries" } } },
    // Level 2
    { "C:/.../restaurant.png", {
             { "C:/.../Customer/customer2.png", glm::vec2(200, 100), "roll" },
             { "C:/.../Customer/customer3.png", glm::vec2(300, 150), "sauce" }
    } },
    // ...
};
void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_Renderer = std::make_shared<Util::Renderer>();

    // 建立初始背景與按鈕
    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ShopButton = std::make_shared<ShopButton>();
    m_ReturnButton = std::make_shared<ReturnButton>();

    // 建立其他遊戲物件
    m_Meat = std::make_shared<Meat>();
    m_Crust = std::make_shared<Crust>();
    m_Knife = std::make_shared<Knife>();
    m_Paper = std::make_shared<Paper>();
    m_PoorMan = std::make_shared<PoorMan>();

    // 配料物件
    m_Fries = std::make_shared<Fries>();
    m_Pickle = std::make_shared<Pickle>();
    m_Sauce = std::make_shared<Sauce>();
    m_ShavedMeat = std::make_shared<ShavedMeat>();

    m_Potato = std::make_shared<Potato>();

    // 顯示 Frying Counter 文字
    m_FryingCounterText = std::make_shared<FryingCounterText>();
    // Money text: 建立 Text 與 GameObject 包裝
    m_MoneyManager = MoneyManager(0);
    // Text drawable
    auto textDrawable = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/msyh.ttc",  // 字體路徑
        30,
        "$0",
        Util::Color{255, 255, 255}
    );
    // 包裝成 GameObject
    m_MoneyTextGO = std::make_shared<Util::GameObject>(
        textDrawable, /*zOrder=*/10
    );
    m_MoneyTextGO->m_Transform.translation = glm::vec2(460.0f, 325.0f);
    m_MoneyText = textDrawable;

    // 綁定回調：餘額改變即更新文字內容
    m_MoneyManager.SetOnChangeCallback([this](int newBal) {
        m_MoneyText->SetText("$" + std::to_string(newBal));
    });

    // 初始畫面只加入背景與部分按鈕
    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);
}

void App::Update() {
    // 移除原本呼叫的 m_Fries->OnClick();（因為我們改由下面的判斷處理）

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }



    // 只有第2階段且有客人時，檢查是否完成訂單
    if (m_CurrentPhase == phase::phase2 && !m_Customers.empty()) {
        for (auto it = m_Customers.begin(); it != m_Customers.end(); ) {
            auto& cust = *it;
            const auto& eaten = cust->GetEatenFoods();
            if (std::find(eaten.begin(), eaten.end(), cust->GetRequestedFood()) != eaten.end()) {
                // 根據食物類型增加金錢
                std::string food = cust->GetRequestedFood();
                if (food == "Roll")         m_MoneyManager.Add(50);
                else if (food == "FrenchFries") m_MoneyManager.Add(30);
                else if (food == "sauce")   m_MoneyManager.Add(10);

                // 移除訂單圖示與客人
                m_Renderer->RemoveChild(cust->GetOrderIcon());
                m_Renderer->RemoveChild(cust);
                it = m_Customers.erase(it);
                continue;
            }
            ++it;
        }
        if (m_Customers.empty()) {
            m_LevelManager.NextLevel();
            LoadLevel(m_LevelManager.GetCurrentLevel());
            std::cout << "切換到下一關" << std::endl;
            return;
        }
    }






    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        // 直接由 LoadLevel 建立第一關的所有物件
        LoadLevel(m_LevelManager.GetCurrentLevel());
    }

    if (m_CurrentPhase == phase::phase2) {
        // 檢查 Fries 按鈕：若尚未放置且被點擊，則建立新 topping 並加入 renderer
        if (!m_Fries->IsPlaced() && m_Fries->IsClicked()) {
            m_Fries->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/topping_fries.png",
                "fries"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -174.0f);  // 放在烤餅中心
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 Sauce 按鈕
        if (!m_Sauce->IsPlaced() && m_Sauce->IsClicked()) {
            m_Sauce->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/topping_sauce.png",
                "sauce"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -208.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 Pickle 按鈕
        if (!m_Pickle->IsPlaced() && m_Pickle->IsClicked()) {
            m_Pickle->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/topping_pickle.png",
                "pickle"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -155.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 ShavedMeat 按鈕
        if (!m_ShavedMeat->IsPlaced() && m_ShavedMeat->IsClicked()) {
            m_ShavedMeat->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/topping_meat.png",
                "shaved_meat"
            );
            newTopping->m_Transform.translation = glm::vec2(180.0f, -192.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        if (!m_Potato->IsPlaced() && m_Potato->IsClicked()) {
            m_Potato->SetPlaced(true);
            m_Frying = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0410/Shawarma/Resources/Image/Food/frying.png",
                "potato"
            );
            m_Frying->m_Transform.translation = glm::vec2(480.0f, -40.0f);
            m_Renderer->AddChild(m_Frying);
            m_Pstate = 0;
        }

        // 只有當 Pstate == 0（已生成 frying）時才檢查點擊 frying
        if (m_Pstate == 0 && m_Frying) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) &&
                mousePos.x >= 400 && mousePos.x <= 550 &&
                mousePos.y >= -100 && mousePos.y <= 0)
            {
                m_Pstate = 1;
            }

            if (m_Pstate == 1) {
                m_Pstate = 0;
                m_FryingCounter++;
                if (m_FryingCounterText) {
                    m_FryingCounterText->UpdateCounter(m_FryingCounter);
                }

                std::cout << "Frying topping clicked, count: " << m_FryingCounter << std::endl;
            }
        }

        // 處理客人與 FrenchFries 的互動邏輯…
        for (auto& customer : m_Customers) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end(); ) {
                auto& friesObj = *it;
                if (customer->IsNearFrenchFries(*friesObj)) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    //std::cout << "READY_TO_EAT" << std::endl;
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        customer->GetEatState() == Customer::EatState::READY_TO_EAT) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        customer->RecordFood("FrenchFries");

                        // 印出所有客人的吃掉食物記錄（僅供除錯用）
                        for (const auto& customer : m_Customers) {
                            std::cout << " RequestFood:"<< customer->GetRequestedFood() << " EatenFood:";
                            for (const auto& food : customer->GetEatenFoods()) {
                                std::cout << food << " ";
                            }
                            std::cout << std::endl;
                        }
                        std::cout << "EATEN" << std::endl;
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                        }
                }
                else {
                    if (customer->GetEatState() != Customer::EatState::EATEN) {
                        customer->SetEatState(Customer::EatState::NOT_EATEN);
                    }
                }
                ++it;
            }
        }

        // 按下 F 鍵補充新的薯條
        if (m_FryingCounter == 5) {
            // 1. 加入新的薯條
            auto newFries = std::make_shared<FrenchFries>();
            newFries->m_Transform.translation = glm::vec2(100.0f + m_FrenchFriesList.size() * 60, -50.0f);
            m_FrenchFriesList.push_back(newFries);
            m_Renderer->AddChild(newFries);

            // 2. 移除 frying 物件
            if (m_Frying) {
                m_Renderer->RemoveChild(m_Frying);
                m_Frying.reset();  // 釋放指標
            }

            // 3. 重設計數器與 potato 狀態
            m_FryingCounter = 0;
            if (m_FryingCounterText) {
                m_FryingCounterText->UpdateCounter(0);
            }
            m_Potato->SetPlaced(false);
            m_Pstate = 2;  // 若你使用 2 表示尚未開始
        }


        // 處理客人與 Roll 的互動邏輯
        for (auto& customer : m_Customers) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end(); ) {
                auto& rollObj = *it;
                // 計算客人與 roll 的距離（例如 50 像素內算靠近）
                float distance = glm::distance(customer->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    // 當滑鼠釋放且客人處於 READY_TO_EAT 狀態，則吃掉 roll
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && customer->GetEatState() == Customer::EatState::READY_TO_EAT) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        // 記錄 roll 內的所有配料
                        customer->RecordFood("Roll");
                        for (const auto& ingredient : rollObj->GetContents()) {
                            customer->RecordFood(ingredient);
                        }
                        // 從 Renderer 中移除，並從 m_Rolls 容器中刪除
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
                        g_IsObjectDragging = false;  // 重置拖曳旗標
                        continue;  // 直接跳到下一個 roll
    }

                }
                else {
                    if (customer->GetEatState() != Customer::EatState::EATEN) {
                        customer->SetEatState(Customer::EatState::NOT_EATEN);
                    }
                }
                ++it;
            }
        }

        // 處理乞丐與 Roll 的互動邏輯
        if (m_PoorMan) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end(); ) {
                auto& rollObj = *it;
                // 計算客人與 roll 的距離（例如 50 像素內算靠近）
                float distance = glm::distance(m_PoorMan->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    // 當滑鼠釋放且客人處於 READY_TO_EAT 狀態，則吃掉 roll
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        // 從 Renderer 中移除，並從 m_Rolls 容器中刪除
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
                        g_IsObjectDragging = false;  // 重置拖曳旗標
                        continue;  // 直接跳到下一個 roll
                    }

                }
                else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN) {
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                    }
                }
                ++it;
            }
        }
        // 處理乞丐與 FrenchFries 的互動邏輯
        if(m_PoorMan) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end(); ) {
                auto& friesObj = *it;
                if (m_PoorMan->IsNearFrenchFries(*friesObj)) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    //std::cout << "READY_TO_EAT" << std::endl;
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        std::cout << "EATEN" << std::endl;
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                        }
                }
                else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN) {
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                    }
                }
                ++it;
            }
        }
    }


    // 當使用者選完配料後按下 R 鍵
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
    
    if (m_ShopButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase3;
        LOG_TRACE("Shop button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0410/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }

    if (m_ReturnButton->IsClicked()) {
        m_CurrentPhase = phase::phase1;
        std::cout << "Return button clicked. Returning..." << std::endl;
        m_CurrentState = State::START;
    }

    if (m_Renderer) {
        m_Renderer->Update();
    }
    m_Crust->Update();
    m_Knife->Update();
    for (auto& friesObj : m_FrenchFriesList) {
        friesObj->Update();
    }
    // 新增：更新所有 roll 物件，確保它們能夠處理拖曳
    for (auto& roll : m_Rolls) {
        roll->Update();
    }

}

void App::LoadLevel(const LevelData& level) {
    // 重新建立 Renderer（清除前一關所有物件）
    m_Renderer = std::make_shared<Util::Renderer>();

    // 加入持續物件（例如返回按鈕、肉、刀、餅皮、紙張等）
    m_Renderer->AddChild(m_ReturnButton);
    m_Renderer->AddChild(m_Meat);
    m_Renderer->AddChild(m_Knife);
    m_Renderer->AddChild(m_Crust);
    m_Renderer->AddChild(m_Paper);
    // 如果配料也是持續物件，可以在這裡加入
    m_Renderer->AddChild(m_Fries);
    m_Renderer->AddChild(m_Sauce);
    m_Renderer->AddChild(m_ShavedMeat);
    m_Renderer->AddChild(m_Pickle);
    m_Renderer->AddChild(m_PoorMan);
    m_Renderer->AddChild(m_Potato);
    m_Potato->SetPlaced(false);  // 重設為未放置
    m_Renderer->AddChild(m_FryingCounterText);
    m_Renderer->AddChild(m_MoneyTextGO);
    // 加入新關卡背景
    m_Background = std::make_shared<BackgroundImage>(level.backgroundImage);
    m_Renderer->AddChild(m_Background);

    // 清空原有客人容器（僅容器，不必遍歷刪除，因為 Renderer 是新建的）
    m_Customers.clear();

    // 根據 LevelData 建立客人，並加入 Renderer
    for (const auto& custConfig : level.customers) {
        auto customer = std::make_shared<Customer>(custConfig.customerImage);
        customer->m_Transform.translation = custConfig.position;
        customer->m_Transform.scale = glm::vec2(0.5f, 0.5f);
        customer->SetRequestedFood(custConfig.foodRequest);

        // 建立食物 icon，並根據 foodRequest 做微調
        std::shared_ptr<Util::GameObject> foodIcon;
        if (custConfig.foodRequest == "Roll") {
            // 對 Roll 做較小的 scale 與略微不同的偏移
            foodIcon = std::make_shared<Util::GameObject>(
                std::make_unique<Util::Image>(custConfig.foodIcon), 5);
            foodIcon->m_Transform.scale = glm::vec2(0.08f, 0.08f);
            foodIcon->m_Transform.translation = custConfig.position + glm::vec2(0.0f, -55.0f);
        } else {
            // 其他食物 icon 預設 scale 與偏移
            foodIcon = std::make_shared<Util::GameObject>(
                std::make_unique<Util::Image>(custConfig.foodIcon), 5);
            foodIcon->m_Transform.scale = glm::vec2(0.3f, 0.3f);
            foodIcon->m_Transform.translation = custConfig.position + glm::vec2(0.0f, -60.0f);
        }

        // 將訂單 icon 存入客人
        customer->SetOrderIcon(foodIcon);

        m_Customers.push_back(customer);
        m_Renderer->AddChild(customer);
        m_Renderer->AddChild(foodIcon);
    }

}




void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

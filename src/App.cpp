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
bool FrenchFries::s_IsDragging = false;

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

    // 配料物件
    m_Fries = std::make_shared<Fries>();
    m_Pickle = std::make_shared<Pickle>();
    m_Sauce = std::make_shared<Sauce>();
    m_ShavedMeat = std::make_shared<ShavedMeat>();

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

    // 當點擊開始按鈕且處於 phase1 時，切換到餐廳畫面（phase2）
    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LOG_TRACE("Start button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0320/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });

        m_Renderer->AddChild(m_ReturnButton);
        m_Renderer->AddChild(m_Meat);
        m_Renderer->AddChild(m_Knife);
        m_Renderer->AddChild(m_Crust);
        m_Renderer->AddChild(m_Paper);

        // 建立互動用 FrenchFries 物件
        for (int i = 0; i < 3; i++) {
            auto friesObj = std::make_shared<FrenchFries>();
            friesObj->m_Transform.translation = glm::vec2(100.0f + i * 60, -50.0f);
            m_FrenchFriesList.push_back(friesObj);
            m_Renderer->AddChild(friesObj);
        }

        // 將作為配料按鈕的物件加入畫面
        m_Renderer->AddChild(m_Fries);
        m_Renderer->AddChild(m_Sauce);
        m_Renderer->AddChild(m_Pickle);
        m_Renderer->AddChild(m_ShavedMeat);

        // 建立客人
        for (int i = 0; i < 2; i++) {
            auto customer = std::make_shared<Customer>();
            customer->m_Transform.translation = glm::vec2(100.f + i * 200, 85.0f);
            m_Customers.push_back(customer);
            m_Renderer->AddChild(customer);
        }
    }

    if (m_CurrentPhase == phase::phase2) {
        // 檢查 Fries 按鈕：若尚未放置且被點擊，則建立新 topping 並加入 renderer
        if (!m_Fries->IsPlaced() && m_Fries->IsClicked()) {
            m_Fries->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0320/Shawarma/Resources/Image/Food/fries.png",
                "fries"
            );
            newTopping->m_Transform.translation = glm::vec2(200.0f, -170.0f);  // 放在烤餅中心
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 Sauce 按鈕
        if (!m_Sauce->IsPlaced() && m_Sauce->IsClicked()) {
            m_Sauce->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0320/Shawarma/Resources/Image/Food/sauce.png",
                "sauce"
            );
            newTopping->m_Transform.translation = glm::vec2(200.0f, -170.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 Pickle 按鈕
        if (!m_Pickle->IsPlaced() && m_Pickle->IsClicked()) {
            m_Pickle->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0320/Shawarma/Resources/Image/Food/pickle.png",
                "pickle"
            );
            newTopping->m_Transform.translation = glm::vec2(200.0f, -170.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // 檢查 ShavedMeat 按鈕
        if (!m_ShavedMeat->IsPlaced() && m_ShavedMeat->IsClicked()) {
            m_ShavedMeat->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                "C:/Shawarma/CHAO0320/Shawarma/Resources/Image/Food/shaved_meat.png",
                "shaved_meat"
            );
            newTopping->m_Transform.translation = glm::vec2(200.0f, -170.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
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
                        customer->RecordFood("fries");
                        std::cout << "EATEN" << std::endl;
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
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

        // 處理客人與 Roll 的互動邏輯
        for (auto& customer : m_Customers) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end(); ) {
                auto& rollObj = *it;
                // 計算客人與 roll 的距離（例如 50 像素內算靠近）
                float distance = glm::distance(customer->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    // 當滑鼠釋放且客人處於 READY_TO_EAT 狀態，則吃掉 roll
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        customer->GetEatState() == Customer::EatState::READY_TO_EAT) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        // 記錄 roll 內的所有配料
                        for (const auto& ingredient : rollObj->GetContents()) {
                            customer->RecordFood(ingredient);
                        }
                        std::cout << "Roll EATEN" << std::endl;
                        // 從 Renderer 中移除，並從 m_Rolls 容器中刪除
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
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
        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0320/Shawarma/Resources/Image/background/restaurant.png");
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


void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

#include "App.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <unordered_set>
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

    // 建立食材物件（以 Topping 表示，帶有 type 參數）
    m_Fries = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/fries.png", "fries");
    m_Sauce = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/sauce.png", "sauce");
    m_Pickle = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/pickle.png", "pickle");
    m_ShavedMeat = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/shaved_meat.png", "shaved_meat");

    // 初始畫面只加入背景與部分按鈕
    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);
}

void App::Update() {
    // 範例中直接使用成員 m_Fries->OnClick() 等函式處理點擊邏輯
    m_Fries->OnClick();

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 當點擊開始按鈕且處於 phase1 時，切換到餐廳畫面（phase2）
    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LOG_TRACE("Start button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });

        m_Renderer->AddChild(m_ReturnButton);
        m_Renderer->AddChild(m_Meat);
        m_Renderer->AddChild(m_Knife);
        m_Renderer->AddChild(m_Crust);

        // 建立多個互動用的 FrenchFries 物件（用來檢查客人是否靠近）
        for (int i = 0; i < 3; i++) {
            auto friesObj = std::make_shared<FrenchFries>();
            friesObj->m_Transform.translation = glm::vec2(100.0f + i * 60, -100.0f);
            m_FrenchFriesList.push_back(friesObj);
            m_Renderer->AddChild(friesObj);
        }

        // 將作為食材按鈕的 Topping 物件也加入畫面
        m_Renderer->AddChild(m_Fries);
        m_Renderer->AddChild(m_Sauce);
        m_Renderer->AddChild(m_Pickle);
        m_Renderer->AddChild(m_ShavedMeat);

        // 建立多個客人
        for (int i = 0; i < 2; i++) {
            auto customer = std::make_shared<Customer>();
            customer->m_Transform.translation = glm::vec2(300.0f + i * 200, 0.0f);
            m_Customers.push_back(customer);
            m_Renderer->AddChild(customer);
        }
    }

    // 檢查各食材按鈕是否被點擊，若是則新增對應 Topping 物件到畫面
    if (m_Fries->IsClicked() && m_CurrentPhase == phase::phase2) {
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/fries.png", "fries");
        LOG_TRACE("Fries topping added");
        m_Renderer->AddChild(newTopping);
    }
    else if (m_Sauce->IsClicked() && m_CurrentPhase == phase::phase2) {
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/sauce.png", "sauce");
        LOG_TRACE("Sauce topping added");
        m_Renderer->AddChild(newTopping);
    }
    else if (m_Pickle->IsClicked() && m_CurrentPhase == phase::phase2) {
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/pickle.png", "pickle");
        LOG_TRACE("Pickle topping added");
        m_Renderer->AddChild(newTopping);
    }
    else if (m_ShavedMeat->IsClicked() && m_CurrentPhase == phase::phase2) {
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/shaved_meat.png", "shaved_meat");
        LOG_TRACE("ShavedMeat topping added");
        m_Renderer->AddChild(newTopping);
    }

    // 檢查客人與每個 FrenchFries 物件的互動（例如當客人靠近時，改變狀態）
    if (m_CurrentPhase == phase::phase2) {
        for (auto& customer : m_Customers) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end(); ) {
                auto& friesObj = *it;
                if (customer->IsNearFrenchFries(*friesObj)) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    std::cout << "READY_TO_EAT" << std::endl;
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
    }

    if (m_ShopButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase3;
        LOG_TRACE("Shop button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Resources/Image/background/restaurant.png");
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
}

void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

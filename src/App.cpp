#include "App.hpp"

#include <iostream>

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <unordered_set>
#include "Util/Keycode.hpp" // for Keycode

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;

    // 畫面
    m_Renderer = std::make_shared<Util::Renderer>();
    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ShopButton = std::make_shared<ShopButton>();
    m_ReturnButton = std::make_shared<ReturnButton>();
    //m_ExitButton = std::make_shared<ExitButton>();
    //m_Boss = std::make_shared<Boss>();

    // 食物
    m_Meat = std::make_shared<Meat>();
    m_Crust = std::make_shared<Crust>();
    m_Knife = std::make_shared<Knife>();
    m_Fries = std::make_shared<Fries>();
    m_Sauce = std::make_shared<Sauce>();
    m_Pickle = std::make_shared<Pickle>();
    m_ShavedMeat = std::make_shared<ShavedMeat>();
    m_FrenchFries = std::make_shared<FrenchFries>();

    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);



}

//Phase1 -> Home
//Phase2 -> Restaurant
//Phase3 -> Shop
std::unordered_set<std::string> placedToppings;
void App::Update() {
    std::unordered_set<std::string> placedToppings;

    Fries fries;
    Sauce sauce;
    Pickle pickle;
    ShavedMeat shavedMeat;

    fries.OnClick(placedToppings);
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LOG_TRACE("Start button clicked! Switching background.");

        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{m_Background});

        m_Renderer->AddChild(m_ReturnButton);
        m_Renderer->AddChild(m_Meat);
        m_Renderer->AddChild(m_Knife);
        m_Renderer->AddChild(m_Crust);
        m_Renderer->AddChild(m_FrenchFries);

        // 添加新食材
        m_Renderer->AddChild(m_Fries);
        m_Renderer->AddChild(m_Sauce);
        m_Renderer->AddChild(m_Pickle);
        m_Renderer->AddChild(m_ShavedMeat);

        // 新增客人
        m_Customer = std::make_shared<Customer>();
        m_Renderer->AddChild(m_Customer);
    }

    //
    // if (m_Fries->IsClicked() || m_Sauce->IsClicked() || m_Pickle->IsClicked() || m_ShavedMeat->IsClicked()) {
    //     LOG_TRACE("Ingredient clicked! Updating crust image.");
    //     std::cout<<"Ingredient clicked!"<<std::endl;
    //     //m_Crust = std::make_shared<Crust>();
    //     //m_Renderer->AddChild(m_Crust);
    // }

    //比較破的加菜寫法嘻嘻
    if (m_Fries->IsClicked() && m_CurrentPhase == phase::phase2) {
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/fries.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_Sauce->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/sauce.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_Pickle->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/pickle.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_ShavedMeat->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Users/yello/Shawarma/Resources/Image/Food/shaved_meat.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    if (m_CurrentPhase == phase::phase2 && m_Customer && m_FrenchFries) {
        if (m_Customer->IsNearFrenchFries(*m_FrenchFries)) {
            // 如果客人與薯條碰撞
            m_Customer->SetEatState(Customer::EatState::READY_TO_EAT);
            std::cout<<"READY_TO_EAT";
             if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                       m_Customer->GetEatState() == Customer::EatState::READY_TO_EAT) {
                // 如果左鍵已放，且之前處於準備吃狀態，則確認吃掉薯條
                m_Customer->SetEatState(Customer::EatState::EATEN);
                m_Customer->RecordFood("fries");
                std::cout<<"EATEN";
                // 移除薯條物件（假設 Renderer 提供 RemoveChild 方法）
                m_Renderer->RemoveChild(m_FrenchFries);
                       }
        } else {
            // 未碰撞，狀態保持為還沒吃（除非已經吃掉）
            if (m_Customer->GetEatState() != Customer::EatState::EATEN) {
                m_Customer->SetEatState(Customer::EatState::NOT_EATEN);
            }
        }
    }


    if (m_ShopButton->IsClicked() && (m_CurrentPhase == phase::phase1) ) {
        m_CurrentPhase= phase::phase3;
        LOG_TRACE("Shop button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{m_Background});
        m_Renderer->AddChild(m_ReturnButton);

    }

    if (m_ReturnButton->IsClicked()) { // 透過 IfExit() 判斷是否要退出
        m_CurrentPhase= phase::phase1;
        std::cout << "Return button clicked. Returning..." << std::endl;
        m_CurrentState = State::START;
    }

    // if (m_ExitButton->IsClicked()) { // 透過 IfExit() 判斷是否要退出
    //     std::cout << "Exit button clicked. Exiting..." << std::endl;
    //     m_CurrentState = State::END;
    // }


    if (m_Renderer) {
        m_Renderer->Update();  // 确保 Renderer

    }
    m_Crust->Update();
    m_Knife->Update();
    m_FrenchFries->Update();
}



void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
    m_Renderer.reset();  // 假設 Renderer 有提供這個函數
}
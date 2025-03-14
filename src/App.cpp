#include "App.hpp"

#include <iostream>

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
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


    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);



}

//Phase1 -> Home
//Phase2 -> Restaurant
//Phase3 -> Shop

void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LOG_TRACE("Start button clicked! Switching background.");

        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/background/restaurant.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{m_Background});

        m_Renderer->AddChild(m_ReturnButton);
        m_Renderer->AddChild(m_Meat);
        m_Renderer->AddChild(m_Knife);
        m_Renderer->AddChild(m_Crust);

        // 添加新食材
        m_Renderer->AddChild(m_Fries);
        m_Renderer->AddChild(m_Sauce);
        m_Renderer->AddChild(m_Pickle);
        m_Renderer->AddChild(m_ShavedMeat);

        //m_Renderer->AddChild(m_Boss);
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
        auto newTopping = std::make_shared<Topping>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/fries.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_Sauce->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/sauce.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_Pickle->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/pickle.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }
    else if(m_ShavedMeat->IsClicked() && m_CurrentPhase == phase::phase2){
        auto newTopping = std::make_shared<Topping>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/shaved_meat.png");
        LOG_TRACE("Fries");
        m_Renderer->AddChild(newTopping);
    }

    if (m_ShopButton->IsClicked() && (m_CurrentPhase == phase::phase1) ) {
        m_CurrentPhase= phase::phase3;
        LOG_TRACE("Shop button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/background/restaurant.png");
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
}



void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
    m_Renderer.reset();  // 假設 Renderer 有提供這個函數
}

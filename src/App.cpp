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

    m_Renderer = std::make_shared<Util::Renderer>();
    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ExitButton = std::make_shared<ExitButton>();

    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_Background);


}


void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_StartButton->IsClicked()) {
        LOG_TRACE("Start button clicked! Switching background.");
        m_Background = std::make_shared<BackgroundImage>("C:/Shawarma/CHAO0306/Shawarma/Resources/Image/background/kitchen.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{m_Background});
        m_Renderer->AddChild(m_ExitButton);
    }

    if (m_ExitButton->IsClicked()) { // 透過 IfExit() 判斷是否要退出
        std::cout << "Exit button clicked. Exiting..." << std::endl;
        m_CurrentState = State::END;
    }


    if (m_Renderer) {
        m_Renderer->Update();  // 确保 Renderer 在每一帧更新
    }
}



void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

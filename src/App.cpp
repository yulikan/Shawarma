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
    m_ExitButton = std::make_shared<ExitButton>();

    m_Renderer->AddChild(m_Background);


}


void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // 按下 Q 鍵切換背景圖
    if (Util::Input::IsKeyUp(Util::Keycode::Q)) {
        LOG_TRACE("Switching background to background2.png");
        // 重新分配背景物件
        m_Background = std::make_shared<BackgroundImage>("C:/Users/yello/Shawarma/Resources/Image/background/kitchen.png");
        // 重新初始化渲染器（如果沒有 SetGameObjects 方法）
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

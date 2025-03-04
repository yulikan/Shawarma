#include "App.hpp"

#include <iostream>

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"


void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_Background = std::make_shared<BackgroundImage>();

    // 初始化渲染器，并添加背景
    m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{m_Background});
}


void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    if (m_Renderer) {
        m_Renderer->Update();  // 确保 Renderer 在每一帧更新
    }
}



void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

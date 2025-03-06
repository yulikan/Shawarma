#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp" // for Keycode

//場景區
class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0306/Shawarma/Resources/Image/background/StartPage.png"),
        1) {
        m_Transform.scale = glm::vec2(0.69f, 0.69f); // 调整缩放比例
    }
    // 接受不同的圖片路徑
    explicit BackgroundImage(const std::string& imagePath)
        : GameObject(std::make_unique<Util::Image>(imagePath), 1) {
        m_Transform.scale = glm::vec2(0.69f, 0.69f);
    }
};

//按鈕
// 退出按鈕（使用 GameObject）
class ExitButton : public Util::GameObject {
public:
    ExitButton() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0306/Shawarma/Resources/Image/Object/exitBtn.png"),
        5) {
        m_Transform.translation = glm::vec2(-540.0f, 300.0f); // 設定按鈕位置
        m_Transform.scale = glm::vec2(0.3f, 0.3f); // 設定按鈕大小
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();

        // 檢測滑鼠左鍵是否按下
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        // Get the actual image dimensions
        float imageWidth = 470.0f * m_Transform.scale.x;
        float imageHeight = 175.0f * m_Transform.scale.y;

        // Calculate button boundaries based on the image center
        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth/2, imageHeight/2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth/2, imageHeight/2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

};


class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    std::shared_ptr<BackgroundImage> m_Background;
    std::shared_ptr<Util::Renderer> m_Renderer;
    std::shared_ptr<ExitButton> m_ExitButton;

    //std::shared_ptr<Util::Image> m_BackgroundImage; // 背景圖片
};

#endif

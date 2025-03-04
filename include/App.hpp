#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"

//場景區
class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0304/Shawarma/Resources/Image/background/StartPage.png"),
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

    //std::shared_ptr<Util::Image> m_BackgroundImage; // 背景圖片
};

#endif

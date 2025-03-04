#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Renderer.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/background/phase0.png"),
        1)
    { }
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

    //std::shared_ptr<Util::Image> m_BackgroundImage; // 背景圖片
};

#endif

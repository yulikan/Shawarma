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
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/background/homePage.png"),
        1) {
        m_Transform.scale = glm::vec2(0.65f, 0.65f); // 调整缩放比例
    }
    // 接受不同的圖片路徑
    explicit BackgroundImage(const std::string& imagePath)
        : GameObject(std::make_unique<Util::Image>(imagePath), 1) {
        m_Transform.scale = glm::vec2(1.8f, 1.85f);
    }
};

//按鈕
// 開始按鈕
class StartButton : public Util::GameObject {
public:
    StartButton() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Object/startBtn1.png"),
        5) {
        m_Transform.translation = glm::vec2(-328.0f, 45.0f); // 設定按鈕位置
        m_Transform.scale = glm::vec2(0.38f, 0.38f); // 設定按鈕大小
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 450.0f * m_Transform.scale.x;
        float imageHeight = 600.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }public:
};

// 商店按鈕
class ShopButton : public Util::GameObject {
public:
    ShopButton() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Object/shopBtn.png"),
        5) {
        m_Transform.translation = glm::vec2(-42.0f, -87.0f); // 設定按鈕位置
        m_Transform.scale = glm::vec2(0.35f, 0.35f); // 設定按鈕大小
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 450.0f * m_Transform.scale.x;
        float imageHeight = 320.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }
};

// 返回餐廳按鈕
class ReturnButton : public Util::GameObject {
public:
    ReturnButton() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Object/startBtn.png"),
        5) {
        m_Transform.translation = glm::vec2(-400.0f, 300.0f); // 設定按鈕位置
        m_Transform.scale = glm::vec2(0.2f, 0.2f); // 設定按鈕大小
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 450.0f * m_Transform.scale.x;
        float imageHeight = 320.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }
};

class Topping : public Util::GameObject {
public:
    Topping(const std::string& imagePath)
        : GameObject(std::make_unique<Util::Image>(imagePath), 4), m_IsPlaced(false) {
        m_Transform.translation = glm::vec2(200.0f, -170.0f); // 初始位置
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    void OnClick() {
        if (!m_IsPlaced && IsClicked()) {
            // 只允許第一次點擊時放置食材
            m_IsPlaced = true;
            m_Transform.translation = glm::vec2(200.0f, -170.0f); // 放置到餅皮上
        }
    }

    void Update(){
        if (!m_IsPlaced) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
            bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
            bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

            if (mouseDown && IsClicked()) {
                m_IsDragging = true;
                m_Offset = m_Transform.translation - mousePos;
            }

            if (mousePressed && m_IsDragging) {
                m_Transform.translation = mousePos + m_Offset;
            }

            if (mouseReleased) {
                m_IsDragging = false;
            }
        }
    }

private:
    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 100.0f * m_Transform.scale.x;
        float imageHeight = 100.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

    bool m_IsPlaced;
    bool m_IsDragging;
    glm::vec2 m_Offset;
};



// 老闆
// class Boss : public Util::GameObject {
//     public:
//     Boss() : GameObject(
//             std::make_unique<Util::Image>("C:/Shawarma/CHAO0306/Shawarma/Resources/Image/Object/boss1.png"),
//             5) {
//         m_Transform.translation = glm::vec2(-450.0f, -200.0f); // 設定按鈕位置
//         m_Transform.scale = glm::vec2(0.5f, 0.5); // 設定按鈕大小
//     }
// };

// 沙威瑪食材
// 肉串
class Meat : public Util::GameObject {
 public:
     Meat() : GameObject(
             std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/meat.png"),
             2) {
         m_Transform.translation = glm::vec2(-390.0f, 90.0f); // 設定按鈕位置
         m_Transform.scale = glm::vec2(0.34f, 0.34f); // 設定按鈕大小
     }
 };

// 餅皮
class Crust : public Util::GameObject {
public:
    Crust() : GameObject(
            std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/crust.png"),
            3), m_IsDragging(false) {
        m_Transform.translation = glm::vec2(200.0f, -170.0f); // 設定初始位置
        m_Transform.scale = glm::vec2(0.2f, 0.2f); // 設定縮放大小
    }

    void Update() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
        bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        if (mouseDown && IsClicked()) {
            // 開始拖曳，記錄滑鼠與圖片的相對位置
            m_IsDragging = true;
            m_Offset = m_Transform.translation - mousePos;
        }

        if (mousePressed && m_IsDragging) {
            // 當拖曳時，更新圖片位置
            m_Transform.translation = mousePos + m_Offset;
        }

        if (mouseReleased) {
            // 放開滑鼠後停止拖曳
            m_IsDragging = false;
        }
    }

private:
    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 700.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

private:
    bool m_IsDragging;
    glm::vec2 m_Offset;
};

// 刀子
class Knife : public Util::GameObject {
public:
    Knife() : GameObject(
            std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Object/knife_origin.png"),
            3), m_IsDragging(false) {
        m_Transform.translation = glm::vec2(-480.0f, -160.0f); // 設定初始位置
        m_Transform.scale = glm::vec2(0.2f, 0.2f); // 設定縮放大小
    }

    void Update() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
        bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        if (mouseDown && IsClicked()) {
            // 開始拖曳，記錄滑鼠與圖片的相對位置
            m_IsDragging = true;
            m_Offset = m_Transform.translation - mousePos;
        }

        if (mousePressed && m_IsDragging) {
            // 當拖曳時，更新圖片位置
            m_Transform.translation = mousePos + m_Offset;
        }

        if (mouseReleased) {
            // 放開滑鼠後停止拖曳
            m_IsDragging = false;
        }
    }

private:
    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 700.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

    bool m_IsDragging;
    glm::vec2 m_Offset;
};

//薯條
class Fries : public Util::GameObject {
public:

    Fries() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/fries.png"), 3) {
        m_Transform.translation = glm::vec2(5.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

    }

    void OnClick() {
        if (IsClicked()) {
            toppings.push_back(std::make_shared<Topping>(
                "C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/fries.png"
            ));
        }
    }

private:
    std::vector<std::shared_ptr<Topping>> toppings;
};

//醬汁
class Sauce : public Util::GameObject {
public:
    Sauce() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/sauce.png"), 3) {
        m_Transform.translation = glm::vec2(-100.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

    }

    void OnClick() {
        if (IsClicked()) {
            toppings.push_back(std::make_shared<Topping>(
                "C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/sauce.png"
            ));
        }
    }

private:
    std::vector<std::shared_ptr<Topping>> toppings;
};

class Pickle : public Util::GameObject {
public:
    Pickle() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/pickle.png"), 3) {
        m_Transform.translation = glm::vec2(-220.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

    }

    void OnClick() {
        if (IsClicked()) {
            toppings.push_back(std::make_shared<Topping>(
                "C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/pickle.png"
            ));
        }
    }

private:
    std::vector<std::shared_ptr<Topping>> toppings;
};

class ShavedMeat : public Util::GameObject {
public:
    ShavedMeat() : GameObject(
        std::make_unique<Util::Image>("C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/shaved_meat.png"), 3) {
        m_Transform.translation = glm::vec2(-335.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 250.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

    }

    void OnClick() {
        if (IsClicked()) {
            toppings.push_back(std::make_shared<Topping>(
                "C:/Shawarma/CHAO0312/Shawarma/Resources/Image/Food/shaved_meat.png"
            ));
        }
    }

private:
    std::vector<std::shared_ptr<Topping>> toppings;
};



class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };
    enum class phase {
        phase1,
        phase2,
        phase3,
    };
    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    phase m_CurrentPhase = phase::phase1;
    std::shared_ptr<BackgroundImage> m_Background;
    std::shared_ptr<Util::Renderer> m_Renderer;
    std::shared_ptr<StartButton> m_StartButton;
    std::shared_ptr<ShopButton> m_ShopButton;
    std::shared_ptr<ReturnButton> m_ReturnButton;
    //std::shared_ptr<ExitButton> m_ExitButton;

    //std::shared_ptr<Boss> m_Boss;
    std::shared_ptr<Meat> m_Meat;
    std::shared_ptr<Crust> m_Crust;
    std::shared_ptr<Knife> m_Knife;
    std::shared_ptr<Fries> m_Fries;
    std::shared_ptr<Sauce> m_Sauce;
    std::shared_ptr<Pickle> m_Pickle;
    std::shared_ptr<ShavedMeat> m_ShavedMeat;

    std::vector<std::shared_ptr<Topping>> toppings; // 儲存新增的配料

};

#endif

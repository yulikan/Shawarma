#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "Util/Renderer.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <unordered_set>
#include <vector>
#include <string>
#include "Roll.hpp"
#include "LevelManager.hpp"
#include "FryingCounterText.hpp"
#include "MoneyManager.hpp"
#include "NextButton.hpp"
#include "PatienceText.hpp"
#include "BeverageMachine.hpp"
#include "Cup.hpp"
//--------------------------------------
// 場景背景
//--------------------------------------
class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage()
        : GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/background/homePage.png"), 1) {
        m_Transform.scale = glm::vec2(0.65f, 0.65f);
    }
    explicit BackgroundImage(const std::string& imagePath)
    : GameObject(std::make_unique<Util::Image>(imagePath), 1) {
        if (imagePath.find("LevelPage1.png") != std::string::npos) {
            m_Transform.scale = glm::vec2(0.66f, 0.69f); // 🔽 調小這張圖的比例
        } else {
            m_Transform.scale = glm::vec2(1.8f, 1.85f); // 其他背景維持原本大小
        }
    }
};

//--------------------------------------
// 按鈕類別
//--------------------------------------
class StartButton : public Util::GameObject {
public:
    StartButton()
        : GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/startBtn1.png"), 5) {
        m_Transform.translation = glm::vec2(-328.0f, 45.0f);
        m_Transform.scale = glm::vec2(0.38f, 0.38f);
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
    }
};

class ShopButton : public Util::GameObject {
public:
    ShopButton()
        : GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/shopBtn.png"), 5) {
        m_Transform.translation = glm::vec2(-42.0f, -87.0f);
        m_Transform.scale = glm::vec2(0.35f, 0.35f);
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

class ReturnButton : public Util::GameObject {
public:
    ReturnButton()
        : GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/startBtn.png"), 5) {
        m_Transform.translation = glm::vec2(-400.0f, 300.0f);
        m_Transform.scale = glm::vec2(0.2f, 0.2f);
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

//--------------------------------------
// Topping 類別（用於各種食材）
//--------------------------------------
class Topping : public Util::GameObject {
public:
    // 建構子接受圖片路徑及食材類型識別字串
    Topping(const std::string& imagePath, const std::string& type)
        : Util::GameObject(std::make_unique<Util::Image>(imagePath), 4),
          m_Type(type), m_IsPlaced(false), m_IsDragging(false) {
        m_Transform.translation = glm::vec2(200.0f, -170.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }
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
    // 點擊後設定為已放置（例如放到料理上）
    void OnClick() {
        if (!m_IsPlaced && IsClicked()) {
            m_IsPlaced = true;
            m_Transform.translation = glm::vec2(0.0f, -210.0f);
        }
    }
    void Update() {
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
    const std::string& GetType() const { return m_Type; }
private:
    std::string m_Type;
    bool m_IsPlaced;
    bool m_IsDragging;
    glm::vec2 m_Offset;
};

//--------------------------------------
// 其他遊戲物件
//--------------------------------------
class Meat : public Util::GameObject {
public:
    Meat()
        : Util::GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/meat.png"), 2) {
        m_Transform.translation = glm::vec2(-390.0f, 90.0f);
        m_Transform.scale = glm::vec2(0.34f, 0.34f);
    }
};

class Paper : public Util::GameObject {
public:
    Paper()
        : Util::GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/Paper.png"), 2) {
        m_Transform.translation = glm::vec2(-155.0f, -225.0f);
        m_Transform.scale = glm::vec2(0.43f, 0.43f);
    }
};


class Crust : public Util::GameObject {
public:
    Crust()
        : Util::GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/crust.png"), 3),
          m_IsDragging(false) {
        m_Transform.translation = glm::vec2(0.0f, -210.0f);
        m_Transform.scale = glm::vec2(0.15f, 0.15f);
    }
    void Update() {
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

class Knife : public Util::GameObject {
public:
    Knife()
        : Util::GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/knife_origin.png"), 3),
          m_IsDragging(false) {
        m_Transform.translation = glm::vec2(-480.0f, -160.0f);
        m_Transform.scale = glm::vec2(0.2f, 0.2f);
    }
    void Update() {
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

class FrenchFries : public Util::GameObject {
public:
    FrenchFries()
        : Util::GameObject(
              std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/FrenchFries.png"),
              5), m_IsDragging(false) {
        m_Transform.translation = glm::vec2(100.0f, -100.0f); // 初始位置
        m_Transform.scale = glm::vec2(0.5f, 0.5f);             // 縮放大小
    }

    void Update() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed  = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
        bool mouseDown     = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        // 只有在目前沒有其他物件拖曳的情況下才允許開始拖曳
        if (mouseDown && IsClicked() && !g_IsObjectDragging && !m_IsDragging) {
            m_IsDragging = true;
            g_IsObjectDragging = true;
            m_Offset = m_Transform.translation - mousePos;
        }

        if (mousePressed && m_IsDragging && g_IsObjectDragging) {
            m_Transform.translation = mousePos + m_Offset;
        }

        if (mouseReleased && m_IsDragging && g_IsObjectDragging) {
            m_IsDragging = false;
            g_IsObjectDragging = false;
        }
    }

private:
    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 200.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

    bool m_IsDragging;
    glm::vec2 m_Offset;
};

//配料
// Fries 按鈕（配料按鈕）修改：移除 OnClick()，新增 IsPlaced() 與 SetPlaced()
class Fries : public Util::GameObject {
public:
    Fries()
        : Util::GameObject(
              std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/fries.png"),
              3),
          m_IsDragging(false),
          m_IsPlaced(false) {
        m_Transform.translation = glm::vec2(5.0f, -120.0f); // 初始位置
        m_Transform.scale = glm::vec2(0.5f, 0.5f);             // 縮放大小
    }

    bool IsClicked() {
        if (g_IsObjectDragging) return false;  // <<< 新增這行，避免誤觸
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

        float imageWidth = 200.0f * m_Transform.scale.x;
        float imageHeight = 200.0f * m_Transform.scale.y;

        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        return mousePressed &&
               mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
               mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
    }

    void Update() {
        // 若有拖曳邏輯可保留（本範例未改動）
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
        bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        if (mouseDown && IsClicked() && !m_IsDragging) {
            m_IsDragging = true;
            m_Offset = m_Transform.translation - mousePos;
        }
        if (mousePressed && m_IsDragging) {
            m_Transform.translation = mousePos + m_Offset;
        }
        if (mouseReleased && m_IsDragging) {
            m_IsDragging = false;
        }
    }

    bool IsPlaced() const { return m_IsPlaced; }
    void SetPlaced(bool placed) { m_IsPlaced = placed; }

private:
    bool m_IsDragging;
    bool m_IsPlaced;  // 紀錄是否已放置
    glm::vec2 m_Offset;
};

// 其他配料按鈕（Sauce、Pickle、ShavedMeat）做類似修改：
class Sauce : public Util::GameObject {
public:
    Sauce() : Util::GameObject(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/sauce.png"), 3),
        m_IsPlaced(false), m_Count(5)
    {
        m_Transform.translation = glm::vec2(-100.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);

        m_CountText = std::make_shared<Util::Text>(
            "C:/Windows/Fonts/arial.ttf", 24,
            std::to_string(m_Count),
            Util::Color::FromName(Util::Colors::WHITE)
        );
        m_CountGO = std::make_shared<Util::GameObject>(m_CountText, 4);
        m_CountGO->m_Transform.translation = glm::vec2(-100.0f, -160.0f);
        m_CountGO->m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void EnableLimit(bool enable) {
        m_LimitEnabled = enable;
    }

    bool IsClicked() {

        if (m_LimitEnabled && m_Count <= 0) return false;

        if (g_IsObjectDragging) return false;  // <<< 新增這行，避免誤觸
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

    void DecreaseCount() {
        if (m_LimitEnabled && m_Count > 0) {
            --m_Count;
            m_CountText->SetText(std::to_string(m_Count));
        }
    }

    void IncreaseCount() {
        if (m_LimitEnabled && m_Count < 5) {
            ++m_Count;
            m_CountText->SetText(std::to_string(m_Count));
        }
    }

    void ResetCount() {
        m_Count = 5;
        if (m_CountText) {
            m_CountText->SetText(std::to_string(m_Count));
        }
    }

    std::shared_ptr<Util::GameObject> GetCounterObject() const {
        return m_CountGO;
    }

    bool IsPlaced() const { return m_IsPlaced; }
    void SetPlaced(bool placed) { m_IsPlaced = placed; }

private:
    bool m_IsPlaced;
    int m_Count;
    bool m_LimitEnabled = false;
    std::shared_ptr<Util::Text> m_CountText;
    std::shared_ptr<Util::GameObject> m_CountGO;
};


class Pickle : public Util::GameObject {
public:
    Pickle() : Util::GameObject(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/pickle.png"), 3),
        m_IsPlaced(false),m_Count(5){  // 初始數量為 5 {
        m_Transform.translation = glm::vec2(-220.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);

        m_CountText = std::make_shared<Util::Text>(
            "C:/Windows/Fonts/arial.ttf", 24,
            std::to_string(m_Count),
            Util::Color::FromName(Util::Colors::WHITE)
        );
        m_CountGO = std::make_shared<Util::GameObject>(m_CountText, 4);
        m_CountGO->m_Transform.translation = glm::vec2(-220.0f, -160.0f); // 顯示在黃瓜下方
        m_CountGO->m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void EnableLimit(bool enable) {
        m_LimitEnabled = enable;
    }

    bool IsClicked() {
        if (m_LimitEnabled && m_Count <= 0) return false;

        if (g_IsObjectDragging) return false;  // <<< 新增這行，避免誤觸
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

    void DecreaseCount() {
        if (m_LimitEnabled && m_Count > 0) {
            --m_Count;
            m_CountText->SetText(std::to_string(m_Count));
        }
    }
    void IncreaseCount() {
        if (m_LimitEnabled && m_Count < 5) {
            ++m_Count;
            m_CountText->SetText(std::to_string(m_Count));
        }
    }

    void ResetCount() {
        m_Count = 5;
        if (m_CountText) {
            m_CountText->SetText(std::to_string(m_Count));
        }
    }

    std::shared_ptr<Util::GameObject> GetCounterObject() const {
        return m_CountGO;
    }


    bool IsPlaced() const { return m_IsPlaced; }
    void SetPlaced(bool placed) { m_IsPlaced = placed; }
private:
    bool m_IsPlaced;
    int m_Count;
    bool m_LimitEnabled = false;
    std::shared_ptr<Util::Text> m_CountText;
    std::shared_ptr<Util::GameObject> m_CountGO;
};

class ShavedMeat : public Util::GameObject {
public:
    ShavedMeat() : Util::GameObject(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/shaved_meat.png"), 3),
        m_IsPlaced(false) {
        m_Transform.translation = glm::vec2(-335.0f, -120.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }
    bool IsClicked() {
        if (g_IsObjectDragging) return false;  // <<< 新增這行，避免誤觸
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
    bool IsPlaced() const { return m_IsPlaced; }
    void SetPlaced(bool placed) { m_IsPlaced = placed; }
private:
    bool m_IsPlaced;
};

class Potato : public Util::GameObject {
public:
    Potato() : Util::GameObject(
        std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/potato.png"), 3),
        m_IsPlaced(false) {
        m_Transform.translation = glm::vec2(590.0f, -88.0f);
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
    bool IsPlaced() const { return m_IsPlaced; }
    void SetPlaced(bool placed) { m_IsPlaced = placed; }
private:
    bool m_IsPlaced;
};

//老闆配料補充
class CucumberHand : public Util::GameObject {
public:
    CucumberHand()
        : Util::GameObject(std::make_unique<Util::Image>(
            "C:/Users/yello/Shawarma/Resources/Image/Food/hand_cucumber.png"), 3)
    {
        m_Transform.translation = glm::vec2(-300.0f, 200.0f);  // 放在黃瓜上方
        m_Transform.scale = glm::vec2(1.5f, 1.5f);
    }

    bool IsClicked() {
        static bool wasPressedOnHand = false;

        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        float imageWidth = 100.0f * m_Transform.scale.x;
        float imageHeight = 100.0f * m_Transform.scale.y;
        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        bool inRange = mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
                       mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && inRange) {
            wasPressedOnHand = true;
        }

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            bool result = wasPressedOnHand && inRange;
            wasPressedOnHand = false;
            return result;
        }

        return false;
    }

};

class SauceHand : public Util::GameObject {
public:
    SauceHand()
        : Util::GameObject(std::make_unique<Util::Image>(
            "C:/Users/yello/Shawarma/Resources/Image/Food/hand_sauce.png"), 3)
    {
        m_Transform.translation = glm::vec2(-300.0f, 100.0f);  // 放在 sauce 上方
        m_Transform.scale = glm::vec2(1.5f, 1.5f);
    }

    bool IsClicked() {
        static bool wasPressedOnHand = false;

        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        float imageWidth = 100.0f * m_Transform.scale.x;
        float imageHeight = 100.0f * m_Transform.scale.y;
        glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
        glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

        bool inRange = mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
                       mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;

        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && inRange) {
            wasPressedOnHand = true;
        }

        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            bool result = wasPressedOnHand && inRange;
            wasPressedOnHand = false;
            return result;
        }

        return false;
    }

};



//--------------------------------------
// 客人類別（包含吃東西的狀態）
//--------------------------------------
class Customer : public Util::GameObject {
public:
    enum class EatState { NOT_EATEN, READY_TO_EAT, EATEN };

    Customer(const std::string& imagePath)
        : Util::GameObject(std::make_unique<Util::Image>(imagePath), 4),
          m_EatState(EatState::NOT_EATEN) {
    }

    void SetRequestedFood(const std::string& food) {
        m_RequestedFood = food;
    }
    const std::string& GetRequestedFood() const { return m_RequestedFood; }

    bool IsNearFrenchFries(const FrenchFries& fries) {
        float distance = glm::distance(m_Transform.translation, fries.GetTransform().translation);
        return distance < 50.0f;
    }
    void SetEatState(EatState state) { m_EatState = state; }
    EatState GetEatState() const { return m_EatState; }
    void RecordFood(const std::string& food) { m_EatenFoods.push_back(food); }
    const std::vector<std::string>& GetEatenFoods() const { return m_EatenFoods; }

    // 新增：設定和取得訂單 icon
    void SetOrderIcon(const std::shared_ptr<Util::GameObject>& orderIcon) { m_OrderIcon = orderIcon; }
    std::shared_ptr<Util::GameObject> GetOrderIcon() const { return m_OrderIcon; }
    // App.txt 中 Customer 類別新增
    void SetPatienceText(std::shared_ptr<PatienceText> pt) { m_PatienceText = pt; }
    std::shared_ptr<PatienceText> GetPatienceText() const { return m_PatienceText; }
    void SetRequiredToppings(const std::vector<std::string>& t) { m_RequiredToppings = t; }
    const std::vector<std::string>& GetRequiredToppings() const { return m_RequiredToppings; }
private:
    EatState m_EatState;
    std::vector<std::string> m_EatenFoods;
    std::string m_RequestedFood;
    // 新增成員：客人對應的訂單 icon
    std::shared_ptr<Util::GameObject> m_OrderIcon;
    std::shared_ptr<PatienceText> m_PatienceText;
    std::vector<std::string> m_RequiredToppings;

};

class PoorMan : public Util::GameObject {
public:
    enum class EatState { NOT_EATEN, READY_TO_EAT, EATEN };
    PoorMan()
        : Util::GameObject(std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Object/poorMan.png"), 2) {
        m_Transform.translation = glm::vec2(500.0f, -270.0f);
        m_Transform.scale = glm::vec2(0.47f, 0.47f);
    }
    bool IsNearFrenchFries(const FrenchFries& fries) {
        float distance = glm::distance(m_Transform.translation, fries.GetTransform().translation);
        return distance < 50.0f;
    }
    void SetEatState(EatState state) { m_EatState = state; }
    EatState GetEatState() const { return m_EatState; }
private:
    EatState m_EatState;
};

class Juice : public Util::GameObject {
public:
    Juice()
        : GameObject(
              std::make_unique<Util::Image>("C:/Users/yello/Shawarma/Resources/Image/Food/juice.png"),
              5), m_IsDragging(false) {
        m_Transform.translation = glm::vec2(-400.0f, -210.0f);
        m_Transform.scale = glm::vec2(0.5f, 0.5f);
    }

    void Update() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
        bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        if (mouseDown && IsClicked() && !g_IsObjectDragging && !m_IsDragging) {
            m_IsDragging = true;
            g_IsObjectDragging = true;
            m_Offset = m_Transform.translation - mousePos;
        }

        if (mousePressed && m_IsDragging) {
            m_Transform.translation = mousePos + m_Offset;
        }

        if (mouseReleased && m_IsDragging) {
            m_IsDragging = false;
            g_IsObjectDragging = false;
        }
    }

private:
    bool IsClicked() {
        glm::vec2 mousePos = Util::Input::GetCursorPosition();
        float w = 150.0f * m_Transform.scale.x;
        float h = 150.0f * m_Transform.scale.y;
        glm::vec2 min = m_Transform.translation - glm::vec2(w/2, h/2);
        glm::vec2 max = m_Transform.translation + glm::vec2(w/2, h/2);
        return Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) &&
               mousePos.x >= min.x && mousePos.x <= max.x &&
               mousePos.y >= min.y && mousePos.y <= max.y;
    }

    bool m_IsDragging;
    glm::vec2 m_Offset;
};


//--------------------------------------
// App 類別
//--------------------------------------
class App {
public:
    enum class State { START, UPDATE, LEVEL_END, END };
    enum class phase { phase1, phase2, phase3, levelComplete ,failed};
    State GetCurrentState() const { return m_CurrentState; }
    void Start();
    void Update();
    void End();
    void LoadLevel(const LevelData& level);

private:
    LevelManager m_LevelManager;
    void ValidTask();
    State m_CurrentState = State::START;
    phase m_CurrentPhase = phase::phase1;
    std::shared_ptr<BackgroundImage> m_Background;
    std::shared_ptr<Util::Renderer> m_Renderer;
    std::shared_ptr<StartButton> m_StartButton;
    std::shared_ptr<ShopButton> m_ShopButton;
    std::shared_ptr<ReturnButton> m_ReturnButton;
    std::shared_ptr<Meat> m_Meat;
    std::shared_ptr<Crust> m_Crust;
    std::shared_ptr<Knife> m_Knife;
    std::shared_ptr<Paper> m_Paper;
    std::shared_ptr<PoorMan>m_PoorMan;
    // 食材以 Topping 物件表示（使用不同 type 參數）
    std::shared_ptr<Fries> m_Fries;
    std::shared_ptr<Sauce> m_Sauce;
    std::shared_ptr<Pickle> m_Pickle;
    std::shared_ptr<ShavedMeat> m_ShavedMeat;
    std::shared_ptr<Potato> m_Potato;

    // 互動用的 FrenchFries 物件（例如用來檢查客人是否靠近）
    std::vector<std::shared_ptr<FrenchFries>> m_FrenchFriesList;
    std::vector<std::shared_ptr<Customer>> m_Customers;
    std::vector<std::shared_ptr<Topping>> toppings; // 其他新增的配料

    std::vector<std::shared_ptr<Roll>> m_Rolls;
    int m_Pstate = 2;  // 初始值為2：未放置
    int m_FryingCounter = 0;
    std::shared_ptr<Topping> m_Frying;  // 儲存frying topping物件指標

    bool m_EnableIngredientLimit = false;    // 原：第2關後才有數量限制
    bool m_EnableCustomTopping   = false;    // 新：第3關後才檢查客製化配料
    //第二關開始新增此功能
    int m_PickleRefillState = 2;
    int m_SauceRefillState = 2;

    bool m_IsPickleHandPressed = false;
    bool m_IsSauceHandPressed = false;

    std::shared_ptr<FryingCounterText>m_FryingCounterText;
    MoneyManager m_MoneyManager;
    std::shared_ptr<Util::Text> m_MoneyText;
    std::shared_ptr<Util::GameObject> m_MoneyTextGO;
    std::shared_ptr<Util::Text>       m_LevelText;      // 關卡文字
    std::shared_ptr<Util::GameObject> m_LevelTextGO;    // 包裝成 GameObject
    std::shared_ptr<Util::GameObject> m_LevelCompleteScreen;
    std::shared_ptr<NextButton> m_NextButton;
    std::shared_ptr<NextButton> m_RetryButton;
    std::shared_ptr<class CucumberHand> m_CucumberHand;
    std::shared_ptr<SauceHand> m_SauceHand;
    std::shared_ptr<BeverageMachine> m_BevMachine;
    std::vector<std::shared_ptr<Cup>> m_Cups;
    std::shared_ptr<Util::GameObject> m_JuicePack;
    std::shared_ptr<Juice> m_Juice;
    // 整關總客人數
    int m_TotalCustomersThisLevel = 0;
    // 耐心耗盡離開的人數
    int m_PatienceFailures = 0;
    // 失敗畫面
    std::shared_ptr<Util::GameObject> m_FailureScreen;
    // --- 輔助函式 ---
    bool IsCurrentLevelComplete() const;   // 判斷關卡是否完成
    void ResetLevelObjects();             // 切換關卡前重置場景
};

#endif

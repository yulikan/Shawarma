#include "Roll.hpp"
#include <iostream>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

// 判斷滑鼠點擊到
bool Roll::IsClicked() {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);

    float imageWidth = 1500.0f * m_Transform.scale.x;
    float imageHeight = 700.0f * m_Transform.scale.y;

    glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2.0f, imageHeight / 2.0f);
    glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2.0f, imageHeight / 2.0f);

    return mouseDown &&
           mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
           mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
}

void Roll::Update() {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
    bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

    // 當滑鼠按下、點在 roll 區域，且目前沒有其他物件正在拖曳時，開始拖曳
    if (mouseDown && IsClicked() && !g_IsObjectDragging && !m_IsDragging) {
        g_IsObjectDragging = true;
        m_IsDragging = true;
        m_DragOffset = m_Transform.translation - mousePos;
    }

    if (mousePressed && m_IsDragging && g_IsObjectDragging) {
        m_Transform.translation = mousePos + m_DragOffset;
    }

    if (mouseReleased && m_IsDragging && g_IsObjectDragging) {
        m_IsDragging = false;
        g_IsObjectDragging = false;
    }
}

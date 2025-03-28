#include "Roll.hpp"
#include <iostream>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

// 判斷滑鼠是否點擊到 roll
// ※這裡根據 roll.png 的尺寸與目前的縮放來設定點擊區域，數值可依實際情況調整
bool Roll::IsClicked() {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    // 使用 IsKeyDown 可持續偵測拖曳過程中的滑鼠按下狀態
    bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);

    // 假設原圖尺寸約為 300x300，依你目前縮放值 0.08，計算點擊區域寬高
    // 此處範例用 1500 與 700 的係數，你可以根據實際圖片做調整
    float imageWidth = 1500.0f * m_Transform.scale.x;  // 例如：1500 * 0.08 = 120
    float imageHeight = 700.0f * m_Transform.scale.y;    // 700 * 0.08 = 56

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
    // 拖曳中，持續更新 roll 位置
    if (mousePressed && m_IsDragging && g_IsObjectDragging) {
        m_Transform.translation = mousePos + m_DragOffset;
    }
    // 滑鼠放開後結束拖曳
    if (mouseReleased && m_IsDragging && g_IsObjectDragging) {
        m_IsDragging = false;
        g_IsObjectDragging = false;
    }
}

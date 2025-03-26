//
// Created by louise on 3/21/2025.
//
#include "Roll.hpp"

#include <App.hpp>
#include <iostream>

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

// 判斷滑鼠是否點擊到 roll，這裡假設 roll.png 的原始尺寸約為 300 x 300，根據實際圖檔做微調
bool Roll::IsClicked() {
    //std::cout << "click" << std::endl;
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    // 使用 IsKeyDown 而非 IsKeyPressed 來確保在拖曳期間也能偵測到滑鼠按下狀態
    bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);

    float imageWidth = 1500.0f * m_Transform.scale.x;
    float imageHeight = 700.0f * m_Transform.scale.y;

    glm::vec2 buttonMin = m_Transform.translation - glm::vec2(imageWidth / 2, imageHeight / 2);
    glm::vec2 buttonMax = m_Transform.translation + glm::vec2(imageWidth / 2, imageHeight / 2);

    return mouseDown &&
           mousePos.x >= buttonMin.x && mousePos.x <= buttonMax.x &&
           mousePos.y >= buttonMin.y && mousePos.y <= buttonMax.y;
}


void Roll::Update() {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool mousePressed = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
    bool mouseDown = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    bool mouseReleased = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

    // 當滑鼠按下且位於 roll 區域，且尚未處於拖曳狀態時，開始拖曳
    if (mouseDown && IsClicked() && !g_IsObjectDragging && !m_IsDragging) {
        //std::cout << "Dragging" << std::endl;
        g_IsObjectDragging = true;
        m_IsDragging = true;
        m_DragOffset = m_Transform.translation - mousePos;
    }
    // 拖曳中：使用 IsKeyPressed 來更新位置
    if (mousePressed && m_IsDragging && g_IsObjectDragging) {
        m_Transform.translation = mousePos + m_DragOffset;
    }
    // 滑鼠放開後停止拖曳
    if (mouseReleased && m_IsDragging && g_IsObjectDragging) {
        m_IsDragging = false;
        g_IsObjectDragging = false;
    }
}



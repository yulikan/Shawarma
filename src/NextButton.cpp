//
// Created by louise on 4/18/2025.
//
#include "NextButton.hpp"

NextButton::NextButton()
: Util::GameObject(
      std::make_unique<Util::Image>(
          "C:/Users/yello/Shawarma/Resources/Image/Object/nextLevelBtn.png"
      ),
      /*layer=*/8
  )
{
    m_Transform.translation = glm::vec2(0.0f, -150.0f);
    m_Transform.scale       = glm::vec2(0.3f, 0.3f);
}

bool NextButton::IsClicked() const {
    // 只有在滑鼠放開那一幀回傳 true
    if (!Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
        return false;

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    float w = 950.0f * m_Transform.scale.x;
    float h = 250.0f * m_Transform.scale.y;
    glm::vec2 min = m_Transform.translation - glm::vec2(w/2, h/2);
    glm::vec2 max = m_Transform.translation + glm::vec2(w/2, h/2);
    return mousePos.x >= min.x && mousePos.x <= max.x
        && mousePos.y >= min.y && mousePos.y <= max.y;
}

// NextButton.hpp
#ifndef NEXTBUTTON_HPP
#define NEXTBUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/vec2.hpp>

// 下一關按鈕
class NextButton : public Util::GameObject {
public:
    explicit NextButton(const std::string& imagePath =
            "C:/Shawarma/CHAO0609/Shawarma/Resources/Image/Object/nextLevelBtn.png",
        int layer = 8
    )
    : Util::GameObject(
          std::make_unique<Util::Image>(imagePath),
          layer
      )
    {
        m_Transform.translation = glm::vec2(0.0f, -150.0f);
        m_Transform.scale       = glm::vec2(0.3f, 0.3f);
    }

    bool IsClicked() const {
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
};

#endif // NEXTBUTTON_HPP
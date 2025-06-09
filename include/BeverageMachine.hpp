#ifndef BEVERAGEMACHINE_HPP
#define BEVERAGEMACHINE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/vec2.hpp>
#include "Cup.hpp"

// 飲料機
class BeverageMachine : public Util::GameObject {
public:
    BeverageMachine(const glm::vec2& pos,
                    const glm::vec2& scale = glm::vec2(1.0f),
                    int layer = 4)
      : GameObject(std::make_unique<Util::Image>(
            "C:/Shawarma/CHAO0609/Shawarma/Resources/Image/Food/machine.png"),
          layer)
      , m_Pos(pos)
      , m_Scale(scale)
      , m_BtnSize(80.0f, 80.0f)
      , m_SodaOffset(-50.0f, 0.0f)
      , m_ColaOffset( 50.0f, 0.0f)
    {
        m_Transform.translation = pos;
        m_Transform.scale       = scale;
    }

    bool IsButtonClicked(DrinkType type) const {
        glm::vec2 mouse = Util::Input::GetCursorPosition();
        bool up = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        glm::vec2 offset = (type==DrinkType::SODA ? m_SodaOffset : m_ColaOffset);
        glm::vec2 center = m_Pos + offset * m_Scale;
        glm::vec2 half   = m_BtnSize * 0.5f * m_Scale;

        glm::vec2 min = center - half;
        glm::vec2 max = center + half;

        return up
            && mouse.x >= min.x && mouse.x <= max.x
            && mouse.y >= min.y+50 && mouse.y <= max.y+50;
    }

    const glm::vec2& GetPosition() const { return m_Pos; }
    const glm::vec2& GetScale()    const { return m_Scale; }

private:
    glm::vec2 m_Pos;
    glm::vec2 m_Scale;
    glm::vec2 m_BtnSize;
    glm::vec2 m_SodaOffset;
    glm::vec2 m_ColaOffset;
};

#endif // BEVERAGEMACHINE_HPP

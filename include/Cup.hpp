#ifndef CUP_HPP
#define CUP_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/vec2.hpp>

enum class DrinkType { SODA, COLA };
enum class CupState  { FULL, EMPTY };

class Cup : public Util::GameObject {
public:
    Cup(DrinkType type,
        const glm::vec2& startPos,
        CupState initState = CupState::FULL)
      : Util::GameObject(nullptr, /*layer=*/5)
      , m_Type(type)
      , m_State(initState)
      , m_IsDragging(false)
    {
        ReloadSprite();
        m_Transform.translation = startPos;
        m_Transform.scale       = glm::vec2(0.4f);
    }

    void Update() {
        glm::vec2 mouse = Util::Input::GetCursorPosition();
        bool down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
        bool up   = Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);

        // 1) 按下开始拖动
        if (down && !m_IsDragging && IsMouseOver(mouse)) {
            m_IsDragging   = true;
            g_IsObjectDragging = true;
            m_DragOffset   = m_Transform.translation - mouse;
        }

        // 2) 拖动中
        if (m_IsDragging && !up) {
            m_Transform.translation = mouse + m_DragOffset;
        }

        // 3) 松开结束拖拽
        if (m_IsDragging && up) {
            m_IsDragging   = false;
            g_IsObjectDragging = false;
        }
    }

    void Serve() {
        if (m_State == CupState::FULL) {
            m_State = CupState::EMPTY;
            ReloadSprite();
        }
    }

    void Refill() {
        if (m_State == CupState::EMPTY) {
            m_State = CupState::FULL;
            ReloadSprite();
        }
    }

    CupState  GetState() const { return m_State; }
    DrinkType GetType()  const { return m_Type;  }
    void      SetPosition(const glm::vec2& pos) { m_Transform.translation = pos; }

private:
    void ReloadSprite() {
        std::string base = "C:/Shawarma/CHAO0603/Shawarma/Resources/Image/Food/";
        std::string path;
        if (m_State == CupState::FULL) {
            path = base + (m_Type==DrinkType::SODA
                           ? "cup_soda_full.png"
                           : "cup_cola_full.png");
        } else {
            path = base + "cup_empty.png";
        }
        SetDrawable(std::make_unique<Util::Image>(path));
    }

    bool IsMouseOver(const glm::vec2& mouse) const {
        glm::vec2 half = glm::vec2(50.0f) * m_Transform.scale;
        glm::vec2 min  = m_Transform.translation - half;
        glm::vec2 max  = m_Transform.translation + half;
        return mouse.x>=min.x && mouse.x<=max.x
            && mouse.y>=min.y && mouse.y<=max.y;
    }

    DrinkType      m_Type;
    CupState       m_State;
    bool           m_IsDragging;
    glm::vec2      m_DragOffset;
};

#endif // CUP_HPP

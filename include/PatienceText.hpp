#ifndef PATIENCETEXT_HPP
#define PATIENCETEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Time.hpp"

class PatienceText : public Util::GameObject {
public:
    PatienceText()
        : GameObject(
              std::make_unique<Util::Text>(
                  "C:/Windows/Fonts/arial.ttf", 24, "60",
                  Util::Color::FromName(Util::Colors::BLUE)),
              6),
          m_RemainingSeconds(60),
          m_AccumulatedMs(0.0f)
    {
        m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void SetPatience(int seconds) {
        m_RemainingSeconds = seconds;
        m_AccumulatedMs = 0.0f;
        UpdateText();
    }

    void Update() {
        m_AccumulatedMs += Util::Time::GetDeltaTimeMs();
        if (m_AccumulatedMs >= 1000.0f) {
            m_AccumulatedMs -= 1000.0f;
            m_RemainingSeconds = std::max(0, m_RemainingSeconds - 1);
            UpdateText();
        }
    }

    int GetRemaining() const { return m_RemainingSeconds; }

private:
    void UpdateText() {
        auto text = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        text->SetText(std::to_string(m_RemainingSeconds));
    }

    int m_RemainingSeconds;
    float m_AccumulatedMs;
};

#endif

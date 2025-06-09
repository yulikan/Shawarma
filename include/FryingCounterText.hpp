#ifndef FRYINGCOUNTERTEXT_HPP
#define FRYINGCOUNTERTEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

// 薯條機
class FryingCounterText : public Util::GameObject {
public:
    FryingCounterText()
        : GameObject(
              std::make_unique<Util::Text>("C:/Windows/Fonts/arial.ttf", 24,
                                           "0",
                                           Util::Color::FromName(Util::Colors::RED)),
              4) {
        m_Transform.translation = {490.0F, -120.0F};
        m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void UpdateCounter(int count) {
        auto text = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        text->SetText( std::to_string(count));
    }
};

#endif // FRYINGCOUNTERTEXT_HPP

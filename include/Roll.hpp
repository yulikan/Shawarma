#ifndef ROLL_H
#define ROLL_H

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "GlobalVars.hpp"

class Roll : public Util::GameObject {
public:
    Roll(const std::vector<std::string>& contents)
        : Util::GameObject(std::make_unique<Util::Image>(RESOURCE_DIR "/Image/Food/roll.png"), 5),
          m_Contents(contents),
          m_IsDragging(false) {
        m_Transform.translation = glm::vec2(0.0f, 0.0f);
        m_Transform.scale = glm::vec2(0.08f, 0.08f);
    }

    // 取得 roll 內的配料
    const std::vector<std::string>& GetContents() const { return m_Contents; }

    void Update();

private:
    // 判斷點擊
    bool IsClicked();

    std::vector<std::string> m_Contents;
    bool m_IsDragging;
    glm::vec2 m_DragOffset;
};

#endif // ROLL_H

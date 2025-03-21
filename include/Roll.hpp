//
// Created by louise on 3/21/2025.
//

#ifndef ROLL_H
#define ROLL_H

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <string>

class Roll : public Util::GameObject {
public:
    // 建構子接受配料內容列表
    Roll(const std::vector<std::string>& contents)
        : Util::GameObject(std::make_unique<Util::Image>("C:/Shawarma/CHAO0320/Shawarma/Resources/Image/Food/roll.png"), 5),
          m_Contents(contents) {
        // 設定 roll 的位置與縮放
        m_Transform.translation = glm::vec2(350.0f, -170.0f);

        m_Transform.scale = glm::vec2(0.1f, 0.1f);
    }

    // 取得 roll 內的配料
    const std::vector<std::string>& GetContents() const { return m_Contents; }

private:
    std::vector<std::string> m_Contents; // 儲存所有配料的 type
};

#endif //ROLL_H

#ifndef ROLL_H
#define ROLL_H

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "GlobalVars.hpp"  // 使用共用的全域變數

class Roll : public Util::GameObject {
public:
    // 建構子接受配料內容列表
    Roll(const std::vector<std::string>& contents)
        : Util::GameObject(std::make_unique<Util::Image>("C:/Shawarma/CHAO0606/Shawarma/Resources/Image/Food/roll.png"), 5),
          m_Contents(contents),
          m_IsDragging(false) {
        // 設定 roll 的初始位置與縮放（根據需求調整）
        m_Transform.translation = glm::vec2(0.0f, 0.0f);
        m_Transform.scale = glm::vec2(0.08f, 0.08f);
    }

    // 取得 roll 內的配料
    const std::vector<std::string>& GetContents() const { return m_Contents; }

    // 每幀更新 roll 狀態（包含拖曳邏輯）
    void Update();

private:
    // 判斷滑鼠是否點擊到 roll
    bool IsClicked();

    std::vector<std::string> m_Contents; // 儲存所有配料的 type
    bool m_IsDragging;                   // 拖曳旗標
    glm::vec2 m_DragOffset;              // 點擊時，roll 中心與滑鼠的相對位移
};

#endif // ROLL_H

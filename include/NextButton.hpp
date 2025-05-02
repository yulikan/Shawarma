
#ifndef NEXTBUTTON_HPP
#define NEXTBUTTON_HPP
#pragma once

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <glm/vec2.hpp>

class NextButton : public Util::GameObject {
public:
    NextButton();

    // 一定要加 const，並且 public，才能在 App 裡呼叫
    bool IsClicked() const;
};

#endif //NEXTBUTTON_HPP

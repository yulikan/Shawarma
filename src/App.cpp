#include "App.hpp"
#include <iostream>
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include "Util/Renderer.hpp"
#include <algorithm> // for std::find
#include "Roll.hpp"
#include "NextButton.hpp"
#include "PatienceText.hpp"
#include "Util/BGM.hpp"
#include <SDL_mixer.h>

void App::Start() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    // bgm設定
    m_BGM.LoadMedia(RESOURCE_DIR"/music/bgm.mp3");
    m_BGM.Play(-1);
    m_BGM.SetVolume(10);

    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    m_Renderer = std::make_shared<Util::Renderer>();

    m_IgnoreNextMouseUp = false;

    m_Background = std::make_shared<BackgroundImage>();
    m_StartButton = std::make_shared<StartButton>();
    m_ShopButton  = std::make_shared<ShopButton>();
    m_ReturnButton = std::make_shared<ReturnButton>();
    m_Meat        = std::make_shared<Meat>();
    m_Crust       = std::make_shared<Crust>();
    m_Knife       = std::make_shared<Knife>();
    m_Paper       = std::make_shared<Paper>();
    m_CurrentLevelPage = "LevelPage1";

    m_LevelCompleteScreen = std::make_shared<Util::GameObject>(
        std::make_unique<Util::Image>(
            RESOURCE_DIR "/Image/background/nextLevel.png"),
        /*layer=*/7
    );
    m_LevelCompleteScreen->m_Transform.translation = glm::vec2(0.0f, 0.0f);
    m_LevelCompleteScreen->m_Transform.scale       = glm::vec2(0.7f, 0.7f);
    m_NextButton = std::make_shared<NextButton>();
    m_PoorMan = std::make_shared<PoorMan>();

    // Toppings
    m_Fries       = std::make_shared<Fries>();
    m_Pickle      = std::make_shared<Pickle>();
    m_Sauce       = std::make_shared<Sauce>();
    m_ShavedMeat  = std::make_shared<ShavedMeat>();

    m_Potato      = std::make_shared<Potato>();
    m_FryingCounterText = std::make_shared<FryingCounterText>();

    // Money text
    m_MoneyManager = MoneyManager(0);
    auto moneyTextDrawable = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/msyh.ttc", 30, "$0", Util::Color{255,255,255}
    );
    m_MoneyTextGO = std::make_shared<Util::GameObject>(moneyTextDrawable, 5);
    m_MoneyTextGO->m_Transform.translation = glm::vec2(460.0f, 325.0f);
    m_MoneyText = moneyTextDrawable;
    m_MoneyManager.SetOnChangeCallback([this](int newBal){
        m_MoneyText->SetText("$" + std::to_string(newBal));
    });

    // 關卡數顯示
    m_LevelText = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/msyh.ttc",
        48,
        "00",
        Util::Color{0,0,0}
    );
    m_LevelTextGO = std::make_shared<Util::GameObject>(m_LevelText, 8);
    m_LevelTextGO->m_Transform.translation = glm::vec2(-5.0f, -45.0f);
    m_DayText = std::make_shared<Util::Text>(
        "C:/Windows/Fonts/arial.ttf", 26,
        "1",
        Util::Color::FromName(Util::Colors::BLACK)
    );
    m_DayTextGO = std::make_shared<Util::GameObject>(m_DayText, 8);
    m_DayTextGO->m_Transform.translation = glm::vec2(-575.0f, 275.0f);

    // 總金額
    m_CompleteMoneyText = std::make_shared<Util::Text>(
    "C:/Windows/Fonts/msyh.ttc",
    48,
    "0",
    Util::Color{0,0,0}
);
    m_CompleteMoneyTextGO = std::make_shared<Util::GameObject>(m_CompleteMoneyText, 8);
    m_CompleteMoneyTextGO->m_Transform.translation = glm::vec2(
        m_LevelTextGO->m_Transform.translation.x + 50.0f,
        m_LevelTextGO->m_Transform.translation.y + 90.0f
    );

    // 初始畫面
    m_Renderer->AddChild(m_StartButton);
    m_Renderer->AddChild(m_ShopButton);
    m_Renderer->AddChild(m_Background);

}

void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit())
        m_CurrentState = State::END;

    // 過關
 if (m_CurrentPhase == phase::levelComplete) {
        // 移除左上角的小小天數
        m_Renderer->RemoveChild(m_DayTextGO);

        // 顯示第 N 關文字
        int currentIdx = static_cast<int>(m_LevelManager.GetCurrentLevelIndex());
        int levelNum   = currentIdx + 1;
        m_LevelText->SetText(std::to_string(levelNum));
        m_Renderer->AddChild(m_LevelTextGO);

        // 顯示結算金額文字
        int currentBalance = m_MoneyManager.GetBalance();
        m_CompleteMoneyText->SetText("$" + std::to_string(currentBalance));
        m_Renderer->AddChild(m_CompleteMoneyTextGO);

        // 檢查是不是第 30 關
        int lastIdx = m_LevelManager.GetTotalLevelCount() - 1;
        if (currentIdx < lastIdx) {
            // 不是最後一關，就維持原本下一關按鈕
            m_Renderer->AddChild(m_LevelCompleteScreen);
            m_Renderer->AddChild(m_NextButton);

            if (m_NextButton->IsClicked()) {
                // 移除結算畫面與按鈕，進下一關
                m_Renderer->RemoveChild(m_LevelTextGO);
                m_Renderer->RemoveChild(m_CompleteMoneyTextGO);
                m_Renderer->RemoveChild(m_LevelCompleteScreen);
                m_Renderer->RemoveChild(m_NextButton);

                m_LevelManager.NextLevel();
                LoadLevel(m_LevelManager.GetCurrentLevel());
                m_CurrentPhase = phase::phase2;
            }
        }
        else {
            // 第 30 關，顯示勝利畫面 (winPage) 及兩個按鈕
            m_Renderer->RemoveChild(m_LevelTextGO);
            m_Renderer->RemoveChild(m_CompleteMoneyTextGO);
            m_Renderer->RemoveChild(m_LevelCompleteScreen);
            m_Renderer->RemoveChild(m_NextButton);

            // winPage 與按鈕
            if (!m_WinScreen) {
                m_WinScreen = std::make_shared<Util::GameObject>(
                    std::make_unique<Util::Image>(RESOURCE_DIR "/Image/background/winPage.png"),
                    /*layer=*/7
                );
                m_WinScreen->m_Transform.translation = glm::vec2(0.0f, 0.0f);
                m_WinScreen->m_Transform.scale       = glm::vec2(1.1f, 1.1f);

                // 重新開始按鈕
                m_RestartButton = std::make_shared<NextButton>(
                    RESOURCE_DIR "/Image/Object/restartBtn.png", /*layer=*/8
                );
                m_RestartButton->m_Transform.translation = glm::vec2(-200.0f, -50.0f);
                m_RestartButton->m_Transform.scale       = glm::vec2(0.25f, 0.25f);

                // 離開遊戲
                m_ExitButton = std::make_shared<NextButton>(
                    RESOURCE_DIR "/Image/Object/exitBtn.png", /*layer=*/8
                );
                m_ExitButton->m_Transform.translation = glm::vec2(200.0f, -50.0f);
                m_ExitButton->m_Transform.scale       = glm::vec2(0.25f, 0.25f);

                // winPage 與按鈕
                m_Renderer->AddChild(m_WinScreen);
                m_Renderer->AddChild(m_RestartButton);
                m_Renderer->AddChild(m_ExitButton);
            }

            // 重新開始
            if (m_RestartButton->IsClicked()) {
                m_Renderer->RemoveChild(m_WinScreen);
                m_Renderer->RemoveChild(m_RestartButton);
                m_Renderer->RemoveChild(m_ExitButton);

                // 重置到第 1 關
                m_LevelManager.Reset();
                LoadLevel(m_LevelManager.GetCurrentLevel());
                m_CurrentPhase = phase::phase2;
                m_WinScreen.reset();
                m_RestartButton.reset();
                m_ExitButton.reset();
            }
            // 離開遊戲
            else if (m_ExitButton->IsClicked()) {
                m_CurrentState = State::END;
            }
        }
        if (m_Renderer) m_Renderer->Update();
        return;
    }

     int currentLevelIndex = m_LevelManager.GetCurrentLevelIndex();
        const LevelData& levelData = m_LevelManager.GetLevels()[currentLevelIndex];
        int threshold = levelData.passThreshold;

    //關卡場景
    if (m_CurrentPhase == phase::phase2
         && m_LevelManager.IsLevelFinished()
         && m_Customers.empty())
    {
        // 依照金額判斷過關
        if (m_MoneyManager.GetBalance() >= threshold) {
            // 過關：顯示通關畫面
            m_Renderer->AddChild(m_LevelCompleteScreen);
            m_Renderer->AddChild(m_NextButton);
            m_CurrentPhase = phase::levelComplete;
        }
        else {
            // 失敗：顯示失敗畫面與重試按鈕
            m_Renderer->RemoveChild(m_DayTextGO);
            int currentIdx = static_cast<int>(m_LevelManager.GetCurrentLevelIndex()); // 0-based
            int levelNum   = currentIdx + 1;
            m_LevelText->SetText(std::to_string(levelNum));
            m_Renderer->AddChild(m_LevelTextGO);
            int currentBalance = m_MoneyManager.GetBalance();
            m_CompleteMoneyText->SetText("$" + std::to_string(currentBalance));
            m_Renderer->AddChild(m_CompleteMoneyTextGO);
            m_FailureScreen = std::make_shared<Util::GameObject>(
                std::make_unique<Util::Image>(
                    RESOURCE_DIR "/Image/background/FailPage.png"
                ), 7
            );
            m_FailureScreen->m_Transform.translation = {0,0};
            m_FailureScreen->m_Transform.scale       = {0.7f,0.7f};
            m_Renderer->AddChild(m_FailureScreen);

            m_RetryButton = std::make_shared<NextButton>(
                RESOURCE_DIR "/Image/Object/retryBtn.png"
            );
            m_Renderer->AddChild(m_RetryButton);

            m_CurrentPhase = phase::failed;
        }
        return;
    }

    // 開始遊戲按鈕
    if (m_StartButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase2;
        LoadLevel(m_LevelManager.GetCurrentLevel());
    }

    // 作弊模式
if (m_CurrentPhase == phase::phase3) {
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    if (m_IgnoreNextMouseUp) {
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            m_IgnoreNextMouseUp = false;
        }
        return;
    }
    // 作弊模式跳下一頁
    if (Util::Input::IsKeyUp(Util::Keycode::N) && m_CurrentLevelPage == "LevelPage1") {
        m_CurrentLevelPage = "LevelPage2";
        m_Background = std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/Image/background/LevelPage2.png");
        m_Renderer = std::make_shared<Util::Renderer>(
            std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }

    // 作弊模式跳上一頁
    if (Util::Input::IsKeyUp(Util::Keycode::P) && m_CurrentLevelPage == "LevelPage2") {
        m_CurrentLevelPage = "LevelPage1";
        m_Background = std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/Image/background/LevelPage1.png");
        m_Renderer = std::make_shared<Util::Renderer>(
            std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
    }

    // 跳關的小按鈕位置設定
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        if (m_CurrentLevelPage == "LevelPage1") {
            glm::vec2 levelSize(230.0f, 200.0f);
            std::vector<glm::vec2> levelCenters = {
                {-500.0f, 235.0f}, {-250.0f, 235.0f}, {0.0f, 235.0f},
{250.0f, 235.0f},  {500.0f, 235.0f},{-500.0f, 0.0f}, {-250.0f, 0.0f}, {0.0f, 0.0f},
{250.0f, 0.0f},  {500.0f, 0.0f},{-500.0f, -235.0f}, {-250.0f, -235.0f}, {0.0f, -235.0f},
{250.0f, -235.0f},  {500.0f, -235.0f},
            };

            for (size_t i = 0; i < levelCenters.size(); ++i) {
                glm::vec2 min = levelCenters[i] - levelSize * 0.5f;
                glm::vec2 max = levelCenters[i] + levelSize * 0.5f;

                if (mousePos.x >= min.x && mousePos.x <= max.x &&
                    mousePos.y >= min.y && mousePos.y <= max.y)
                {
                    size_t levelIndex = i;  // 第1～5關
                    if (levelIndex >= m_LevelManager.GetTotalLevelCount()) {
                        std::cout << "尚未實作關卡 index = " << levelIndex << std::endl;
                        continue;
                    }
                    m_LevelManager.SetLevelIndex(levelIndex);
                    LoadLevel(m_LevelManager.GetCurrentLevel());
                    m_CurrentPhase = phase::phase2;
                    return;
                }
            }
        }

        if (m_CurrentLevelPage == "LevelPage2") {
            glm::vec2 levelSize(230.0f, 200.0f);
            std::vector<glm::vec2> levelCenters = {
                {-500.0f, 235.0f}, {-250.0f, 235.0f}, {0.0f, 235.0f},
{250.0f, 235.0f},  {500.0f, 235.0f},{-500.0f, 0.0f}, {-250.0f, 0.0f}, {0.0f, 0.0f},
{250.0f, 0.0f},  {500.0f, 0.0f},{-500.0f, -235.0f}, {-250.0f, -235.0f}, {0.0f, -235.0f},
{250.0f, -235.0f},  {500.0f, -235.0f},
            };

            for (size_t i = 0; i < levelCenters.size(); ++i) {
                glm::vec2 min = levelCenters[i] - levelSize * 0.5f;
                glm::vec2 max = levelCenters[i] + levelSize * 0.5f;

                if (mousePos.x >= min.x && mousePos.x <= max.x &&
                    mousePos.y >= min.y && mousePos.y <= max.y)
                {
                    size_t levelIndex = i+15;  // 第16～30關
                    if (levelIndex >= m_LevelManager.GetTotalLevelCount()) {
                        std::cout << "尚未實作關卡 index = " << levelIndex << std::endl;
                        continue;
                    }
                    m_LevelManager.SetLevelIndex(levelIndex);
                    LoadLevel(m_LevelManager.GetCurrentLevel());
                    m_CurrentPhase = phase::phase2;
                    return;
                }
            }
        }
    }

    if (m_Renderer) m_Renderer->Update();
    return;
}

    // 遊戲畫面
    if (m_CurrentPhase == phase::phase2) {

    // 耐心值
    for (auto it = m_Customers.begin(); it != m_Customers.end();) {
        auto& customer = *it;
        if (m_EnablePatience) {
            if (auto pt = customer->GetPatienceText()) {
                pt->Update();
                if (pt->GetRemaining() <= 0) {
                    m_Renderer->RemoveChild(pt);
                    customer->SetPatienceText(nullptr);
                    if (customer->GetOrderIcon())
                        m_Renderer->RemoveChild(customer->GetOrderIcon());
                    m_Renderer->RemoveChild(customer);

                    it = m_Customers.erase(it);
                    continue;
                }
            }
        }

        // 檢查是否吃完
        const auto& eaten = customer->GetEatenFoods();
        const auto& requested = customer->GetRequestedFood();
        if (std::find(eaten.begin(), eaten.end(), requested) != eaten.end()) {
            // 吃完算錢
            if (requested == "FrenchFries") m_MoneyManager.Add(30);
            else if (requested == "Juice")       m_MoneyManager.Add(15);

            // 吃完要移除：客人、訂單、耐心值
            if (auto pt2 = customer->GetPatienceText()) {
                m_Renderer->RemoveChild(pt2);
                customer->SetPatienceText(nullptr);
            }
            if (customer->GetOrderIcon())
                m_Renderer->RemoveChild(customer->GetOrderIcon());
            m_Renderer->RemoveChild(customer);
            it = m_Customers.erase(it);
            continue;
        }
        ++it;
    }

// 新客人生成
        std::array<bool, 3> occupied = { false, false, false };
    for (auto& customer : m_Customers) {
        glm::vec2 pos = customer->m_Transform.translation;
        for (size_t i = 0; i < kCustomerSlots.size(); ++i) {
            if (pos == kCustomerSlots[i]) {
                occupied[i] = true;
                break;
            }
        }
    }

// 新客人生成在指定生成區(補上剛剛離開的人的位子)
    auto newCfgs = m_LevelManager.SpawnCustomers((int)m_Customers.size());
    for (auto& cfg : newCfgs) {
        size_t slotIndex = 0;
        while (slotIndex < occupied.size() && occupied[slotIndex]) {
            ++slotIndex;
        }
        if (slotIndex >= occupied.size()) {
            slotIndex = occupied.size() - 1;
        }
        occupied[slotIndex] = true;

        auto customer = std::make_shared<Customer>(cfg.customerImage);
        customer->m_Transform.translation = kCustomerSlots[slotIndex];
        customer->m_Transform.scale       = glm::vec2(0.5f, 0.5f);
        customer->SetRequestedFood(cfg.foodRequest);
        glm::vec2 custPos = customer->m_Transform.translation;

        // Food Icon
        auto foodIcon = std::make_shared<Util::GameObject>(
            std::make_unique<Util::Image>(cfg.foodIcon), 5);
        if (cfg.foodRequest == "Roll") {
            if (m_EnableCustomTopping) {
                customer->SetRequiredToppings(cfg.requiredToppings);
            } else {
                customer->SetRequiredToppings({});
            }
            foodIcon->m_Transform.scale       = glm::vec2(0.08f, 0.08f);
            foodIcon->m_Transform.translation = custPos + glm::vec2(0.0f, -55.0f);
        } else {
            foodIcon->m_Transform.scale       = glm::vec2(0.3f, 0.3f);
            foodIcon->m_Transform.translation = custPos + glm::vec2(0.0f, -60.0f);
        }
        customer->SetOrderIcon(foodIcon);

        // Patience Text
        auto patienceText = std::make_shared<PatienceText>();
        patienceText->SetPatience(30);
        patienceText->m_Transform.translation = custPos + glm::vec2(0.0f, -100.0f);
        customer->SetPatienceText(patienceText);

        // 把客人、FoodIcon、PatienceText 加到場景中
        m_Customers.push_back(customer);
        m_Renderer->AddChild(customer);
        m_Renderer->AddChild(foodIcon);
        if (m_EnablePatience) {
            m_Renderer->AddChild(patienceText);
        }
    }
        // 配料按鈕
        // Fries button
        if (!m_Fries->IsPlaced() && m_Fries->IsClicked()) {
            m_Fries->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                RESOURCE_DIR "/Image/Food/topping_fries.png",
                "fries"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -214.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Sauce button
        if (!m_Sauce->IsPlaced() && m_Sauce->IsClicked()) {
            m_Sauce->SetPlaced(true);
            m_Sauce->DecreaseCount();
            auto newTopping = std::make_shared<Topping>(
                RESOURCE_DIR "/Image/Food/topping_sauce.png",
                "sauce"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -248.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Pickle button
        if (!m_Pickle->IsPlaced() && m_Pickle->IsClicked()) {
            m_Pickle->SetPlaced(true);
            m_Pickle->DecreaseCount();
            auto newTopping = std::make_shared<Topping>(
                RESOURCE_DIR "/Image/Food/topping_pickle.png",
                "pickle"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -195.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // pickle 補充
        if (m_CucumberHand && m_EnableIngredientLimit && m_Pickle) {
            if (!m_IsPickleHandPressed && m_CucumberHand->IsClicked()) {
                m_IsPickleHandPressed = true;
            }
            if (m_IsPickleHandPressed && !m_CucumberHand->IsClicked() &&
                Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
                m_IsPickleHandPressed = false;
                m_Pickle->IncreaseCount();
                }
        }

        // sauce 補充
        if (m_SauceHand && m_EnableIngredientLimit && m_Sauce) {
            if (!m_IsSauceHandPressed && m_SauceHand->IsClicked()) {
                m_IsSauceHandPressed = true;
            }
            if (m_IsSauceHandPressed && !m_SauceHand->IsClicked() &&
                Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
                m_IsSauceHandPressed = false;
                m_Sauce->IncreaseCount();
                }
        }


        // ShavedMeat button
        if (!m_ShavedMeat->IsPlaced() && m_ShavedMeat->IsClicked()) {
            m_ShavedMeat->SetPlaced(true);
            auto newTopping = std::make_shared<Topping>(
                RESOURCE_DIR "/Image/Food/topping_meat.png",
                "shaved_meat"
            );
            newTopping->m_Transform.translation = glm::vec2(0.0f, -232.0f);
            m_Renderer->AddChild(newTopping);
            toppings.push_back(newTopping);
        }

        // Potato & frying logic
        if (!m_Potato->IsPlaced() && m_Potato->IsClicked()) {
            m_Potato->SetPlaced(true);
            m_Frying = std::make_shared<Topping>(
                RESOURCE_DIR "/Image/Food/frying.png",
                "potato"
            );
            m_Frying->m_Transform.translation = glm::vec2(480.0f, -40.0f);
            m_Renderer->AddChild(m_Frying);
            m_Pstate = 0;
        }
        if (m_Pstate == 0 && m_Frying) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) &&
                mousePos.x >= 400 && mousePos.x <= 550 &&
                mousePos.y >= -100 && mousePos.y <= 0
            ) {
                m_Pstate = 1;
            }
            if (m_Pstate == 1) {
                m_Pstate = 0;
                m_FryingCounter++;
                if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(m_FryingCounter);
                std::cout << "Frying topping clicked, count: " << m_FryingCounter << std::endl;
            }
        }


//飲料杯邏輯
for (auto& cup : m_Cups)
    cup->Update();
bool served = false;
for (auto& cust : m_Customers) {
    glm::vec2 custPos = cust->m_Transform.translation;

    for (size_t i = 0; i < m_Cups.size(); ++i) {
        auto& cup = m_Cups[i];

        // 杯子 full 才能喝
        if (cup->GetState() == CupState::FULL
            && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
        {
            float dist = glm::distance(custPos, cup->m_Transform.translation);
            float interactRadius = 50.0f * cup->m_Transform.scale.x;

            if (dist < interactRadius) {
                // 符合訂單+錢
                std::string req = cust->GetRequestedFood();
                if ((cup->GetType() == DrinkType::SODA && req == "Soda") ||
                    (cup->GetType() == DrinkType::COLA && req == "Cola"))
                {
                    if (cup->GetType() == DrinkType::SODA) {
                        m_MoneyManager.Add(20);
                        cust->RecordFood("Soda");
                    }
                    else {
                        m_MoneyManager.Add(25);
                        cust->RecordFood("Cola");
                    }
                }

                // 喝掉變成 empty
                DrinkType servedType = cup->GetType();
                m_Renderer->RemoveChild(cup);
                m_Cups.erase(m_Cups.begin() + i);

                glm::vec2 offset = (servedType == DrinkType::SODA
                    ? glm::vec2(-50, -50)
                    : glm::vec2( 50, -50));
                auto emptyCup = std::make_shared<Cup>(
                    servedType,
                    m_BevMachine->GetPosition() + offset * m_BevMachine->GetScale(),
                    CupState::EMPTY);
                m_Cups.push_back(emptyCup);
                m_Renderer->AddChild(emptyCup);

                served = true;
                break;
            }
        }
    }

    if (served) {
        served = false;
        break;
    }
}


// 補充飲料到杯子
if (m_BevMachine->IsButtonClicked(DrinkType::SODA)) {
    for (auto& cup : m_Cups) {
        if (cup->GetType() == DrinkType::SODA
         && cup->GetState() == CupState::EMPTY)
        {
            cup->Refill();
            cup->SetPosition(
                m_BevMachine->GetPosition()
                + glm::vec2(-50, -50) * m_BevMachine->GetScale());
            break;
        }
    }
}
if (m_BevMachine->IsButtonClicked(DrinkType::COLA)) {
    for (auto& cup : m_Cups) {
        if (cup->GetType() == DrinkType::COLA
         && cup->GetState() == CupState::EMPTY)
        {
            cup->Refill();
            cup->SetPosition(
                m_BevMachine->GetPosition()
                + glm::vec2(50, -50) * m_BevMachine->GetScale());
            break;
        }
    }
}



        // Customer FrenchFries interaction
        for (auto& customer : m_Customers) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end();) {
                auto& friesObj = *it;
                if (customer->IsNearFrenchFries(*friesObj)) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        customer->GetEatState() == Customer::EatState::READY_TO_EAT
                    ) {
                        customer->SetEatState(Customer::EatState::EATEN);
                        customer->RecordFood("FrenchFries");
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (customer->GetEatState() != Customer::EatState::EATEN)
                        customer->SetEatState(Customer::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        // Refill fries when counter hits 5
        if (m_FryingCounter == 5) {
            auto newFries = std::make_shared<FrenchFries>();
            newFries->m_Transform.translation = glm::vec2(100.0f + m_FrenchFriesList.size()*60, -50.0f);
            m_FrenchFriesList.push_back(newFries);
            m_Renderer->AddChild(newFries);
            if (m_Frying) {
                m_Renderer->RemoveChild(m_Frying);
                m_Frying.reset();
            }
            m_FryingCounter = 0;
            if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(0);
            m_Potato->SetPlaced(false);
            m_Pstate = 2;
        }
        if (Util::Input::IsKeyUp(Util::Keycode::R)) {
            // 檢查目前畫面上的 roll 數量是否小於 3
            if (m_Rolls.size() < 3) {
                // 儲存烤餅上的配料
                std::vector<std::string> rollContents;
                std::cout << "Roll contents:" << std::endl;
                for (auto& topping : toppings) {
                    std::string type = topping->GetType();
                    rollContents.push_back(type);
                    std::cout << type << std::endl;
                    m_Renderer->RemoveChild(topping);
                }
                toppings.clear();

                // 重置所有配料按鈕狀態
                m_Fries->SetPlaced(false);
                m_Sauce->SetPlaced(false);
                m_Pickle->SetPlaced(false);
                m_ShavedMeat->SetPlaced(false);
                m_CurrentDay = m_LevelManager.GetCurrentLevelNumber();
                if (m_DayText) {
                    m_DayText->SetText(std::to_string(m_CurrentDay));
                }
                m_Renderer->AddChild(m_DayTextGO);

                auto roll = std::make_shared<Roll>(rollContents);
                float baseX   = 145.0f;
                float baseY   = -130.0f;
                float offsetY = -40.0f * static_cast<float>(m_Rolls.size());
                roll->m_Transform.translation = glm::vec2(baseX, baseY + offsetY);
                m_Rolls.push_back(roll);
                m_Renderer->AddChild(roll);
            } else {
                std::cout << "no more than 3 roll" << std::endl;
            }

        }

for (auto& customer : m_Customers) {
    if (customer->GetEatState() == Customer::EatState::READY_TO_EAT &&
        Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
    {
        int lvl = m_LevelManager.GetCurrentLevelIndex() + 1;  // 1-based

        for (auto it = m_Rolls.begin(); it != m_Rolls.end(); ++it) {
            auto& rollObj = *it;
            float distance = glm::distance(
                customer->m_Transform.translation,
                rollObj->m_Transform.translation
            );
            if (distance < 50.0f) {
                bool correctOrder = false;
                int price = 0;

                auto actual = rollObj->GetContents(); // vector<string>

                if (lvl >= 25) {
                    // 客製化關卡
                    const auto& required = customer->GetRequiredToppings();
                    if (!required.empty()) {
                        auto req = required;
                        std::sort(req.begin(), req.end());
                        std::sort(actual.begin(), actual.end());
                        if (actual == req) {
                            correctOrder = true;
                            price = m_LevelManager.CalcRollPrice(req);
                        }
                    }
                } else {
                    correctOrder = true;
                    int toppingCount = static_cast<int>(actual.size());
                    price = 10 + toppingCount * 10;
                    if (price > 50) price = 50;
                }

                if (correctOrder) {
                    m_MoneyManager.Add(price);
                    customer->RecordFood("Roll");
                    for (auto& ing : actual) {
                        customer->RecordFood(ing);
                    }
                    customer->SetEatState(Customer::EatState::EATEN);
                    m_Renderer->RemoveChild(rollObj);
                    m_Rolls.erase(it);
                    g_IsObjectDragging = false;
                }

                break;
            }
        }
    }

            // 如果距離 < 50 但鼠標未松開，設為 READY_TO_EAT
            for (auto& rollObj : m_Rolls) {
                float dist2 = glm::distance(
                    customer->m_Transform.translation,
                    rollObj->m_Transform.translation
                );
                if (dist2 < 50.0f) {
                    customer->SetEatState(Customer::EatState::READY_TO_EAT);
                    break;
                }
            }
        }


        // 乞丐 aka 垃圾桶，邏輯跟客人差不多，只是不挑食
        if (m_PoorMan) {
            for (auto it = m_Rolls.begin(); it != m_Rolls.end();) {
                auto& rollObj = *it;
                float distance = glm::distance(m_PoorMan->m_Transform.translation, rollObj->m_Transform.translation);
                if (distance < 50.0f) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT
                    ) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        m_Renderer->RemoveChild(rollObj);
                        it = m_Rolls.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN)
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        if (m_PoorMan) {
            for (auto it = m_FrenchFriesList.begin(); it != m_FrenchFriesList.end();) {
                auto& friesObj = *it;
                if (m_PoorMan->IsNearFrenchFries(*friesObj)) {
                    m_PoorMan->SetEatState(PoorMan::EatState::READY_TO_EAT);
                    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) &&
                        m_PoorMan->GetEatState() == PoorMan::EatState::READY_TO_EAT
                    ) {
                        m_PoorMan->SetEatState(PoorMan::EatState::EATEN);
                        m_Renderer->RemoveChild(friesObj);
                        it = m_FrenchFriesList.erase(it);
                        g_IsObjectDragging = false;
                        continue;
                    }
                } else {
                    if (m_PoorMan->GetEatState() != PoorMan::EatState::EATEN)
                        m_PoorMan->SetEatState(PoorMan::EatState::NOT_EATEN);
                }
                ++it;
            }
        }

        // 點擊 juicepack 生成 juice（場上只能一個）
        if (!m_Juice && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
            glm::vec2 mousePos = Util::Input::GetCursorPosition();
            float w = 300.0f * m_JuicePack->m_Transform.scale.x;
            float h = 300.0f * m_JuicePack->m_Transform.scale.y;
            glm::vec2 min = m_JuicePack->m_Transform.translation - glm::vec2(w/2, h/2);
            glm::vec2 max = m_JuicePack->m_Transform.translation + glm::vec2(w/2, h/2);
            if (mousePos.x >= min.x && mousePos.x <= max.x &&
                mousePos.y >= min.y && mousePos.y <= max.y) {
                m_Juice = std::make_shared<Juice>();
                m_Renderer->AddChild(m_Juice);
                }
        }

        // 給客人喝
        if (m_Juice) {
            m_Juice->Update();
            for (auto it = m_Customers.begin(); it != m_Customers.end(); )
            {
                auto& cust = *it;
                float dist = glm::distance(
                    cust->m_Transform.translation,
                    m_Juice->m_Transform.translation
                );
                if (dist < 50.0f && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
                {
                    // 先確認這位客人是否要喝果汁
                    if (cust->GetRequestedFood() == "Juice")
                    {
                        // 已喝到
                        cust->RecordFood("Juice");

                        // 刪除客人的 PatienceText、Order Icon、客人
                        if (auto pt = cust->GetPatienceText()) {
                            m_Renderer->RemoveChild(pt);
                            cust->SetPatienceText(nullptr);
                        }
                        if (cust->GetOrderIcon()) {
                            m_Renderer->RemoveChild(cust->GetOrderIcon());
                        }
                        m_Renderer->RemoveChild(cust);
                        it = m_Customers.erase(it);

                        // 加錢（15 元）
                        m_MoneyManager.Add(15);
                    }
                    else
                    {
                    }

                    // 不論有沒有給正確的飲料，都把果汁移除
                    m_Renderer->RemoveChild(m_Juice);
                    m_Juice.reset();
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }


    }

    // 作弊模式按鈕(店鋪)
    if (m_ShopButton->IsClicked() && m_CurrentPhase == phase::phase1) {
        m_CurrentPhase = phase::phase3;
        m_Background = std::make_shared<BackgroundImage>(RESOURCE_DIR "/Image/background/LevelPage1.png");
        m_Renderer = std::make_shared<Util::Renderer>(std::vector<std::shared_ptr<Util::GameObject>>{ m_Background });
        m_Renderer->AddChild(m_ReturnButton);
        m_IgnoreNextMouseUp = true;
        return;
    }
    if (m_ReturnButton->IsClicked() ) {
        m_CurrentPhase = phase::phase1;
        m_CurrentState = State::START;
        m_IgnoreNextMouseUp = true;
        return;
    }

    // 失敗
    if (m_CurrentPhase == phase::failed && m_RetryButton && m_RetryButton->IsClicked()) {
        m_Renderer->RemoveChild(m_FailureScreen);
        m_Renderer->RemoveChild(m_RetryButton);
        m_FailureScreen.reset();
        m_RetryButton.reset();

        // 重新玩當前關卡
        LoadLevel(m_LevelManager.GetCurrentLevel());
        m_CurrentPhase = phase::phase2;
        return;
    }
    if (m_Renderer) m_Renderer->Update();
    for (auto& friesObj : m_FrenchFriesList) friesObj->Update();
    for (auto& roll : m_Rolls) roll->Update();
}

// 關卡的各種前置作業
void App::LoadLevel(const LevelData& level) {
    m_Renderer = std::make_shared<Util::Renderer>();
    m_CurrentDay = m_LevelManager.GetCurrentLevelNumber();
    m_DayText->SetText( std::to_string(m_CurrentDay));
    m_Renderer->AddChild(m_DayTextGO);

    m_MoneyManager = MoneyManager(0);
    m_MoneyManager.SetOnChangeCallback([this](int newBal){
        m_MoneyText->SetText("$" + std::to_string(newBal));
    });
    m_MoneyText->SetText("$0");
    m_Rolls.clear();
    m_FrenchFriesList.clear();
    toppings.clear();
    m_Cups.clear();
    m_Fries->SetPlaced(false);
    m_Sauce->SetPlaced(false);
    m_Pickle->SetPlaced(false);
    m_ShavedMeat->SetPlaced(false);

    if (m_Juice) {
        m_Renderer->RemoveChild(m_Juice);
        m_Juice.reset();
    }

    int currentIndex = m_LevelManager.GetCurrentLevelIndex();
    std::cout << "current level: " << currentIndex + 1 << std::endl;

    // 第 10 關開始配料會減少
    m_EnableIngredientLimit = (currentIndex >= 10);
    m_Pickle->EnableLimit(m_EnableIngredientLimit);
    m_Sauce->EnableLimit(m_EnableIngredientLimit);

    // 從第 25 關起啟用客製配料檢查
    m_EnableCustomTopping = (currentIndex >= 25);

   // 第 5 關開始有耐心值
    if (currentIndex >= 5) {
        m_EnablePatience = true;
    }
    else {
        m_EnablePatience = false;
    }
    // 薯條數值重置
    m_FryingCounter = 0;
    if (m_FryingCounterText) m_FryingCounterText->UpdateCounter(0);

    m_Renderer->AddChild(m_ReturnButton);
    m_Renderer->AddChild(m_Meat);
    m_Renderer->AddChild(m_Knife);
    m_Renderer->AddChild(m_Crust);
    m_Renderer->AddChild(m_Paper);
    m_Renderer->AddChild(m_Fries);
    m_Renderer->AddChild(m_Sauce);
    m_Renderer->AddChild(m_ShavedMeat);
    m_Renderer->AddChild(m_Pickle);

    m_BevMachine = std::make_shared<BeverageMachine>(
    glm::vec2(300.0f, -100.0f),
    glm::vec2(0.5f)
);
    m_Renderer->AddChild(m_BevMachine);

    glm::vec2 basePos = m_BevMachine->GetPosition();
    glm::vec2 scale   = m_BevMachine->GetScale();
    glm::vec2 off1(-50.0f, -50.0f), off2(50.0f, -50.0f);

    // 生成汽水
    auto sodaCup = std::make_shared<Cup>(
        DrinkType::SODA,
        basePos + off1 * scale,
        CupState::EMPTY);
    m_Cups.push_back(sodaCup);
    m_Renderer->AddChild(sodaCup);

    // 生成可樂
    auto colaCup = std::make_shared<Cup>(
        DrinkType::COLA,
        basePos + off2 * scale,
        CupState::EMPTY);
    m_Cups.push_back(colaCup);
    m_Renderer->AddChild(colaCup);

    m_JuicePack = std::make_shared<Util::GameObject>(
        std::make_unique<Util::Image>(RESOURCE_DIR "/Image/Food/juicepack.png"), 4);
    m_JuicePack->m_Transform.translation = glm::vec2(-420.0f, -210.0f);
    m_JuicePack->m_Transform.scale = glm::vec2(0.5f, 0.5f);
    m_Renderer->AddChild(m_JuicePack);

    if (m_EnableIngredientLimit) {
        m_CucumberHand = std::make_shared<CucumberHand>();
        m_SauceHand = std::make_shared<SauceHand>();
        m_Renderer->AddChild(m_CucumberHand);
        m_Renderer->AddChild(m_SauceHand);
    } else {
        m_CucumberHand = nullptr;
        m_SauceHand = nullptr;
    }



    m_Renderer->AddChild(m_PoorMan);
    m_Renderer->AddChild(m_Potato);
    m_Potato->SetPlaced(false);
    m_Renderer->AddChild(m_FryingCounterText);
    m_Renderer->AddChild(m_MoneyTextGO);

    m_Renderer->AddChild(m_Pickle->GetCounterObject());
    m_Pickle->EnableLimit(m_EnableIngredientLimit);
    m_Pickle->ResetCount();

    m_Renderer->AddChild(m_Sauce->GetCounterObject());
    m_Sauce->EnableLimit(m_EnableIngredientLimit);
    m_Sauce->ResetCount();

    m_Background = std::make_shared<BackgroundImage>(level.backgroundImage);
    m_Renderer->AddChild(m_Background);
    m_TotalCustomersThisLevel = (int)level.customers.size();
    m_PatienceFailures = 0;
    m_FailureScreen.reset();
    m_Customers.clear();
    m_LevelManager.StartLevel();
}

void App::End() {
    LOG_TRACE("End");
    m_Renderer.reset();
}

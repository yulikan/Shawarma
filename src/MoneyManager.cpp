//
// Created by louise on 4/18/2025.
//
#include "MoneyManager.hpp"

MoneyManager::MoneyManager(int initial)
    : m_Balance(initial)
{}

int MoneyManager::GetBalance() const {
    return m_Balance;
}

void MoneyManager::Add(int amount) {
    m_Balance += amount;
    if (m_Callback) m_Callback(m_Balance);
}

bool MoneyManager::Spend(int amount) {
    if (amount > m_Balance) return false;
    m_Balance -= amount;
    if (m_Callback) m_Callback(m_Balance);
    return true;
}

void MoneyManager::SetOnChangeCallback(std::function<void(int)> callback) {
    m_Callback = std::move(callback);
}
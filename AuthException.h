// AuthManager.h
#pragma once

#include <string>
#include <vector>

/// \brief Керує автентифікацією та авторизацією користувачів.
class AuthManager
{
public:
    /// \brief Створює об’єкт менеджера автентифікації.
    /// \param usersFile Шлях до файлу з обліковими записами користувачів.
    explicit AuthManager(const std::string& usersFile = "data/users.txt");

    /// \brief Виконує вхід користувача за введеними обліковими даними.
    /// \return true, якщо автентифікація пройшла успішно, інакше false.
    bool login();

    /// \brief Повертає ім’я поточного автентифікованого користувача.
    /// \return Ім’я користувача або порожній рядок, якщо вхід не виконано.
    std::string getCurrentUser() const;

    /// \brief Перевіряє, чи є поточний користувач адміністратором.
    /// \return true, якщо поточний користувач має права адміністратора, інакше false.
    bool isAdmin() const;

    /// \brief Повертає список усіх зареєстрованих користувачів.
    /// \return Вектор імен усіх користувачів.
    std::vector<std::string> listUsers() const;

    /// \brief Додає нового користувача з указаними обліковими даними.
    /// \param username Ім’я нового користувача.
    /// \param password Пароль нового користувача.
    /// \return true, якщо користувача успішно додано, інакше false.
    bool addUser(const std::string& username, const std::string& password);

    /// \brief Видаляє користувача з указаним ім’ям.
    /// \param username Ім’я користувача, якого потрібно видалити.
    /// \return true, якщо користувача успішно видалено, інакше false.
    bool deleteUser(const std::string& username);

private:
    std::string usersFile;
    std::string currentUser;

    /// \brief Перевіряє коректність облікових даних користувача.
    /// \param login Ім’я користувача.
    /// \param password Пароль користувача.
    /// \return true, якщо облікові дані коректні, інакше false.
    bool validateCredentials(const std::string& login, const std::string& password) const;
};

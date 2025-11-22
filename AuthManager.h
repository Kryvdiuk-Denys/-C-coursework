#pragma once

#include <string>
#include <vector>

/// \brief Клас для авторизації та керування користувачами.
class AuthManager
{
public:
 /// \brief Створює менеджер з файлом користувачів.
 /// \param usersFile Шлях до файлу з користувачами.
 explicit AuthManager(const std::string& usersFile = "data/users.txt");

 /// \brief Виконує вхід користувача.
 /// \return true, якщо автентифікація успішна, інакше false.
 /// \throws ValidationException У разі некоректного введення облікових даних.
 bool login();

 /// \brief Повертає логін поточного користувача.
 /// \return Логін поточного користувача або порожній рядок, якщо вхід не виконано.
 std::string getCurrentUser() const;

 /// \brief Перевіряє, чи є поточний користувач адміністратором.
 /// \return true, якщо поточний користувач має права адміністратора, інакше false.
 bool isAdmin() const;

 /// \brief Повертає список усіх логінів користувачів.
 /// \return Вектор логінів усіх користувачів.
 std::vector<std::string> listUsers() const;

 /// \brief Додає нового користувача.
 /// \param username Логін нового користувача.
 /// \param password Пароль нового користувача.
 /// \return true, якщо користувача додано, інакше false (логін уже існує).
 bool addUser(const std::string& username, const std::string& password);

 /// \brief Видаляє користувача за логіном.
 /// \param username Логін користувача, якого потрібно видалити.
 /// \return true, якщо користувача видалено, інакше false (не знайдено або admin).
 bool deleteUser(const std::string& username);

private:
 std::string usersFile;
 std::string currentUser;

 /// \brief Перевіряє коректність облікових даних користувача.
 /// \param login Логін користувача.
 /// \param password Пароль користувача.
 /// \return true, якщо облікові дані коректні, інакше false.
 bool validateCredentials(const std::string& login,
     const std::string& password) const;
};

// TourManager.h
#pragma once

#include "Tour.h"

#include <memory>
#include <string>
#include <vector>

/// \file TourManager.h
/// \brief Оголошення класу TourManager для керування списком турів.

/// \class TourManager
/// \brief Клас для керування списком турів.
/// \details Відповідає за:
/// - завантаження та збереження турів у файл;
/// - пошук, сортування та фільтрацію турів;
/// - редагування та видалення турів;
/// - роботу меню адміністратора та користувача;
/// - бронювання турів.
class TourManager
{
public:
   /// \brief Створює менеджер турів із вказаним файлом даних.
   /// \param dataFile Шлях до CSV-файлу зі списком турів.
   explicit TourManager(const std::string& dataFile = "data/tours.csv");

   /// \brief Завантажує тури з файлу у пам’ять.
   /// \throws FileException Якщо файл не вдається відкрити або прочитати.
   void load();

   /// \brief Зберігає всі тури з пам’яті у файл.
   /// \throws FileException Якщо файл не вдається відкрити для запису.
   void save() const;

   /// \brief Запускає головне (адміністративне) меню керування турами.
   void mainMenu();

   /// \brief Запускає меню звичайного користувача.
   /// \param username Ім’я користувача, для якого відображається меню.
   void userMenu(const std::string& username);

   /// \brief Оформлює бронювання туру для вказаного користувача.
   /// \param username Ім’я користувача, який бронює тур.
   void bookTicket(const std::string& username);

private:
   std::string                        dataFile;
   std::vector<std::shared_ptr<Tour>> tours;

   /// \brief Виводить у консоль усі тури з поточного списку.
   void displayAll() const;

   /// \brief Додає новий тур (міський або гірськолижний).
   void addTour();

   /// \brief Відкриває меню пошуку турів.
   void searchMenu() const;

   /// \brief Відкриває меню сортування турів.
   void sortMenu();

   /// \brief Відкриває меню фільтрації турів.
   void filterMenu() const;

   /// \brief Редагує обраний тур за індексом.
   void editTour();

   /// \brief Видаляє обраний тур за індексом.
   void deleteTour();

   /// \brief Перевіряє, чи входить дата у заданий діапазон.
   /// \param date Дата для перевірки.
   /// \param from Початок інтервалу (може бути порожнім).
   /// \param to Кінець інтервалу (може бути порожнім).
   /// \return true, якщо дата входить у діапазон або діапазон не заданий.
   static bool dateInRange(const std::string& date,
      const std::string& from,
      const std::string& to);

   // Обгортки для меню користувача.

   /// \brief Виводить усі тури (використовується у меню користувача).
   void listAllTours() const
   {
      displayAll();
   }

   /// \brief Викликає меню сортування для користувача.
   void sortToursMenu()
   {
      sortMenu();
   }

   /// \brief Викликає меню фільтрації для користувача.
   void filterToursMenu() const
   {
      filterMenu();
   }

   /// \brief Виводить довідку для адміністратора.
   void helpInfoAdmin() const;

   /// \brief Виводить довідку для користувача.
   void helpInfoUser() const;
};

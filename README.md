# Дослідження обмежень ресурсів у середовищі Docker

## Зміст
[Завдання 1: Встановлення та дослідження лімітів відкритих файлів](#завдання-1-встановлення-та-дослідження-лімітів-відкритих-файлів)

---

## Завдання 1: Встановлення та дослідження лімітів відкритих файлів

### Опис
Це завдання досліджує механізми обмеження кількості відкритих файлових дескрипторів у FreeBSD системі. Ми вивчаємо як переглядати поточні значення обмежень, їх м'які (soft) та жорсткі (hard) значення, а також можливості зміни цих обмежень з правами root.

### Результати виконання
- ```bash
  # Запуск контейнера FreeBSD 14 за допомогою podman (FreeBSD нативно не підтримує Docker)
  podman run -it --rm quay.io/dougrabson/freebsd14-minimal /bin/sh
  
  # Перегляд м'якого ліміту відкритих файлів
  ulimit -aS | grep "open files"
  open files                      (-n)  116964
  
  # Перегляд жорсткого ліміту відкритих файлів
  ulimit -aH | grep "open files"
  open files                      (-n)  116964
  
  # Зміна ліміту на нове значення
  ulimit -n 3000
  
  # Спроба збільшити ліміт
  ulimit -n 3001
  
  # Зменшення ліміту
  ulimit -n 2000

  # Перегляд м'якого ліміту відкритих файлів
  ulimit -aS | grep "open files"
  open files                      (-n)  2000

  # Перегляд жорсткого ліміту відкритих файлів
  ulimit -aH | grep "open files"
  open files                      (-n)  2000

  # Вихід з контейнеру і перевірка обмжень оригінального образу
  exit
  ulimit -n
  116964
  ```

### Висновки
- Система FreeBSD 14 за замовчуванням налаштована з досить високим значенням максимальної кількості відкритих файлів - 116964.
- У FreeBSD, як і в інших UNIX-подібних системах, користувач із привілеями root може змінювати будь-які значення лімітів.
- При зміні ліміту через `ulimit -n` змінюються обидва значення (м'яке і жорстке) одночасно на правах root (має повний контроль над цими лімітами).


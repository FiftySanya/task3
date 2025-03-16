# Дослідження обмежень ресурсів у середовищі Docker
# Дослідження обмежень файлових дескрипторів у FreeBSD

## Зміст
[Завдання1: Встановлення та дослідження лімітів відкритих файлів](#завдання1-встановлення-та-дослідження-лімітів-відкритих-файлів)

---

## Завдання1: Встановлення та дослідження лімітів відкритих файлів

### Опис
Це завдання досліджує механізми обмеження кількості відкритих файлових дескрипторів у FreeBSD системі. Ми вивчаємо як переглядати поточні значення обмежень, їх м'які (soft) та жорсткі (hard) значення, а також можливості зміни цих обмежень з правами root.

### Команди для дослідження

```bash
# Запуск контейнера FreeBSD 14
podman run -it --rm quay.io/dougrabson/freebsd14-minimal /bin/sh

# Перевірка поточного значення максимальної кількості відкритих файлів
ulimit -n

# Перегляд м'якого ліміту відкритих файлів
ulimit -aS | grep "open files"

# Перегляд жорсткого ліміту відкритих файлів
ulimit -aH | grep "open files"

# Зміна ліміту на нове значення
ulimit -n 3000

# Спроба збільшити ліміт
ulimit -n 3001

# Зменшення ліміту
ulimit -n 2000
```

### Результати виконання

Початкові значення:
```
# ulimit -n
116964
# ulimit -aS | grep "open files"
open files                      (-n)  116964
# ulimit -aH | grep "open files"
open files                      (-n)  116964
```

Встановлення нового значення:
```
# ulimit -n 3000
# ulimit -aS | grep "open files"
open files                      (-n)  3000
# ulimit -aH | grep "open files"
open files                      (-n)  3000
```

Подальші зміни:
```
# ulimit -n 3001
# ulimit -n 2000
# ulimit -n
2000
# ulimit -aS | grep "open files"
open files                      (-n)  2000
# ulimit -aH | grep "open files"
open files                      (-n)  2000
```

### Аналіз результатів

#### Початкові значення лімітів
- Система FreeBSD 14 за замовчуванням налаштована з досить високим значенням максимальної кількості відкритих файлів - 116964.
- М'які (soft) та жорсткі (hard) ліміти мають однакові значення.

#### Зміна лімітів
- Команда `ulimit -n 3000` успішно змінила обидва (м'який і жорсткий) ліміти до значення 3000.
- При спробі встановити значення 3001 через команду `ulimit -n 3001` не виникло помилки, але наступна команда `ulimit -n 2000` була виконана успішно.
- Кінцевий результат показує, що ліміт був успішно змінений на 2000.

#### Особливості механізму ulimit у FreeBSD
- У FreeBSD, як і в інших UNIX-подібних системах, користувач із привілеями root може змінювати будь-які значення лімітів.
- При зміні ліміту через `ulimit -n` змінюються обидва значення (м'яке і жорстке) одночасно.
- Система не виводить помилки при невдалих операціях зміни лімітів, що потребує додаткової перевірки фактичного значення після зміни.

#### Значення для розробників та адміністраторів
- Ліміт відкритих файлів є критично важливим для серверів із високим навантаженням, особливо для веб-серверів, баз даних та інших сервісів, що відкривають багато з'єднань одночасно.
- Значення за замовчуванням (116964) є достатньо високим для більшості завдань, але може потребувати коригування для специфічних високонавантажених сценаріїв.
- При налаштуванні системи важливо перевіряти фактичні значення лімітів після їх зміни, щоб переконатися, що зміни були застосовані.

### Висновки
- Система FreeBSD надає гнучкі механізми для налаштування обмежень на кількість відкритих файлових дескрипторів.
- Користувач з правами root має повний контроль над цими лімітами.
- При налаштуванні системи рекомендується явно встановлювати обидва значення (м'яке і жорстке) для запобігання непередбачуваної поведінки.
- Важливо перевіряти фактичні значення після внесення змін, оскільки система не завжди явно повідомляє про невдалі операції.


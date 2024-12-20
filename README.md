# Расчетно-графическая работа (РГР)

## Цель работы

Закрепить и структурировать знания, полученные в рамках изучения дисциплины **«Основы систем мобильной связи»**.

---

## Этапы выполнения

### 1. Генерация битовой последовательности
- **Входные данные**: Имя и фамилия на латинице.
- **Результат**: Битовая последовательность ASCII-кодов имени и фамилии. Визуализация последовательности.

### 2. Вычисление CRC
- **Описание**: Реализован генератор CRC длиной \(M\) бит.
- **Результат**: Добавление CRC к битовой последовательности. Вывод CRC в терминал.

### 3. Генерация последовательности Голда
- **Описание**: Добавление \(G\)-битной последовательности синхронизации (Голда).
- **Результат**: Полная битовая последовательность (данные + CRC + синхронизация). Визуализация последовательности.

### 4. Преобразование бит в отсчеты сигнала
- **Описание**: Реализация амплитудной модуляции. Каждый бит представлен \(N\) отсчетами.
- **Результат**: Массив временных отсчетов длиной \(N \cdot (L + M + G)\). Визуализация сигнала.

### 5. Добавление зашумленного сигнала
- **Описание**: Генерация шума (нормальное распределение с \(\mu = 0\) и вводимым \(\sigma\)). Смешивание сигнала с шумом.
- **Результат**: Зашумленный сигнал. Визуализация сигнала.

### 6. Корреляционный приемник
- **Описание**: Определение начала последовательности синхронизации.
- **Результат**: Позиция начала синхросигнала. Вывод в терминал. Удаление лишних отсчетов.

### 7. Демодуляция и декодирование
- **Описание**:
  - Разделение оставшихся символов на \(N\)-отсчеты.
  - Интерпретация с порогом \(P\) (нулевые/единичные значения).
  - Восстановление данных без синхросигнала.
  - Проверка CRC на ошибки.
  - Декодирование ASCII.
- **Результат**: Полученные данные без ошибок. Вывод текста в терминал.

### 8. Спектральный анализ
- **Описание**: Анализ спектра передаваемого и принимаемого сигналов.
- **Результат**: Графики спектра для коротких, средних и длинных символов.

### 9. Оформление отчета
- **Описание**: 
  - Титульный лист, содержание, цель и задачи.
  - Теоретические сведения.
  - Этапы выполнения работы с графиками, скриншотами и промежуточными выводами.
  - Результирующие таблицы.
  - Заключение.
  - QR-код со ссылкой на репозиторий (GitHub).

---

## Использование

### 1. Запуск симуляции

Для запуска симуляции выполните следующую команду:

```bash
python main.py
```

### 2. Входные параметры

- Имя и фамилия на латинице.
- Длина битовой последовательности (\(L\)).
- Параметры CRC (\(M\)).
- Длина последовательности Голда (\(G\)).
- Число отсчетов на символ (\(N\)).
- Уровень шума (\(\sigma\)).

### 3. Выходные данные

- Визуализация сигналов.
- Позиция синхросигнала.
- Восстановленный текст.

---

## Требования

- **Python 3.8+**
- Библиотеки:
  - `numpy`
  - `matplotlib`
- **CPP-20**

---

## Результаты

### Графики:
- Передаваемый сигнал.
- Принятый зашумленный сигнал.
- Спектры сигналов.

### Таблицы:
- Сравнение исходных и декодированных данных.
- Уровень ошибок (BER).

### Пример вывода:

```
CRC: 110011
Синхросигнал начинается с позиции: 32
Данные корректно приняты.
Восстановленный текст: Dmitry
```

---

## Заключение

Работа успешно выполнена. Полученные результаты подтверждают теоретические знания, приобретенные в ходе изучения дисциплины. 

--- 

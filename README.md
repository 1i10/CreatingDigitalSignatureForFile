# Формирование цифровой подписи файла

## Навигация
- [Краткое описание](#краткое-описание)
- [Структура репозитория](#структура-репозитория)
- [Описание алгоритма](#описание-алгоритма-формирования-цифровой-подписи)
- [Сборка и настройка](#сборка-и-настройка-проекта)
- [Руководство пользователя](#руководство-пользователя)
- [Примеры](#примеры)
- [Тестирование](#тестирование)
  - [Сборка и настройка](#сборка-и-настройка-теста)    
  - [Результат тестирования](#результат-теста)


## Краткое описание

Проект разработан в IDE Microsoft Visual Studio 2022 с использованием библиотек boost 1.84.0 и openssl 3.2.1. Также работоспособность приложения была проверена с использованием Boost.Test.   
Приложение предназначено для создания и проверки цифровой подписи файлов на платформе Windows.  

## Структура репозитория

**CreatingDigitalSignatureForFile** - проект, включающий в себя приложение для формирования цифровой подписи.  
Содержит следующие директории:  
* **src** - содержит исходные файлы проекта;
* **include** - содержит заголовочные файлы проекта;
* **testData** - включает в себя файл для тестирования (example.txt);
* **example** - содержит изображения для руководства, примеров и т.д. данного репозитория.  
  
**Tests** - проект с тестом, который проверяет уникальность сформированных цифровых подписей.  
Содержит следующие файлы:  
* **createTestFiles.bat** - скрипт, который генерирует 1024 тестовых файла и помещает их в директорию testData на одном уровне с данным скриптом;
* **Test.cpp** - содержит тест, который проверяет действительно ли алгоритм по формированию цифровых подписей способен создать 1024 уникальные подписи.  
  
  
Классы и методы задокументированы в самом коде с использованием стиля Doxygen.

## Описание алгоритма формирования цифровой подписи

Алгоритм формирования цифровой подписи основан на использовании OpenSSL для генерации криптографических ключей и [SHA-256](https://ru.wikipedia.org/wiki/SHA-2) для хеширования метаданных файла и его содержимого.

1. *Создание ключей:* Приложение генерирует пару криптографических ключей (приватный и публичный) с использованием библиотеки OpenSSL.

2. *Хеширование метаданных и содержимого файла:* Полученные ключи используются для [XOR](https://en.wikipedia.org/wiki/XOR_cipher)-шифрования хеша, созданного из метаданных файла (имя, время создания, размер, расширение) и его содержимого.

3. *Сохранение подписи:* Цифровая подпись вместе с ключами сохраняется в виде последовательности байт в файл с расширением ".sig".

4. *Проверка подписи:* Для проверки подписи, алгоритм повторяет процесс хеширования и XOR с использованием сохраненных ключей и сравнивает результат с сохраненной подписью.

## Сборка и настройка проекта

Загрузите проект на локальную машину, воспользовавшись командой *git clone*.  
Откройте решение **CreatingDigitalSignatureForFile.sln** в Microsoft Visual Studio.  

Для успешной сборки проекта, выполните следующие шаги:

**1. Зависимости:**  
Убедитесь, что у вас установлены следующие библиотеки:

* Boost 1.84.0
* OpenSSL 3.2.1
  
**2. Установка зависимостей в свойствах проекта:**  
Откройте свойства проекта в Microsoft Visual Studio 2022. Перейдите в раздел "Каталоги VC++". Добавьте необходимые пути для заголовочных файлов Boost и OpenSSL в поле "Внешние каталоги включения".

**3. Настройка библиотек в свойствах проекта:**
Перейдите в раздел "Компоновщик" и выберите "Общие". Добавьте пути к библиотекам Boost и OpenSSL в поле "Дополнительные каталоги библиотек".

**4. Выбор используемых библиотек:**
В разделе "Ввод" компоновщика укажите необходимые библиотеки для линковки. Добавьте следующие:  
* libssl.lib и libcrypto.lib (для OpenSSL)  

**5. Пересборка проекта:**
После настройки зависимостей и библиотек, пересоберите проект, чтобы убедиться, что все изменения были применены успешно.  
  
## Руководство пользователя

После запуска консольного приложения пользователю необходимо ввести путь до файла, для которого необходимо сформировать цифровую подпись или проверить ее наличие.    
Например:  
> Enter the path to the file: ./testData/example.txt  
  
Далее, в зависимости от наличия подписи, пользователю будет предложено: добавить или удалить цифровую подпись.
Для положительного ответа пользователю требуется ввести 'y'('Y').

Подпись сохраняется в директории исходного файла в формате ".sig".

## Примеры

**Формирование цифровой подписи:**  
![Пример1](https://github.com/1i10/CreatingDigitalSignatureForFile/blob/main/CreatingDigitalSignatureForFile/example/exampleWork1.gif)  
  
**Проверка наличия цифровой подписи и ее удаление:**  
![Пример2](https://github.com/1i10/CreatingDigitalSignatureForFile/blob/main/CreatingDigitalSignatureForFile/example/exampleWork2.gif)  
  
**Изменение исходного файла и проверка наличия цифровой подписи:**  
![Пример3](https://github.com/1i10/CreatingDigitalSignatureForFile/blob/main/CreatingDigitalSignatureForFile/example/exampleWork3.gif)  


## Тестирование
  
*Цель тестирования:*  Убедиться, что реализованный алгоритм способен создать 1024 уникальные подписи.  
  
### Сборка и настройка
  
**!** Первым делом, следует перейти в директорию Tests и запустить скрипт **createTestFiles.bat**, который сформирует 1024 тестовых файла (будут располагаться на этом же уровне в директории "testData").  
  
Далее, перейдем непосредственно к настройке самого тестового проекта в MS VS.  
1. Проделайте те же шаги по настройке зависимостей, что и в основном проекте.  
2. Перейдите в раздел "Компоновщик" и выберите "Общие". Добавьте(проверьте), что указан путь к объектным файлам .obj основного проекта.
3. В разделе "Ввод" компоновщика укажите(проверьте, что есть) объектные файлы основного проекта: *DigitalSignature.obj, File.obj, FileMetadata.obj, KeyManager.obj*.
4. Проверьте, что проект Tests зависит от основного проекта (Кликнуть пкм по проекту одного из тестов → Добавить → Ссылка. Выбрать CreatingDigitalSignatureForFile).
5. Пересобрать решение в обозревателе.  
  
**Запуск тестов:**  
Вкладка "Тест" → "Обозреватель тестов" → "Выполнить все тесты в представлении"


### Результат тестирования

Для каждого файла вычисляется его цифровая подпись. Цифровая подпись помещается в хеш-таблицу. Если была создана уже существующая цифровая подпись, то счетчик коллизий увеличивается. Если в итоге коллизий нет, то тест считается успешно пройденным.  

![image](https://github.com/1i10/CreatingDigitalSignatureForFile/blob/main/CreatingDigitalSignatureForFile/example/testResult.png "Результат тестирования")  

Исходя из полученных результатов, можно сделать вывод, что разработанный алгоритм с высокой вероятностью создает уникальную цифровую подпись файла.  
# boost-ext.di (Boost.DI) v1.3.0

Официальный single-header из проекта [boost-ext/di](https://github.com/boost-ext/di),
широко известного как **Boost.DI**. Важно: библиотека **не входит в состав Boost**
(ни в 1.83, ни в 1.90) и на сайте проекта прямо указано «not an official Boost library».
Поэтому заголовок закреплён в репозитории, чтобы сборка была воспроизводимой и не
зависела от сети.

| Параметр | Значение |
|---|---|
| Версия | v1.3.0 (2023-05-23) |
| Файл | `include/boost/di.hpp` |
| Размер | 136 957 байт |
| SHA256 | `bd74fa32901ceb090efa3eef9414425f45d0c4a4ab7ca9b573052243ab89d83a` |
| Лицензия | Boost Software License 1.0 (`LICENSE_1_0.txt`) |
| Требования | C++14+, заголовок зависит только от стандартной библиотеки |

Используется только в composition root (`src/app/composition_root.cpp`) как DI-контейнер.
Boost.DI — compile-time контейнер: отсутствующая зависимость или цикл обнаруживаются
на этапе компиляции, runtime-регистрации и service locator нет.

Обновление: заменить `include/boost/di.hpp` и обновить SHA256/версию в этой таблице.

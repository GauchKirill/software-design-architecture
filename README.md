# ПОС — преподавательская оценка студентов

Консольный модуль «ПОС» из домашнего задания №1 по курсу «Проектирование и архитектура ПО».
Учебный домен: студенты, рубрикатор критериев, выставленные оценки и честный итоговый балл.

Проект демонстрирует доменную модель в духе DDD, осознанное применение
SOLID и пять уместных паттернов GoF. Домен не знает про CSV, JSON, файлы и консоль: всё общение
с внешним миром идёт через порты, реализации подключаются на краю приложения.

**Стек:** C++23, CMake 3.24+, Ninja, GoogleTest 1.14, системный Boost 1.90
(ProgramOptions, JSON, Filesystem, Algorithm/String), DI-контейнер Boost.DI (boost-ext.di v1.3.0,
вендорный заголовок).

---

## Что реализовано

| Требование из ТЗ | Где |
|---|---|
| Студенты и рубрикатор: добавление и просмотр | `StudentService`, `RubricatorService`, меню 1–2 |
| Выставление/просмотр/исправление/отмена оценок | `EvaluationService`, меню 3 |
| Инвариант «балл в диапазоне критерия» | `Score`, `Evaluation::rescore`, `EvaluationFactory` |
| Инвариант «оценка только по критерию рубрикатора» | `EvaluationFactory` + поиск `CriterionId` в `Rubricator` |
| Итог считается Domain Service | `FinalScoreCalculator` + `FinalScorePolicy` |
| Экспорт итога | CSV/JSON, `ReportService`, `FileReportWriter` |
| Сквозной демонстрационный сценарий | `DemoScenario`, `pos --demo` |
| DI-контейнер | Boost.DI в `CompositionRoot` |
| 61 gtest-тест на инварианты и сервисы | `tests/`, `ctest` |

---

## Быстрый старт

### Требования

- компилятор с C++23 (`g++` 14+; проект проверен на GCC 16.0.1);
- CMake 3.24+ и Ninja (или Make);
- Boost **1.90** с компонентами `program_options`, `json`, `filesystem`
  (заголовки и библиотеки одной версии — см. раздел «Boost»);
- GoogleTest 1.14 (если не найден, CMake скачает его через `FetchContent`);
- Boost.DI уже лежит в `third_party/boost-ext/di/` и в системных пакетах не нуждается.

Установка зависимостей на Ubuntu/Debian:

```bash
sudo apt install build-essential cmake ninja-build \
    libboost-dev libboost-program-options-dev libboost-json-dev libboost-filesystem-dev \
    libgtest-dev libgmock-dev
```

### Сборка, тесты, запуск

```bash
cmake --preset release
cmake --build --preset release

./build/release/pos                 # интерактивное меню
./build/release/pos --demo          # сквозной сценарий, экспорт в ./out/
./build/release/pos --help          # справка
```

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset asan
```

| Опция | По умолчанию | Смысл |
|---|---|---|
| `--demo` | выкл. | прогнать демонстрационный сценарий и выйти |
| `--formula` | `weighted-average` | формула итога: `weighted-average` или `weighted-points` |
| `--rounding` | `1` | число знаков после запятой (0..4) |
| `--out` | `out` | каталог для CSV/JSON |
| `-h`, `--help` | — | справка |

Демо создаёт студентов, критерии и оценки, показывает отказ на `12/10`, отказ на повторной
оценке, исправляет оценку, считает итоги и пишет `out/final_scores.csv` и
`out/final_scores.json`. Ожидаемые итоги (взвешенное среднее, 1 знак): Иванов — 88.0,
Петрова — 91.0, Сидоров — 42.0 (оценено 2 критерия из 3; неоценённый даёт 0).

### Интерактивное меню

```
1) Студенты        — добавить / список
2) Рубрикатор      — добавить критерий / список
3) Оценки          — выставить / список по студенту / исправить / отменить
4) Итоги           — по студенту / по всем
5) Экспорт итогов  — csv или json, путь по умолчанию out/final_scores.<fmt>
0) Выход
```

Ошибки домена не роняют приложение: сообщение печатается, меню продолжает работу. Ввод/вывод
инъецируется через `std::istream`/`std::ostream`, поэтому сценарии тестируемы.

---

## Архитектура

Слои и направление зависимостей (стрелка — «знает о»):

```
┌────────────────────────────┐
│ presentation/console       │  ConsoleApp, ConsoleIo, ConsoleView
└─────────────┬──────────────┘
              │
┌─────────────▼──────────────┐
│ application                │  PosFacade, use-case services, DTO, порты отчётов
└─────────────┬──────────────┘
              │
┌─────────────▼──────────────┐
│ domain                     │  model, инварианты, порты, Domain Service, Strategy
└─────────────▲──────────────┘
              │ реализует порты
┌─────────────┴──────────────┐
│ infrastructure             │  in-memory репозитории, SystemClock, CSV/JSON, файлы
└────────────────────────────┘
                       ▲
                       │ собирает граф
┌──────────────────────┴─────┐
│ app / composition root     │  Boost.DI, AppConfig, DemoScenario, CLI
└────────────────────────────┘
```

Правило зависимостей: `domain` не включает ничего из `application`, `infrastructure`,
`presentation` и не знает про Boost/файлы/консоль. `application` зависит только от доменных
портов и DTO. `infrastructure` реализует порты домена и приложения. Сборка графа — в
`src/app/composition_root.cpp`, единственном месте, где известны конкретные классы.

---

## DDD: карта модели

### Ubiquitous Language → код

| Предметная область | Код | Тип |
|---|---|---|
| студент, номер зачётки | `Student`, `StudentId` | Entity, VO |
| ФИО, группа | `FullName`, `GroupName` | VO |
| критерий, код критерия | `Criterion`, `CriterionId` | Entity внутри агрегата, VO |
| максимум критерия, вес | `MaxScore`, `Weight` | VO |
| рубрикатор | `Rubricator` | Aggregate Root |
| оценка, идентификатор оценки | `Evaluation`, `EvaluationId` | Aggregate Root, VO |
| балл | `Score` | VO |
| итог | `FinalScore` | VO |
| результат расчёта | `FinalScoreResult` | VO-структура |

В коде имена английские (общепринятая практика C++), но каждое понятие однозначно сопоставлено
с русским термином; сообщения об ошибках и консольный интерфейс используют слова предметной
области («студент», «критерий», «оценка», «итог»). Это и есть практическая проверка UL: преподаватель
читает таблицу и узнаёт свои термины, а не `DataManager`/`Processor`.

### Entity и Value Object: пример осознанного разделения

- `Student` — **Entity**: идентичность задаёт номер зачётки (`StudentId`). Смена ФИО или группы
  не меняет `id`; `change_name`/`change_group` — поведение сущности.
- `FullName` — **Value Object**: два одинаковых ФИО взаимозаменяемы, объект иммутабелен,
  нормализуется (trim) и проверяется при создании.
- `Evaluation` — **Entity**: факт «этому студенту по этому критерию поставили балл» имеет
  идентичность и жизненный цикл (создание, исправление, отмена).
- `Score` — **Value Object**: определяется парой `(points, max_points)`, инвариант
  `0 <= points <= max_points` проверяется в конструкторе. Невалидный балл невозможно создать.

### Агрегаты и инварианты

| Агрегат / объект | Инвариант | Где охраняется |
|---|---|---|
| `Score` | `0 <= points <= max_points`, `max_points > 0` | конструктор `Score` / `Score::of` |
| `Criterion` | непустое имя, `max_score > 0`, `weight > 0` | `Criterion`, `MaxScore`, `Weight` |
| `Rubricator` | уникальность критерия по коду и названию | `Rubricator::add_criterion` (`ConflictError`) |
| `Evaluation` | балл не выше максимума; максимум сохраняется при исправлении | `EvaluationFactory`, `Evaluation::rescore` |
| `Evaluation` + `Rubricator` | оценка только по существующему критерию | `EvaluationFactory::create(const Student&, const Rubricator&, CriterionId, ...)` — фабрика сама ищет критерий и бросает `NotFoundError` |
| `Student` + `Evaluation` | одна актуальная оценка на пару (студент, критерий) | `EvaluationService` + `EvaluationReader::find_by_student_and_criterion` |
| `Student` + `Rubricator` + оценки | итог согласован с набором оценок; чужие/неизвестные/дублирующиеся оценки отвергаются | `FinalScoreCalculator` (Domain Service) |

`EvaluationFactory` — единственный путь создания `Evaluation`: конструктор приватный, фабрика —
`friend`. Поэтому «оценка без критерия» или «балл 12 из 10» невыразимы в модели.

### Repository (порты)

Порты — в `src/domain/ports/`, реализации — в `src/infrastructure/persistence/`. Интерфейсы
разделены по ISP: `StudentReader`/`StudentWriter`, `RubricatorReader`/`RubricatorWriter`
(рубрикатор сохраняется целиком как агрегат), `EvaluationReader`/`EvaluationWriter`,
`Clock`. Домен не знает, in-memory это, файл или сеть; замена хранилища — новый адаптер
и одна строка в composition root.

### Domain Service

`FinalScoreCalculator` (`src/domain/services/final_score_calculator.cpp`) — правило «итог по
студенту» не принадлежит ни студенту, ни критерию, ни одной оценке. Сервис проверяет, что все
оценки относятся к запрошенному студенту, что критерии есть в рубрикаторе и что по критерию нет
дубликатов, затем делегирует формулу выбранной `FinalScorePolicy` и возвращает
`FinalScoreResult` с числом оценённых критериев.

- `WeightedAveragePolicy`: `Σ(weight_i × points_i / max_i) / Σ(weight_i) × 100`;
- `WeightedPointsPolicy`: `Σ(points_i × weight_i) / Σ(max_i × weight_i) × 100`.

Неоценённый критерий даёт 0 баллов — осознанное правило «честного итога».

### Domain Events

В текущем ядре события не нужны: единственный межагрегатный сценарий (уникальность оценки на
пару) решается синхронной проверкой через репозиторий в application-сервисе. При появлении
пересдач/нескольких курсов события (`EvaluationRecorded`, `RubricatorChanged`) станут уместны —
см. ограничения и ADR.


---

## SOLID: где что проявилось

| Принцип | Проявление в коде |
|---|---|
| **S** | Домен — правила (`src/domain`), application — сценарии (`*Service`, `PosFacade`), infrastructure — I/O (`InMemory*Repository`, `FileReportWriter`), presentation — только меню/ввод/вывод (`ConsoleApp`, `ConsoleIo`, `ConsoleView`). `ConsoleApp` не считает итог и не пишет файлы. |
| **O** | Новый критерий — данные, код не меняется. Новая формула итога — новый `FinalScorePolicy` и ветка выбора в composition root. Новый формат экспорта — новый подкласс `FinalScoreReportTemplate` и ветка `ReportFactoryImpl`. |
| **L** | `WeightedAveragePolicy`, `WeightedPointsPolicy`, `RoundedFinalScorePolicy` взаимозаменяемы по контракту `FinalScorePolicy` и всегда возвращают `FinalScore` 0..100. LSP проверяется в `FinalScorePolicyTest.DecoratorRoundsResultAndKeepsContract`. |
| **I** | Узкие порты: `StudentReader`/`StudentWriter`, `EvaluationReader`/`EvaluationWriter`, `ReportWriter`; нет «IGodRepository». `ReportService` не получает доступ к записи оценок. |
| **D** | Application зависит от абстракций (`FinalScorePolicy`, `FinalScoreReportFactory`, `ReportWriter`, порты репозиториев); конкретику подставляет `CompositionRoot` через Boost.DI. |

### Сознательные упрощения (и почему это приемлемо)

- **Один рубрикатор и один курс.** В ТЗ ядро без нескольких курсов; `Rubricator` — singleton-агрегат.
  При расширении появится `CourseId` и `CourseRepository`, но модель критериев/оценок не изменится.
- **Критерии не редактируются после добавления.** Изменение `max_score` у уже оценённого критерия
  требует пересогласования всех `Evaluation` — это отдельный сценарий. Сейчас критерий иммутабелен,
  а `EvaluationService::correct` дополнительно проверяет совпадение максимума.
- **In-memory репозитории не потокобезопасны.** Приложение однопоточное; для конкурентного режима
  нужен адаптер с транзакциями/блокировками, домен при этом не меняется.
- **Уникальность оценки на пару — на уровне application.** Это межагрегатный инвариант; держать
  его внутри `Evaluation` нельзя без загрузки всех оценок студента, а делать `Student` агрегатом
  со всеми оценками — против правила «маленьких агрегатов». Проверка вынесена в сценарий и
  продублирована бы уникальным индексом в реальной БД.
- **`PosFacade` — в основном делегирование.** Это осознанный Facade: консоль не должна знать
  пять сервисов и порядок их вызова; цена — тонкий слой делегирования.

---

## GoF: пять паттернов по делу

| Группа | Паттерн | Где | Какую боль закрывает | Последствия |
|---|---|---|---|---|
| Порождающий | **Factory Method** | `EvaluationFactory` (`src/domain/services/evaluation_factory.cpp`) | Единая точка создания `Evaluation` с проверкой `score ∈ [0, max]` и поиском критерия в рубрикаторе. Приватный конструктор не даёт обойти проверку. | + невалидная оценка или критерий «из ниоткуда» невыразимы; − для создания нужны `Student`, `Rubricator` и `CriterionId`, а не просто id |
| Структурный | **Facade** | `PosFacade` (`src/application/pos_facade.cpp`) | Консоль не знает про пять сервисов, репозитории и калькулятор — один узкий API сценариев. | + UI не размазан по application; − дополнительный слой делегирования |
| Структурный | **Decorator** | `RoundedFinalScorePolicy` | Округление итога (0..4 знака) поверх любой стратегии, без дублирования в каждой формуле и без изменения самих стратегий. | + композиция и включение флагом `--rounding`; − цепочка из двух объектов и виртуальный вызов |
| Поведенческий | **Strategy** | `FinalScorePolicy` + `WeightedAveragePolicy` / `WeightedPointsPolicy` | Разные правила итога; выбор на этапе композиции (`--formula`). Новый способ расчёта не правит `FinalScoreCalculator`. | + OCP; − нужно явно выбирать/документировать формулу |
| Поведенческий | **Template Method** | `FinalScoreReportTemplate` + `CsvFinalScoreReport` / `JsonFinalScoreReport` | Общий каркас выгрузки (заголовок → строки с разделителем → подвал), различается только сериализация строк. | + CSV/JSON не дублируют обход; − для нового формата нужен новый подкласс (наследование) |

Почему не взяты другие: **Singleton** заменён DI-контейнером; **Builder** избыточен для критерия
из четырёх полей; **Observer/Command** не нужны без асинхронности и undo-стека; **Adapter** не
считаем паттерном за реализацию порта — это просто адаптер слоя инфраструктуры.
Правило ТЗ «максимум два в группе» соблюдено: по два структурных и поведенческих, один порождающий.


---

## Boost: что используется и почему

| Библиотека | Где | Зачем |
|---|---|---|
| **Boost.DI** (boost-ext.di v1.3.0) | `src/app/composition_root.cpp` | DI-контейнер: singleton-хранилище, порты → адаптеры, выбор Strategy/Decorator через lambda-binding |
| **Boost.ProgramOptions** 1.90 | `src/app/cli.cpp` | разбор `--demo`, `--formula`, `--rounding`, `--out`, `--help`, автосправка |
| **Boost.JSON** 1.90 | `src/infrastructure/reports/json_final_score_report.cpp` | экранирование строк для корректного JSON |
| **Boost.Filesystem** 1.90 | `src/infrastructure/reports/file_report_writer.cpp` | `path`, `create_directories`, `ofstream` |
| **Boost.Algorithm/String** 1.90 | `src/presentation/console/console_io.cpp` | `trim_copy` для пользовательского ввода |

Сознательно **не** используем: Boost.Test (по ТЗ нужен gtest), Boost.Format (в C++23 есть
`std::format`), Boost.DateTime (`std::chrono::year_month_day`), Boost.UUID (естественные ключи
домена), Boost.Leaf (инварианты выражены исключениями). Boost не «для галочки»: каждая
библиотека стоит на границе приложения или в infrastructure.

### Boost.DI: важный нюанс

Boost.DI **не входит в состав Boost**: это отдельный проект
[boost-ext/di](https://github.com/boost-ext/di), на сайте которого прямо написано
«not an official Boost library». Поэтому официальный single header v1.3.0 закреплён в
`third_party/boost-ext/di/`:

- файл `include/boost/di.hpp`, SHA256 `bd74fa32901ceb090efa3eef9414425f45d0c4a4ab7ca9b573052243ab89d83a`;
- лицензия BSL-1.0 в `LICENSE_1_0.txt`;
- версия/источник/контрольная сумма — в `third_party/boost-ext/di/README.md`.

Вендоринг делает сборку воспроизводимой и не зависящей от сети. CMake подключает заголовок как
`SYSTEM`-include (`pos::boost_di`), поэтому предупреждения чужого кода не попадают в нашу сборку.
Опция `-DPOS_BOOST_DI_INCLUDE_DIR=...` позволяет подменить заголовок системным, если он появится.

Контейнер compile-time: отсутствующая зависимость или цикл — ошибка компиляции, а не runtime.
Поэтому тестируется не «контейнер вообще», а собранный граф: `tests/app/composition_root_test.cpp`
проверяет общее состояние репозиториев, выбор формулы и округления. Подробнее — ADR-0004.

## Структура проекта

```
src/
├── domain/                 # правила, не знает про I/O
│   ├── model/              # Student, Criterion, Rubricator, Evaluation, VO
│   ├── services/           # EvaluationFactory, FinalScoreCalculator, Strategy, Decorator
│   └── ports/              # Reader/Writer, Clock
├── application/            # сценарии и DTO
│   ├── services/           # Student/Rubricator/Evaluation/FinalScore/Report
│   ├── ports/              # FinalScoreReport, ReportFactory, ReportWriter
│   ├── dto/                # StudentDto, CriterionDto, EvaluationDto, FinalScoreDto
│   └── pos_facade.*        # Facade для консоли
├── infrastructure/         # адаптеры
│   ├── persistence/        # InMemoryDatabase + reader/writer
│   ├── reports/            # Template Method, CSV/JSON, FileReportWriter
│   └── time/               # SystemClock
├── presentation/console/   # ConsoleApp, ConsoleIo, ConsoleView
├── app/                    # AppConfig, CLI, CompositionRoot, DemoScenario
└── main.cpp
tests/                      # 61 gtest-тест (domain/application/infrastructure/app)
third_party/boost-ext/di/   # вендорный Boost.DI v1.3.0
docs/adr/                   # архитектурные решения
cmake/                      # PosBoost, PosHelpers, PosTesting
```

---

## Тестирование

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

| Набор | Что проверяет |
|---|---|
| `domain_tests` | инварианты `Score`, `Criterion`, `Rubricator`, `EvaluationFactory`, `Evaluation::rescore`; формулы обеих стратегий; округление Decorator; LSP; проверки Domain Service (чужой студент, неизвестный критерий, дубликат) |
| `application_tests` | добавление/дубликаты студентов; выставление, исправление, отмена оценок; неизвестные студент/критерий; итоги и `assessed/total`; CSV/JSON и запись через порт |
| `infrastructure_tests` | CSV-экранирование запятых/кавычек; валидность JSON и пустого массива |
| `app_tests` | граф Boost.DI и общее состояние репозиториев; выбор формулы/округления; отказ при неверной конфигурации; сквозной демо-сценарий с файлами в временном каталоге |

Тесты детерминированы: `FixedClock` вместо системного времени, `TempDir` для файлов,
`CapturingReportWriter` вместо диска, уникальные временные каталоги. Флейки-тестов нет.

Дополнительно проверено:

```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset asan
```

61/61 тестов проходят и под AddressSanitizer + UndefinedBehaviorSanitizer.


---

## Обязательные вопросы: рефлексия по решениям

**1. Почему `Student` — Entity, а `StudentId`/`FullName` — Value Objects?**
Студент сохраняет идентичность при смене ФИО/группы; его сравнивают по номеру зачётки. ФИО и
группа — измерения без собственной жизни: два одинаковых ФИО взаимозаменяемы, поэтому это VO с
валидацией и value-equality.

**2. Где охраняется инвариант «балл не выходит за пределы критерия»?**
В `Score`: конструктор принимает `points` и `max_points` и не даёт создать объект вне диапазона.
Фабрика оценки передаёт туда `criterion.max_score()`, а `rescore` берёт сохранённый максимум.
Даже при вызове из другого сценария обойти правило нельзя.

**3. Почему `Evaluation` — отдельный агрегат, а не часть `Rubricator` или `Student`?**
Оценка имеет собственный жизненный цикл и идентичность; включение всех оценок в `Student` или
`Rubricator` раздуло бы агрегаты и создало бы конкуренцию при изменениях. Ссылки — только по id
(`StudentId`, `CriterionId`), согласованность итога считает Domain Service.

**4. Как обеспечена уникальность оценки на пару (студент, критерий), если это межагрегатное правило?**
`EvaluationService` перед созданием спрашивает `EvaluationReader::find_by_student_and_criterion` и
бросает `ConflictError`. В реальной БД это дополнительно закрывается уникальным индексом. Внутрь
`Evaluation` правило не поместить без загрузки всех оценок, а большой агрегат — хуже.

**5. Почему итог — Domain Service, а формула — Strategy?**
Правило «итог по студенту и рубрикатору» не принадлежит ни одному объекту: оно объединяет
`Rubricator`, набор `Evaluation` и студента. Это классический Domain Service. Сама формула —
изменяемая часть, поэтому вынесена в `FinalScorePolicy`; сервис отвечает за согласованность
данных, стратегия — за арифметику.

**6. Зачем Decorator, если можно добавить флаг округления в каждую стратегию?**
Тогда округление дублировалось бы в каждой формуле и нарушало бы OCP. Декоратор оборачивает любую
стратегию, включается конфигурацией (`--rounding`) и не меняет базовые классы; LSP сохраняется —
результат всегда `FinalScore` 0..100.

**7. Почему экспорт — Template Method, а не Strategy?**
У CSV и JSON общий каркас (заголовок, обход строк, разделители, подвал) и различается только
сериализация строки. Template Method фиксирует порядок шагов и убирает дублирование обхода;
Strategy здесь была бы менее выразительной, потому что меняется не алгоритм целиком, а шаги.

**8. Почему порты разделены на Reader и Writer?**
Принцип ISP: `FinalScoreService` нужно только чтение, `StudentService` — чтение и запись.
Узкие интерфейсы проще подменять в тестах и невозможно случайно вызвать `save` там, где сценарий
должен быть только читающим. `ReportWriter` — отдельный порт, чтобы application не знал про файлы.

**9. Почему DTO не считаются доменной моделью?**
DTO — плоские структуры для границы (консоль, отчёты), у них нет инвариантов и поведения. Домен
живёт в `src/domain`; преобразование выполняет `application/mapping.hpp`. Анемичный DTO допустим
именно на границе, анемичная доменная модель — нет.

**10. Что изменится при пересдачах или нескольких курсах?**
Пересдачи: в `Evaluation`/`EvaluationId` появится номер попытки, а итог — правило выбора
актуальной/лучшей попытки (новая Strategy). Несколько курсов: `Rubricator` станет агрегатом с
`CourseId`, оценки получат `CourseId`, а composition root будет выбирать курс. Порты и Domain
Service сохранят роли; появятся доменные события для межагрегатной согласованности.

**11. Почему Boost.DI вендорный, если в системе стоит Boost?**
Потому что Boost.DI не входит в Boost (это boost-ext.di) и в системном Boost 1.90 отсутствует.
Вендоринг официального заголовка v1.3.0 с закреплённым SHA256 выполняет требование «DI на
Boost.DI» и сохраняет офлайн-воспроизводимость. Альтернатива — самописный контейнер — была бы
хуже: меньше проверенного поведения и больше кода не по теме ДЗ.

---

## Роль ИИ

Проект реализован с помощью AI-ассистента по тексту домашнего задания: ассистент
предложил архитектуру, сгенерировал код, CMake, тесты, README и ADR. Все решения проверены
запуском: сборка GCC 16/C++23, 61/61 gtest-тестов, ASan/UBSan, ручной прогон `--demo`, просмотр
CSV/JSON.

Полное понимание архитектуры, список используемого стека технологий, написание чёткого ТЗ
для выполнения задания, осталось на стороне студента.

---

## Архитектурные решения (ADR)

- [ADR-0001: агрегаты и инварианты](docs/adr/0001-aggregates-and-invariants.md)
- [ADR-0002: Strategy и Decorator для расчёта итога](docs/adr/0002-final-score-strategy-and-decorator.md)
- [ADR-0003: порты хранения и экспорта](docs/adr/0003-ports-persistence-and-export.md)
- [ADR-0004: DI на Boost.DI](docs/adr/0004-di-with-boost-di.md)

---

## Лицензии сторонних компонентов

Boost.DI распространяется по Boost Software License 1.0 (текст — в
`third_party/boost-ext/di/LICENSE_1_0.txt`). Остальные зависимости берутся из системных пакетов
Boost 1.90 и GoogleTest 1.14 и используются по их лицензиям.


# ADR-0003: Порты хранения и экспорта

- **Статус:** принято
- **Контекст:** домен не должен знать про CSV/JSON/консоль; нужна возможность заменить in-memory
  хранение и способ выгрузки без правки домена.

## Решение

1. Порты хранения объявлены в `src/domain/ports/` и разделены по ISP:
   `StudentReader`/`StudentWriter`, `RubricatorReader`/`RubricatorWriter`,
   `EvaluationReader`/`EvaluationWriter`, `Clock`.
2. In-memory реализации — в `src/infrastructure/persistence/`, поверх единого
   `InMemoryDatabase`. Reader и Writer — разные объекты, но смотрят в одну БД.
3. Порты отчётов объявлены в `src/application/ports/final_score_report.hpp`:
   `FinalScoreReport` (рендеринг), `FinalScoreReportFactory` (выбор формата),
   `ReportWriter` (запись).
4. CSV/JSON реализованы в `src/infrastructure/reports/` через Template Method; запись файла —
   `FileReportWriter` на Boost.Filesystem. Application не знает про `ofstream` и Boost.
5. DTO (`StudentDto`, `CriterionDto`, `EvaluationDto`, `FinalScoreDto`) — только на границе
   приложения; домен их не использует.

## Обоснование

- Домен тестируется без файлов и БД; application — с fake/in-memory адаптерами.
- Новый способ хранения (SQLite/Postgres/HTTP) — новый адаптер и одна регистрация в DI.
- Новый формат отчёта — новый подкласс Template Method и ветка фабрики; Domain Service не
  затрагивается.
- Узкие интерфейсы не дают сценарию лишних прав (например, `ReportService` не пишет оценки).

## Последствия

- Больше интерфейсов и файлов, чем при «IGodRepository»; зато зависимости явные и тестируемые.
- In-memory БД не потокобезопасна и не транзакционна — осознанное ограничение прототипа.
- В реальной БД уникальность пары (студент, критерий) должна быть поддержана индексом, иначе
  application-проверка останется единственной защитой.

## Альтернативы

- **Прямое использование `std::fstream` в application** — отвергнуто: нарушает DIP и мешает тестам.
- **Сериализация в домене** — отвергнуто: домен не должен знать форматы.
- **Один «толстый» репозиторий** — отвергнуто по ISP и SRP.

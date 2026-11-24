#pragma once

#include <stdexcept>
#include <string>

namespace pos::domain {

/// Базовое исключение предметной области. Все нарушения инвариантов
/// выражаются наследниками этого типа (E.14 из C++ Core Guidelines).
class DomainError : public std::runtime_error {
public:
    explicit DomainError(const std::string& message) : std::runtime_error(message) {}
};

/// Нарушен инвариант значения/сущности (некорректный балл, пустое ФИО и т.п.).
class ValidationError final : public DomainError {
public:
    using DomainError::DomainError;
};

/// Запрошенный объект не найден.
class NotFoundError final : public DomainError {
public:
    using DomainError::DomainError;
};

/// Конфликт с текущим состоянием (дубликат, повторная оценка).
class ConflictError final : public DomainError {
public:
    using DomainError::DomainError;
};

}  // namespace pos::domain

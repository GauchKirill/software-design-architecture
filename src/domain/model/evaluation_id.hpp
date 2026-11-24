#pragma once

#include <cctype>
#include <string>
#include <string_view>

#include "domain/errors.hpp"
#include "domain/model/criterion_id.hpp"
#include "domain/model/student_id.hpp"
#include "domain/model/validation.hpp"

namespace pos::domain {

/// Идентификатор оценки. В текущей версии одна актуальная оценка на пару
/// (студент, критерий), поэтому идентификатор детерминированно строится из пары.
/// При появлении пересдач сюда добавится номер попытки.
class EvaluationId {
public:
    explicit EvaluationId(std::string value) : value_(detail::trim_copy(value)) {
        if (value_.empty()) {
            throw ValidationError("Идентификатор оценки не может быть пустым");
        }
        if (value_.size() > 100) {
            throw ValidationError("Идентификатор оценки длиннее 100 символов");
        }
        for (char ch : value_) {
            if (std::isspace(static_cast<unsigned char>(ch)) != 0) {
                throw ValidationError("Идентификатор оценки не должен содержать пробелы");
            }
        }
    }

    [[nodiscard]] static EvaluationId for_pair(const StudentId& student_id,
                                               const CriterionId& criterion_id) {
        return EvaluationId(student_id.value() + "::" + criterion_id.value());
    }

    [[nodiscard]] const std::string& value() const noexcept { return value_; }
    [[nodiscard]] std::string to_string() const { return value_; }

    auto operator<=>(const EvaluationId&) const = default;
    bool operator==(const EvaluationId&) const = default;

private:
    std::string value_;
};

}  // namespace pos::domain

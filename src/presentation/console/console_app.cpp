#include "presentation/console/console_app.hpp"

#include <format>
#include <string>
#include <string_view>
#include <utility>

#include "application/ports/final_score_report.hpp"
#include "presentation/console/console_view.hpp"

namespace pos::presentation {

ConsoleApp::ConsoleApp(std::shared_ptr<application::PosFacade> facade,
                       std::filesystem::path out_dir,
                       std::istream& in,
                       std::ostream& out)
    : facade_(std::move(facade)),
      out_dir_(std::move(out_dir)),
      out_(out),
      io_(in, out) {}

void ConsoleApp::run() {
    out_ << "Модуль ПОС — преподавательская оценка студентов\n";
    out_ << "Введите 0 для выхода.\n";

    while (!io_.eof()) {
        print_menu();
        const auto choice = io_.read_line("Пункт меню: ");
        if (io_.eof() && choice.empty()) {
            break;
        }
        if (choice.empty()) {
            continue;
        }
        if (choice == "0") {
            break;
        }
        try {
            dispatch(choice);
        } catch (const std::exception& error) {
            out_ << "Ошибка: " << error.what() << '\n';
        }
    }

    out_ << "Завершение работы.\n";
}

void ConsoleApp::print_menu() const {
    out_ << "\n1) Студенты\n"
            "2) Рубрикатор\n"
            "3) Оценки\n"
            "4) Итоги\n"
            "5) Экспорт итогов\n"
            "0) Выход\n";
}

void ConsoleApp::dispatch(std::string_view choice) {
    if (choice == "1") {
        students_menu();
    } else if (choice == "2") {
        rubricator_menu();
    } else if (choice == "3") {
        evaluations_menu();
    } else if (choice == "4") {
        final_scores_menu();
    } else if (choice == "5") {
        export_menu();
    } else {
        out_ << "Неизвестный пункт меню: " << choice << '\n';
    }
}

void ConsoleApp::students_menu() {
    while (!io_.eof()) {
        out_ << "\nСтуденты:\n  1) добавить\n  2) список\n  0) назад\n";
        const auto choice = io_.read_line("Действие: ");
        if (choice == "0" || choice.empty()) {
            return;
        }
        try {
            if (choice == "1") {
                const auto id = io_.read_line("Номер зачётки: ");
                const auto name = io_.read_line("ФИО: ");
                const auto group = io_.read_line("Группа: ");
                const auto student = facade_->add_student(id, name, group);
                out_ << std::format("Добавлен: {} ({}, {})\n",
                                    student.full_name, student.id, student.group);
            } else if (choice == "2") {
                print_students(out_, facade_->list_students());
            } else {
                out_ << "Неизвестное действие\n";
            }
        } catch (const std::exception& error) {
            out_ << "Ошибка: " << error.what() << '\n';
        }
    }
}

void ConsoleApp::rubricator_menu() {
    while (!io_.eof()) {
        out_ << "\nРубрикатор:\n  1) добавить критерий\n  2) список\n  0) назад\n";
        const auto choice = io_.read_line("Действие: ");
        if (choice == "0" || choice.empty()) {
            return;
        }
        try {
            if (choice == "1") {
                const auto id = io_.read_line("Код критерия: ");
                const auto name = io_.read_line("Название: ");
                const auto max_score = io_.read_int("Максимальный балл: ");
                const auto weight = io_.read_double("Вес: ");
                const auto criterion =
                    facade_->add_criterion(id, name, max_score, weight);
                out_ << std::format("Добавлен критерий: {} (max {}, вес {:.1f})\n",
                                    criterion.name, criterion.max_score, criterion.weight);
            } else if (choice == "2") {
                print_criteria(out_, facade_->list_criteria());
            } else {
                out_ << "Неизвестное действие\n";
            }
        } catch (const std::exception& error) {
            out_ << "Ошибка: " << error.what() << '\n';
        }
    }
}

void ConsoleApp::evaluations_menu() {
    while (!io_.eof()) {
        out_ << "\nОценки:\n"
                "  1) выставить\n"
                "  2) список по студенту\n"
                "  3) исправить\n"
                "  4) отменить\n"
                "  0) назад\n";
        const auto choice = io_.read_line("Действие: ");
        if (choice == "0" || choice.empty()) {
            return;
        }
        try {
            if (choice == "1") {
                const auto student_id = io_.read_line("Номер зачётки: ");
                const auto criterion_id = io_.read_line("Код критерия: ");
                const auto points = io_.read_int("Балл: ");
                const auto comment =
                    io_.read_optional("Комментарий (Enter — без комментария): ");
                const auto evaluation = facade_->record_evaluation(
                    student_id, criterion_id, points, comment);
                out_ << std::format("Оценка {}: {}/{} по критерию '{}'\n",
                                    evaluation.id,
                                    evaluation.points,
                                    evaluation.max_points,
                                    evaluation.criterion_name);
            } else if (choice == "2") {
                const auto student_id = io_.read_line("Номер зачётки: ");
                print_evaluations(out_, facade_->list_evaluations(student_id));
            } else if (choice == "3") {
                const auto evaluation_id = io_.read_line("ID оценки: ");
                const auto points = io_.read_int("Новый балл: ");
                const auto evaluation = facade_->correct_evaluation(evaluation_id, points);
                out_ << std::format("Оценка {} исправлена: {}/{}\n",
                                    evaluation.id,
                                    evaluation.points,
                                    evaluation.max_points);
            } else if (choice == "4") {
                const auto evaluation_id = io_.read_line("ID оценки: ");
                facade_->cancel_evaluation(evaluation_id);
                out_ << "Оценка отменена\n";
            } else {
                out_ << "Неизвестное действие\n";
            }
        } catch (const std::exception& error) {
            out_ << "Ошибка: " << error.what() << '\n';
        }
    }
}

void ConsoleApp::final_scores_menu() {
    while (!io_.eof()) {
        out_ << "\nИтоги:\n  1) по студенту\n  2) по всем\n  0) назад\n";
        const auto choice = io_.read_line("Действие: ");
        if (choice == "0" || choice.empty()) {
            return;
        }
        try {
            if (choice == "1") {
                const auto student_id = io_.read_line("Номер зачётки: ");
                const auto row = facade_->calculate_final_score(student_id);
                print_final_scores(out_, std::span{&row, 1});
            } else if (choice == "2") {
                print_final_scores(out_, facade_->calculate_all_final_scores());
            } else {
                out_ << "Неизвестное действие\n";
            }
        } catch (const std::exception& error) {
            out_ << "Ошибка: " << error.what() << '\n';
        }
    }
}

void ConsoleApp::export_menu() {
    try {
        const auto format = application::parse_report_format(
            io_.read_line("Формат (csv/json): "));
        const auto default_path =
            out_dir_ / ("final_scores." + application::to_string(format));
        auto path = io_.read_line(std::format("Путь [{}]: ", default_path.string()));
        if (path.empty()) {
            path = default_path.string();
        }
        facade_->export_report(format, path);
        out_ << "Отчёт записан: " << path << '\n';
    } catch (const std::exception& error) {
        out_ << "Ошибка: " << error.what() << '\n';
    }
}

}  // namespace pos::presentation

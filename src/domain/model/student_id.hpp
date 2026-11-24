#pragma once

#include "domain/model/string_id.hpp"

namespace pos::domain {

struct StudentIdTag;
using StudentId = StringId<StudentIdTag>;

}  // namespace pos::domain

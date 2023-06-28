#pragma once

namespace hyperkb {

class Concept;
class Role : public Concept {
public:
  bool is_role() const { return true; }
};

using RolePtr = std::shared_ptr<Role>;
}  // namespace hyperkb
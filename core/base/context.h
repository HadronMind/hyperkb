#pragma once

#include "base/concept.h"

namespace hyperkb {

class Context;
using ContextPtr = std::shared_ptr<Context>;

class Context : public Concept {};

}  // namespace hyperkb
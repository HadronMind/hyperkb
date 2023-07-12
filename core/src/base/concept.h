#pragma once

#include <limits>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "concept_repr.h"
#include "element.h"

namespace hyperkb {

class Concept;
using ConceptPtr = std::shared_ptr<Concept>;
class Context;
using ContextPtr = std::shared_ptr<Context>;
class Category;
using CategoryPtr = std::shared_ptr<Category>;

/**
 * @brief Base class to represent a general concept.
 *
 * We formulate an Entity-Relation model, as well as two supplementary
 * concepts of Role and Context to construct the whole picture.
 */
class Concept : public Element {
  friend class Category;

public:
  Concept(const std::string& iname, const CategoryPtr& category)
      : iname(iname), mCategory(category){};

  Concept(const std::string& iname, const CategoryPtr& category,
          const ElementPtr& parent_element, const ContextPtr& context)
      : iname(iname),
        mCategory(category),
        //     mParent(parent_element),
        mContext(context){};

  inline bool IsEntity() const { return false; }
  inline bool IsRelation() const { return false; }

  inline std::string GetInnerName() const { return iname; }
  inline std::weak_ptr<Category>& GetCategory() { return mCategory; }
  // inline ElementPtr& parent() { return mParent; }
  inline ContextPtr& GetContext() { return mContext; }

  /**
   * @brief Check the object is instantized from Concept subtype.
   *
   * @return boolean
   */
  template <typename T>
  inline typename std::enable_if_t<std::is_base_of<Concept, T>::value, bool>
  IsInstanceOf() const {
    return std::dynamic_pointer_cast<T>(*this) != nullptr;
  }

  /**
   * @brief Add new presentation to this concept. This operation always succeeds
   * if a valid modal is given.
   * @param repr
   * @param modal
   */
  void AddRepr(const ConceptReprPtr& repr, const ConceptRepr::REPR_MODAL modal);

  // TODO: add custom repr filter
  std::list<ConceptReprPtr> GetRepr(const ConceptRepr::REPR_MODAL modal) const;

  uint32_t ReprCount() const;

  uint32_t ReprCount(const ConceptRepr::REPR_MODAL modal) const;

  // Concept in string is denoted by curly braces.
  std::string ToString() const;

protected:
  explicit Concept(const Concept&) {}
  explicit Concept(Concept&&) {}

private:
  inline std::string _element_key_string(const ConceptPtr& ele) {
    return ele->iname;
  }

private:
  std::string iname;
  std::weak_ptr<Category> mCategory;

  // An element would have an empty (default) or single related context when
  // user provides. A context link would be created when more than one contexts
  // are related to the element.
  ContextPtr mContext;

  // stored representations
  std::map<ConceptRepr::REPR_MODAL, std::list<ConceptReprPtr>> repr_map;

  // TODO: support general properties
};

/**
 * Create ConceptPtr of specific subclass using appropriate constructor.
 */
template <typename T, typename... Args>
static inline typename std::enable_if_t<std::is_base_of<Concept, T>::value,
                                        std::shared_ptr<T>>
create_concept(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

/**
 * Cast ConceptPtr to the specific Concept subclass.
 */
template <typename T>
static inline typename std::enable_if_t<std::is_base_of<Concept, T>::value,
                                        std::shared_ptr<T>>
cast_from_concept(const ConceptPtr& concept) {
  return std::dynamic_pointer_cast<T>(concept);
}

/**
 * Cast specific Concept subclass to ConceptPtr.
 */
template <typename T>
static inline
    typename std::enable_if_t<std::is_base_of<Concept, T>::value, ConceptPtr>
    cast_to_concept(const std::shared_ptr<const T>& concept) {
  return std::dynamic_pointer_cast<Concept>(
      std::const_pointer_cast<T>(concept));
}

}  // namespace hyperkb

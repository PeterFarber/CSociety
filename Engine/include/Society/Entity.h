//
// Created by Peter Farber on 5/13/24.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <Society/Components/Component.h>
#include <Society/Core/Base.h>

#define INSTANCEOF_REF(derivedType, baseREF) (dynamic_cast<derivedType*>(&baseREF) != nullptr)
#define INSTANCEOF_PTR(derivedType, basePTR) (dynamic_cast<derivedType*>(basePTR) != nullptr)

namespace sc {
    class Component;

    class DECLSPEC Entity {
        protected:
            std::vector<Component *> m_components;

        public:
            virtual ~Entity() = default;

            template<typename T>
            T *FindComponent() {
                for (auto component: m_components) {
                    if (component->isA<T>()) {
                        return dynamic_cast<T *>(component);
                    }
                }
                return nullptr;
            }

            virtual void Update(const double deltaTime) {
                for (const auto component: m_components) {
                    component->Update(deltaTime);
                }
            }
    };
} // sc

#endif //ENTITY_H

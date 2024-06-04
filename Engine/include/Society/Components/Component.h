//
// Created by Peter Farber on 5/13/24.
//

#ifndef COMPONENT_H
#define COMPONENT_H

namespace sc {
    class Entity;

    class Component {
        protected:
            Entity *m_entity = nullptr;

        public:
            virtual ~Component() = default;

            virtual void Update(double deltaTime);

            template<typename T>
            bool isA() {
                {
                    return (dynamic_cast<T *>(this) != nullptr);
                }
            }
    };
}

#endif //COMPONENT_H

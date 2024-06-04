//
// Created by Peter Farber on 5/23/24.
//

#ifndef SC_SPHERE_H
#define SC_SPHERE_H
#include "PrimitiveMesh.h"

namespace sc {
    constexpr unsigned int X_SEGMENTS = 64;
    constexpr unsigned int Y_SEGMENTS = 64;
    constexpr float PI = 3.14159265359f;

    class Sphere final : public PrimitiveMesh {

        private:
            unsigned int m_indexCount;

        public:
            Sphere();

            void Draw() override;

    };
}
#endif //SPHERE_H

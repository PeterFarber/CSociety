//
// Created by Peter Farber on 5/23/24.
//

#ifndef SC_CUBE_H
#define SC_CUBE_H
#include "PrimitiveMesh.h"

namespace sc {
    class Cube final : public PrimitiveMesh {

        private:

        public:
            Cube();

            void Draw() override;

    };
}
#endif //SPHERE_H

//
// Created by Peter Farber on 5/23/24.
//

#ifndef SC_QUAD_H
#define SC_QUAD_H
#include "PrimitiveMesh.h"

namespace sc {
    class Quad final : public PrimitiveMesh {

        private:

        public:
            Quad();

            void Draw() override;

    };
}
#endif //SPHERE_H

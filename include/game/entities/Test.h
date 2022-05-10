//
// Created by Jacopo Beragnoli on 09/05/22.
//

#ifndef MINIMINIMOTORWAYS_TEST_H
#define MINIMINIMOTORWAYS_TEST_H

#include "../../engine/Core.h"

namespace Game {
    class Test : public Engine::ECS::Entity {
    public:
        explicit Test(id_t entityId) : Entity(entityId) {}
    };
}


#endif //MINIMINIMOTORWAYS_TEST_H
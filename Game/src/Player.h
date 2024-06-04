//
// Created by Peter Farber on 5/13/24.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <Society/Entity.h>


class Player : public sc::Entity {
public:
    Player();

    void Update(double deltaTime) override;
};


#endif

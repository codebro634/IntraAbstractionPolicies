#pragma once

#ifndef MULTIPLAYEROTMDP_H
#define MULTIPLAYEROTMDP_H
#include <map>
#include <set>
#include <vector>

#include "../Gamestate.h"
#include "../../Agents/Agent.h"

namespace MPTOMDP
{

    struct Gamestate: public ABS::Gamestate{
        ABS::Gamestate* ground_state;
        bool operator==(const ABS::Gamestate& other) const override;
        [[nodiscard]] size_t hash() const override;

        ~Gamestate() override {
            delete ground_state;
        }
    };

    class Model: public ABS::Model
    {
    public:
        ~Model() override;
        explicit Model(ABS::Model* original_model,std::map<int,Agent*> agents, double discount, int player, bool deterministic_opponents);
        void printState(ABS::Gamestate* state) override;
        ABS::Gamestate* getInitialState(std::mt19937& rng) override;
        ABS::Gamestate* getInitialState(int num) override;
        ABS::Gamestate* copyState(ABS::Gamestate* uncasted_state) override;
        int getNumPlayers() override;
        bool hasTransitionProbs() override;

        [[nodiscard]] std::vector<int> obsShape() const override {
            return original_model->obsShape();
        }

        [[nodiscard]] std::vector<int> actionShape() const override {
            return original_model->actionShape();
        }

        void getObs(ABS::Gamestate* uncasted_state, int* obs) override {
            original_model->getObs(dynamic_cast<Gamestate*>(uncasted_state)->ground_state, obs);
        }

        [[nodiscard]] int encodeAction(ABS::Gamestate* state, int* decoded_action, bool* valid) override {
            return original_model->encodeAction(dynamic_cast<Gamestate*>(state)->ground_state, decoded_action, valid);
        }

    private:
        ABS::Model* original_model;
        std::map<int,Agent*> agents;
        double discount;
        int player;
        bool deterministic_opponents;

        const static int ENEMY_SEED = 42;

        std::pair<double,double> playTillNextTurn(Gamestate* state, std::mt19937& rng);

        std::pair<std::vector<double>,double> applyAction_(ABS::Gamestate* uncasted_state, int action, std::mt19937& rng, std::vector<std::pair<int,int>>* decision_outcomes) override;
        std::vector<int> getActions_(ABS::Gamestate* uncasted_state) override;
    };

}

#endif


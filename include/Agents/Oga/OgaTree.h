#ifndef OGATREE_H
#define OGATREE_H

#include <map>
#include <set>

#include "OgaAbstractNodes.h"
#include "OgaUtils.h"
#include "../../Utils/ValueIteration.h"

namespace OGA {

    struct OgaSearchStats;

    class OgaTree
    {
    private:

        /* Behavior modifiers */
        OgaBehaviorFlags behavior_flags;

        OgaStateNode* root;
        ABS::Model* model;
        Set<OgaStateNode> d_states{}; //Contains all state-nodes in the tree
        Set<OgaQStateNode> q_states{}; //Contains all q-state-nodes in the tree

        //Helper, redundant data structure for efficiency. WARNING: Thse Maps may contain abstract nodes / distribution that are no longer part of the tree
        std::vector<Map<OgaQStateNode, NextDistribution*>> next_distribution_map{}; //Saves the latest calculated NextDistribution for each q state
        std::vector<Map<NextDistribution, OgaAbstractQStateNode*>> abstract_q_state_node_map{};
        std::vector<Map<NextAbstractQStates, OgaAbstractStateNode*>> abstract_state_node_map{};


        std::vector<OgaAbstractStateNode*> terminal_abstract_state_nodes{}; //Abstract states that represent the terminal states for each depth.
        std::vector<OgaAbstractStateNode*> unexplored_abstract_state_nodes{}; //Abstract states that represent the unexplored states for each depth.

        // Contains all abstract non-empty nodes of the tree
        std::map<int,AbsStateSet> abstract_state_nodes{};
        std::map<int,AbsQSet> abstract_q_state_nodes{};

        //All abstract nodes that are no longer part of the tree because they are empty. Only neede for cleanup at the end of search
        std::set<OgaAbstractQStateNode*> abstract_q_corpses;
        std::set<OgaAbstractStateNode*> abstract_state_corpses;

        //Helper sets for breadth-first updating to prevent multi updates. Cleared after each update.
        std::vector<Set<OgaStateNode>> to_update_states{};
        std::vector<Set<OgaQStateNode>> to_update_q_states{};

        void _stageForUpdate(OgaStateNode* state_node);
        void _stageForUpdate(OgaQStateNode* q_state_node);

        void updateQAbstractions(unsigned K, int depth);
        void updateStateAbstractions(unsigned K, int depth, OgaSearchStats& search_stats, std::mt19937& rng);

    public:
        explicit OgaTree(ABS::Gamestate* root_state, ABS::Model* model, const OgaBehaviorFlags& behavior_flags, std::mt19937& rng);
        ~OgaTree();

        [[nodiscard]] OgaStateNode* getRoot() const;

        [[nodiscard]] std::pair<OgaStateNode*, bool> findOrCreateState(ABS::Gamestate* state, unsigned depth, std::mt19937& rng);
        [[nodiscard]] std::pair<OgaQStateNode*, bool> findOrCreateQState(ABS::Gamestate* state, unsigned depth, int action, std::mt19937& rng);

        void insert(OgaStateNode* state_node);
        void insert(OgaQStateNode* state_node);

        void performUpdateAbstractions(unsigned K, OgaSearchStats& search_stats, std::mt19937& rng);
        void addUpdateQStateNodeAbstraction(OgaQStateNode* q_state_node);
        void addUpdateStateNodeAbstraction(OgaStateNode* state_node);

        // Debugging/Statistics functions
        void printAbsTree(ABS::Model* model, size_t layers) const;

    };

}

#endif //OGATREE_H

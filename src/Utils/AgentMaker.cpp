#include "../../include/Utils/AgentMaker.h"
#include "../../include/Agents/Oga/OgaAgent.h"
#include "../../include/Agents/Mcts/MctsAgent.h"
#include "../../include/Agents/LookupTableAgent.h"
#include "../../include/Agents/RandomAgent.h"

#include <map>
#include <set>


Agent* getDefaultAgent(bool strong){
    if (strong)
        return new Mcts::MctsAgent({{500, "iterations"}, {4}, 1.0, 1, -1, true, true});
    else
       return new RandomAgent();
}

std::string extraArgs(std::map<std::string, std::string>& given_args, const std::set<std::string>& acceptable_args){
    for (auto& [key, val] : given_args) {
        if(!acceptable_args.contains(key))
            return key;
    }
    return "";
}

Agent* getAgent(const std::string& agent_type, const std::vector<std::string>& a_args)
{

    //Parse named args
    std::map<std::string, std::string> agent_args;
    for(auto &arg   : a_args) {
        //split at '='
        auto pos = arg.find('=');
        if (pos == std::string::npos) {
            std::cout << "Invalid agent argument: " << arg << ". It must be of the form arg_name=arg_val" << std::endl;
            return nullptr;
        }
        agent_args[arg.substr(0, pos)] = arg.substr(pos + 1);
    }
    std::set<std::string> acceptable_args;

    Agent* agent;
    if (agent_type == "random") {
        acceptable_args = {};
        agent =  new RandomAgent();
    }
    else if(agent_type == "mcts")
    {
        assert (agent_args.contains("iterations"));
        if(agent_args.contains("wirsa"))
            assert (agent_args.contains("a") && agent_args.contains("b"));
        acceptable_args = {"iterations", "rollout_length", "discount", "num_rollouts", "dag", "dynamic_exp_factor", "expfacs", "wirsa", "a", "b"};

        int iterations = std::stoi(agent_args["iterations"]);
        int rollout_length = agent_args.find("rollout_length") == agent_args.end() ? -1 : std::stoi(agent_args["rollout_length"]);
        double discount = agent_args.find("discount") == agent_args.end() ? 1.0 : std::stod(agent_args["discount"]);
        int num_rollouts = agent_args.find("num_rollouts") == agent_args.end() ? 1: std::stoi(agent_args["num_rollouts"]);
        bool dag = agent_args.find("dag") == agent_args.end() ? false : std::stoi(agent_args["dag"]);
        bool dynamic_exp_factor = agent_args.find("dynamic_exp_factor") == agent_args.end() ? false : std::stoi(agent_args["dynamic_exp_factor"]);
        bool wirsa = agent_args.find("wirsa") == agent_args.end() ? false : std::stoi(agent_args["wirsa"]);
        double a = agent_args.find("a") == agent_args.end() ? 0.0 : std::stod(agent_args["a"]);
        double b = agent_args.find("b") == agent_args.end() ? 0.0 : std::stod(agent_args["b"]);
        std::string exp_facs = agent_args.find("expfacs") == agent_args.end() ? "1" : agent_args["expfacs"];
        std::vector<double> expfac;
        std::stringstream ss(exp_facs);
        double i;
        while (ss >> i){
            expfac.push_back(i);
            if (ss.peek() == ';')
                ss.ignore();
        }
        auto args = Mcts::MctsArgs{.budget = {iterations, "iterations"}, .exploration_parameters = expfac, .discount = discount,
            .num_rollouts = num_rollouts,
            .rollout_length = rollout_length,
            .dag=dag,
            .dynamic_exploration_factor=dynamic_exp_factor,
            .wirsa = wirsa,
            .a=a,.b=b};
        agent =  new Mcts::MctsAgent(args);
    }
     else if (agent_type == "oga") {
        assert (agent_args.contains("iterations"));
        acceptable_args = {"iterations", "discount", "expfac", "K", "exact_bookkeeping", "group_terminal_states", "group_partially_expanded_states", "equiv_chance",
            "partial_expansion_group_threshold", "ignore_partially_expanded_states", "eps_a", "eps_t", "in_abs_policy",
             "num_rollouts", "rollout_length"};

        int iterations = std::stoi(agent_args["iterations"]);
        double discount = agent_args.find("discount") == agent_args.end() ? 1.0 : std::stod(agent_args["discount"]);
        double expfac = agent_args.find("expfac") == agent_args.end() ? 2.0 : std::stod(agent_args["expfac"]);
        unsigned K = agent_args.find("K") == agent_args.end() ? 1 : std::stoi(agent_args["K"]);
        bool exact_bookkeeping = agent_args.find("exact_bookkeeping") == agent_args.end() ? true : std::stoi(agent_args["exact_bookkeeping"]);
        bool group_terminal_states = agent_args.find("group_terminal_states") == agent_args.end() ? true : std::stoi(agent_args["group_terminal_states"]);
        bool group_partially_expanded_states = agent_args.find("group_partially_expanded_states") == agent_args.end() ? false : std::stoi(agent_args["group_partially_expanded_states"]);
        unsigned partial_expansion_group_threshold = agent_args.find("partial_expansion_group_threshold") == agent_args.end() ? std::numeric_limits<int>::max() : std::stoi(agent_args["partial_expansion_group_threshold"]);
        double eps_a = agent_args.find("eps_a") == agent_args.end() ? 0.0 : std::stod(agent_args["eps_a"]);
        double eps_t = agent_args.find("eps_t") == agent_args.end() ? 0.0 : std::stod(agent_args["eps_t"]);
        int num_rollouts = agent_args.find("num_rollouts") == agent_args.end() ? 1 : std::stoi(agent_args["num_rollouts"]);
        int rollout_length = agent_args.find("rollout_length") == agent_args.end() ? -1 : std::stoi(agent_args["rollout_length"]);
         std::string in_abs_policy = agent_args.find("in_abs_policy") == agent_args.end() ? "random" : agent_args["in_abs_policy"];

        auto args = OGA::OgaArgs{
            .budget = {iterations, "iterations"},
            .recency_count_limit = K,
            .exploration_parameter = expfac,
            .discount = discount,
            .num_rollouts = num_rollouts,
            .rollout_length = rollout_length,
            .behavior_flags = {
                .exact_bookkeeping=exact_bookkeeping,
                .group_terminal_states=group_terminal_states,
                .group_partially_expanded_states=group_partially_expanded_states,
                .partial_expansion_group_threshold=partial_expansion_group_threshold,
                .eps_a = eps_a,
                .eps_t = eps_t,
            },
            .in_abs_policy = in_abs_policy,
            .track_statistics = false,
        };
        agent =  new OGA::OgaAgent(args);
    }
     else{
        throw std::runtime_error("Invalid agent");
    }

    if (agent != nullptr) {
        if (!extraArgs(agent_args, acceptable_args).empty()) {
            std::string err_string = "Invalid agent argument: " + extraArgs(agent_args, acceptable_args);
            std::cout << err_string << std::endl;
            throw std::runtime_error(err_string);
        }
        return agent;
    }else {
        std::cout << "Invalid agent" << std::endl;
        throw std::runtime_error("Invalid agent");
    }
}
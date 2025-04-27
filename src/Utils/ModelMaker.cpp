#include "../../include/Utils/ModelMaker.h"
#include "../../include/Agents/Mcts/MctsAgent.h"
#include "../../include/Agents/RandomAgent.h"
#include "../../include/Agents/Agent.h"

#include "../../include/Utils/AgentMaker.h"

#include "../../include/Games/MDPs/SailingWind.h"
#include "../../include/Games/MDPs/Navigation.h"
#include "../../include/Games/MDPs/SkillsTeaching.h"
#include "../../include/Games/MDPs/PushYourLuck.h"
#include "../../include/Games/MDPs/SysAdmin.h"
#include "../../include/Games/MDPs/TriangleTireworld.h"
#include "../../include/Games/Wrapper/MultiPlayerToMDP.h"
#include "../../include/Games/MDPs/EarthObservation.h"
#include "../../include/Games/MDPs/Manufacturer.h"
#include "../../include/Games/MDPs/GameOfLife.h"
#include "../../include/Games/MDPs/Traffic.h"
#include "../../include/Games/MDPs/Tamarisk.h"
#include "../../include/Games/MDPs/RaceTrack.h"
#include "../../include/Games/MDPs/CrossingTraffic.h"
#include "../../include/Games/MDPs/AcademicAdvising.h"
#include "../../include/Games/MDPs/CooperativeRecon.h"

#include <map>
#include <set>
#include <bits/ranges_algo.h>

void checkArguments(std::string model, std::map<std::string, std::string> model_args) {
    std::map<std::string,std::pair<std::set<std::string>,std::set<std::string>>> model_arg_map;
    model_arg_map["mptomdp"] = {{"model_type", "model_args", "player"}, {"model_type", "model_args", "agents", "player", "discount", "deterministic_opponents"}};
    model_arg_map["num"] = {{"goal", "max_action", "zero_sum"}, {"zero_sum", "goal", "max_action"}};
    model_arg_map["saving"] = { {"p", "t"}, {"p", "t"}};
    model_arg_map["ts"] = { {}, {}};
    model_arg_map["che"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["con"] = {{"zero_sum", "arena_size"}, {"zero_sum", "arena_size"}};
    model_arg_map["pyl"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["qua"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["ct"] = {{"width", "height", "spawn_rate", "idle_action"}, {"width", "height", "spawn_rate", "idle_action"}};
    model_arg_map["mab"] = {{"repeats", "means", "stds"}, {"repeats", "means", "stds"}};
    model_arg_map["pushyl"] = {{"map"}, {"map"}};
    model_arg_map["rfbe"] = {{"map"}, {"map", "deterministic_spread"}};
    model_arg_map["aa" ] = {{"map", "dense_rewards"}, {"map", "dense_rewards", "idle_action"}};
    model_arg_map["sa"] = {{"map"}, {"map"}};
    model_arg_map["eo"] = {{"map"}, {"map"}};
    model_arg_map["sw"] = {{"size"}, {"size", "deterministic"}};
    model_arg_map["rt"] = {{"map"}, {"map", "reset_at_crash", "fail_prob"}};
    model_arg_map["gol"] = {{"map"}, {"map", "action_mode"}};
    model_arg_map["j5"] = {{"joint","decoupled_action_space"}, {"joint","decoupled_action_space"}};
    model_arg_map["trt"] = {{"map"}, {"map", "idle_action", "reduced_action_space"}};
    model_arg_map["wf"] = {{"map"}, {"map"}};
    model_arg_map["wlp"] = {{"map"}, {"map"}};
    model_arg_map["ctf"] = {{"zero_sum","map"}, {"zero_sum","map"}};
    model_arg_map["c4"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["oth"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["pus"] = {{"zero_sum","map"}, {"zero_sum","map"}};
    model_arg_map["ktk"] = {{"zero_sum","map"}, {"zero_sum","map"}};
    model_arg_map["ttt"] = {{"zero_sum"}, {"zero_sum"}};
    model_arg_map["st"] = {{"map"}, {"map", "idle_action", "reduced_action_space"}};
    model_arg_map["tr"] = {{"map"}, {"map"}};
    model_arg_map["man"] = {{"map"}, {"map"}};
    model_arg_map["gtr"] = {{"map"}, {"map"}};
    model_arg_map["ele"] = {{"map"}, {"map"}};
    model_arg_map["tam"] = {{"map"}, {"map"}};
    model_arg_map["recon"] = {{"map"}, {"map"}};
    model_arg_map["navigation"] = {{"map"}, {"map", "idle_action"}};

    //check if model exists
    if (!model_arg_map.contains(model)) {
        std::cerr << "The model '" << model << "' is not recognized. Please choose one of the following values:" << std::endl;
        std::cerr << "aa (Academic Advising), recon (Cooperative Recon), ct (Crossing Traffic), eo (Earth Observation), ele (Elevators), gol (Game of Life), gtr (Graph Traversal), "
                     "man (Manufacturer), j5 (Join Five), mab (Multi-armed bandit), navigation (Navigation), "
                     "pushyl (Push Your Luck), rt (Racetrack), rfbe (Red Finned Blue Eye), sw (Sailing Wind), saving (Saving), st (Skill Teaching), sa (SysAdmin), "
                     "tam (Tamarisk), ts (Toy Soccer), tr (Traffic), trt (Triangle Tireworld), wf (Wildfire), "
                     "wlp (Wildlife Preserve), ctf (Capture the Flag), che (Chess), c4 (Connect 4), con (Constrictor), ktk (Kill the King), num (Numbers Race),"
                     "oth (Othello), pus (Pusher), pyl (Pylos), qua (Quarto), ttt (TicTacToe)" << std::endl;
        throw std::runtime_error("");
    }

    //check for missing required arguments
    for (auto& arg : model_arg_map[model].first){
        if (!model_args.contains(arg)){
            std::cerr <<"Model " << model << " is missing required argument: " << arg << std::endl;
            std::cerr<< "Required arguments are: ";
            for (auto& arg : model_arg_map[model].first){
                std::cerr << arg << " ";
            }
            std::cerr << std::endl;
            throw std::runtime_error("");
        }
    }

    //check for invalid arguments
    for (auto& arg : model_args){
        if (!model_arg_map[model].second.contains(arg.first)){
            std::cerr <<"Model " << model << " has invalid argument: " << arg.first <<"." << std::endl;
            std::cerr << "Valid arguments are: ";
            for (auto& arg : model_arg_map[model].second){
                std::cerr << arg << " ";
            }
            std::cerr << std::endl;
            throw std::runtime_error("");
        }
    }
}

std::string decode_alternative_name_formulation(const std::string& model_type) {
    std::map<std::string,std::string> alternatives_map;
    alternatives_map["numbers race"] = "num";
    alternatives_map["saving"] = "saving";
    alternatives_map["tictactoe"] = "ts";
    alternatives_map["tic tac toe"] = "ts";
    alternatives_map["chess"] = "che";
    alternatives_map["constrictor"] = "con";
    alternatives_map["pylos"] = "pyl";
    alternatives_map["quarto"] = "qua";
    alternatives_map["crossing traffic"] = "ct";
    alternatives_map["multi-armed bandit"] = "mab";
    alternatives_map["multi armed bandit"] = "mab";
    alternatives_map["multiarmedbandit"] = "mab";
    alternatives_map["push your luck"] = "pushyl";
    alternatives_map["pushyourluck"] = "pushyl";
    alternatives_map["red finned blue eye"] = "rfbe";
    alternatives_map["redfinnedblueeye"] = "rfbe";
    alternatives_map["academic advising"] = "aa";
    alternatives_map["academicadvising"] = "aa";
    alternatives_map["sysadmin"] = "sa";
    alternatives_map["sys admin"] = "sa";
    alternatives_map["earth observation"] = "eo";
    alternatives_map["earthobservation"] = "eo";
    alternatives_map["sailing wind"] = "sw";
    alternatives_map["sailingwind"] = "sw";
    alternatives_map["racetrack"] = "rt";
    alternatives_map["game of life"] = "gol";
    alternatives_map["gameoflife"] = "gol";
    alternatives_map["join five"] = "j5";
    alternatives_map["joinfive"] = "j5";
    alternatives_map["triangle tireworld"] = "trt";
    alternatives_map["triangletireworld"] = "trt";
    alternatives_map["wildfire"] = "wf";
    alternatives_map["wildlife preserve"] = "wlp";
    alternatives_map["wildlifepreserve"] = "wlp";
    alternatives_map["capture the flag"] = "ctf";
    alternatives_map["capturetheflag"] = "ctf";
    alternatives_map["connect 4"] = "c4";
    alternatives_map["connect4"] = "c4";
    alternatives_map["othello"] = "oth";
    alternatives_map["pusher"] = "pus";
    alternatives_map["kill the king"] = "ktk";
    alternatives_map["killtheking"] = "ktk";
    alternatives_map["tic tac toe"] = "ttt";
    alternatives_map["tictactoe"] = "ttt";
    alternatives_map["skill teaching"] = "st";
    alternatives_map["skillteaching"] = "st";
    alternatives_map["traffic"] = "tr";
    alternatives_map["manufacturer"] = "man";
    alternatives_map["graph traversal"] = "gtr";
    alternatives_map["graphtraversal"] = "gtr";
    alternatives_map["elevators"] = "ele";
    alternatives_map["tamarisk"] = "tam";
    alternatives_map["cooperative recon"] = "recon";
    alternatives_map["cooperativerecon"] = "recon";
    alternatives_map["navigation"] = "navigation";

    //model type to lowercase
    std::string model_type_lower = model_type;
    std::transform(model_type_lower.begin(), model_type_lower.end(), model_type_lower.begin(), ::tolower);

    return alternatives_map.contains(model_type_lower) ? alternatives_map[model_type_lower] : model_type_lower;
}

ABS::Model* getModel(std::string model_type, const std::vector<std::string>& m_args, int horizon){

    model_type = decode_alternative_name_formulation(model_type);
    
    std::map<std::string, std::string> model_args;
    for(auto &arg : m_args) {
        //split at '='
        auto pos = arg.find('=');
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid agent argument");
        }
        model_args[arg.substr(0, pos)] = arg.substr(pos + 1);
    }
    checkArguments(model_type, model_args);

    ABS::Model *model = nullptr;
    std::set<std::string> acceptable_args;


    if (model_type == "ct") {
        bool idle_action = model_args.contains("idle_action") ? std::stoi(model_args["idle_action"]) : true;
        model =  new CT::Model(std::stoi(model_args["width"]), std::stoi(model_args["height"]), std::stof(model_args["spawn_rate"]), idle_action);
    }
    else if(model_type == "man"){
        model = new MAN::Model(model_args["map"]);
    }
   else if (model_type == "tam") {
        model =  new TAM::Model(model_args["map"]);
    }else if(model_type == "recon") {
        model = new RECON::ReconModel(model_args["map"]);
    }
    else if(model_type == "tr") {
        model = new TR::TrafficModel(model_args["map"]);
    }
    else if(model_type == "st") {
        bool idle_action = model_args.contains("idle_action") ? std::stoi(model_args["idle_action"]) : false;
        bool reduced_action_space = model_args.contains("reduced_action_space") ? std::stoi(model_args["reduced_action_space"]) : true;
        model = new ST::SkillsTeachingModel(model_args["map"],idle_action, reduced_action_space);
    }else if(model_type == "trt") {
        bool idle_action = model_args.contains("idle_action") ? std::stoi(model_args["idle_action"]) : false;
        bool reduced_action_space = model_args.contains("reduced_action_space") ? std::stoi(model_args["reduced_action_space"]) : true;
        model = new TRT::Model(model_args["map"],idle_action, reduced_action_space);
    }
    else if (model_type == "sa") {
        model = new SA::Model(model_args["map"]);
    }
    else if (model_type == "eo"){
        model = new EO::Model(model_args["map"]);
    }
    else if (model_type == "sw") {
        bool deterministic = model_args.contains("deterministic") ? std::stoi(model_args["deterministic"]) : false;
        model =  new SW::Model(std::stoi(model_args["size"]), std::stoi(model_args["size"]), deterministic);
    }
    else if (model_type == "rt") {
        bool reset_at_crash = model_args.contains("reset_at_crash") ? std::stoi(model_args["reset_at_crash"]) : false;
        double fail_prob = model_args.contains("fail_prob") ? std::stod(model_args["fail_prob"]) : 0.0;
        model =  new RT::Model(model_args["map"], fail_prob, reset_at_crash);
    }
    else if (model_type == "gol") {
        GOL::ActionMode action_mode = GOL::ActionMode::SAVE_ONLY;
        if (model_args.contains("action_mode")){
            if (model_args["action_mode"] == "all")
                action_mode = GOL::ActionMode::ALL;
            else if (model_args["action_mode"] == "save_only")
                action_mode = GOL::ActionMode::SAVE_ONLY;
            else if (model_args["action_mode"] == "revive_only")
                action_mode = GOL::ActionMode::REVIVE_ONLY;
            else{
                std::cout << "Invalid action mode" << std::endl;
                throw std::runtime_error("Invalid action mode");
            }
        }
        model =  new GOL::Model(model_args["map"],action_mode);
    }
    else if (model_type == "aa") {
        bool idle_action = model_args.contains("idle_action") ? std::stoi(model_args["idle_action"]) : false;
        model =  new AA::Model(model_args["map"], std::stoi(model_args["dense_rewards"]), idle_action);
    }
    else if (model_type == "navigation") {
        bool idle_action = model_args.contains("idle_action") ? std::stoi(model_args["idle_action"]) : false;
        model =  new Navigation::Model(model_args["map"],idle_action);
    }

    assert (model != nullptr);

    //wrap in finite horizon
    if (horizon > 0){
        auto wrapped_model = new FINITEH::Model(model, (size_t)horizon, true);
        return wrapped_model;
    }else
        return model;

}

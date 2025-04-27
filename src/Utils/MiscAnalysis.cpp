#include "../../include/Utils/MiscAnalysis.h"
#include "../../include/Arena.h"
#include "../../include/Utils/ValueIteration.h"
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../../include/Games/MDPs/SailingWind.h"
#include "../../include/Games/MDPs/Navigation.h"
#include "../../include/Games/MDPs/SkillsTeaching.h"
#include "../../include/Games/MDPs/SysAdmin.h"
#include "../../include/Games/MDPs/TriangleTireworld.h"
#include "../../include/Games/MDPs/EarthObservation.h"
#include "../../include/Games/MDPs/Manufacturer.h"
#include "../../include/Games/MDPs/GameOfLife.h"
#include "../../include/Games/MDPs/CrossingTraffic.h"
#include "../../include/Games/MDPs/Tamarisk.h"
#include "../../include/Games/Wrapper/FiniteHorizon.h"
#include "../../include/Games/MDPs/RaceTrack.h"
#include "../../include/Games/MDPs/AcademicAdvising.h"
#include "../../include/Games/MDPs/Traffic.h"
#include "../../include/Games/MDPs/CooperativeRecon.h"
#include "../../include/Utils/Argparse.h"
#include "../../include/Utils/Distributions.h"
#include "../../include/Agents/Oga/OgaAgent.h"

#include <iostream>

namespace MISC {
    double round(double d, int precision) {
        return std::round(d * std::pow(10, precision)) / std::pow(10, precision);
    }

    void measureIntraAbsRate(){
        std::vector<std::pair<std::string,ABS::Model*>> model_list = {};

        // model_list.emplace_back("wf", new  WF::Model("../resources/WildfireSetups/1_IPPC.txt"));
        // model_list.emplace_back("rfbe", new  RFBE::Model("../resources/RedFinnedBlueEyesMaps/1_IPPC.txt", false));
        // model_list.emplace_back("ele", new  ELE::Model("../resources/ElevatorSetups/10_IPPC.txt", false));

        model_list.emplace_back( "Sailing Wind", new  SW::Model(15,15, false));
        model_list.push_back({"Academic Advising",new  AA::Model("../resources/AcademicAdvisingCourses/2_Anand.txt",false,false)});
        model_list.push_back({"Cooperative Recon", new  RECON::ReconModel("../resources/CooperativeReconSetups/3_IPPC.txt")});
        model_list.emplace_back("Game of Life", new  GOL::Model("../resources/GameOfLifeMaps/3_Anand.txt", GOL::ActionMode::SAVE_ONLY));
        model_list.emplace_back("Earth Observation", new  EO::Model("../resources/EarthObservationMaps/1_IPPC.txt"));
        model_list.emplace_back("Manufacturer", new  MAN::Model("../resources/ManufacturerSetups/3_IPPC.txt"));
        model_list.emplace_back("Navigation", new  Navigation::Model("../resources/NavigationMaps/3_Anand.txt",false));
        model_list.emplace_back("Racetrack", new  RT::Model("../resources/Racetracks/ring-2.track", 0.0, false));
        model_list.emplace_back("Skills Teaching", new  ST::SkillsTeachingModel("../resources/SkillsTeachingSkills/5_IPPC.txt",false, true));
        model_list.emplace_back( "Crossing Traffic" , new  CT::Model(4,3,0.5, true));
        model_list.emplace_back("SysAdmin ", new  SA::Model("../resources/SysAdminTopologies/4_Anand.txt"));
        model_list.emplace_back("Tamarisk", new  TAM::Model("../resources/TamariskMaps/2_IPPC.txt"));
        model_list.emplace_back("Traffic", new  TR::TrafficModel("../resources/TrafficModels/1_IPPC.txt"));
        model_list.emplace_back("Triangle Tireworld", new  TRT::Model("../resources/TriangleTireworlds/5_IPPC.txt",false,true));

        for (auto & [name, model] : model_list) {

            std::ostringstream stream1;
            stream1 << name << " & ";

            int horizon = model->getNumPlayers() == 1? 50 : 100;

            std::unordered_map<std::pair<FINITEH::Gamestate*,int> , double, VALUE_IT::QMapHash, VALUE_IT::QMapCompare> Q_map = {};
            //MISC::createQTable(model, horizon, &Q_map, "", false,0);
            //std::cout << "Qtable done!" << std::endl;
            //loadQTable(model, Q_map, "../nobackup/Q_analysis/" + abbreviation_map[name] + ".txt");

            std::vector<std::string> row = {name};
            for (double eps : {0.0,0.8,1.6}) {
                for (bool partial : {false,true}) {
                    auto oga = OGA::OgaAgent({
                       .budget={
                       1000,
                       "iterations"
                       },
                       .recency_count_limit=3,
                       .exploration_parameter=4.0,
                       .discount=1.0,
                       .num_rollouts = 1,
                       .rollout_length = -1,
                       .behavior_flags={
                       .exact_bookkeeping=true,
                       .group_terminal_states=true,
                       .group_partially_expanded_states=partial,
                       .partial_expansion_group_threshold=9999,
                       .eps_a = 0,
                       .eps_t = eps,
                       },
                        .in_abs_policy = "random",
                        .track_statistics = true,
                        .Q_map = &Q_map,
                        .distribution_agent = nullptr
                       });

                    const int num_maps = 100;
                    std::mt19937 rng1(static_cast<unsigned int>(42));

                    playGames(*model, num_maps, {&oga}, rng1, MUTED, {horizon,horizon});

                    stream1 << std::fixed << std::setprecision(2) << round(oga.getStatistics("intra_abs_required",-1) / (double) oga.getStatistics("ucb_calls",-1),2) << " & ";
                }
            }

            std::string result = stream1.str();
            std::ofstream outfile("../nobackup/intra_abs_stats.txt", std::ios::app); // Open in append mode
            if (outfile.is_open()) {
                outfile << result << std::endl;
                outfile.close();
            } else {
                std::cerr << "Failed to open the file." << std::endl;
            }
            std::cout << result <<  " \\\\" << std::endl;

        }

        //free models
        for(auto& [name, model] : model_list)
            delete model;
    }
}
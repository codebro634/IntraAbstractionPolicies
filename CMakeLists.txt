cmake_minimum_required(VERSION 3.16.3)
project(IntraAbs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter")

set(SOURCE_FILES
        src/main.cpp
        src/Arena.cpp
        src/Agents/Mcts/MctsAgent.cpp
        src/Agents/Mcts/MctsNode.cpp
        src/Agents/RandomAgent.cpp
        include/Games/Gamestate.h
        include/Agents/Agent.h
        include/Agents/Mcts/MctsAgent.h
        include/Agents/Mcts/MctsNode.h
        include/Agents/RandomAgent.h
        include/Arena.h
        include/Agents/HumanAgent.h
        src/Agents/HumanAgent.cpp
        include/Games/MDPs/GameOfLife.h
        src/Games/MDPs/GameOfLife.cpp
        src/Games/MDPs/Navigation.cpp
        src/Games/MDPs/Navigation.cpp
        include/Games/MDPs/SailingWind.h
        src/Games/MDPs/SailingWind.cpp
        include/Agents/Oga/OgaAgent.h
        src/Agents/Oga/OgaAgent.cpp
        include/Utils/UnitTest.h
        include/Games/MDPs/SysAdmin.h
        src/Games/MDPs/SysAdmin.cpp
        include/Games/MDPs/RaceTrack.h
        src/Games/MDPs/RaceTrack.cpp
        include/Games/MDPs/AcademicAdvising.h
        src/Games/MDPs/AcademicAdvising.cpp
        include/Utils/Argparse.h
        include/Games/MDPs/Tamarisk.h
        src/Games/MDPs/Tamarisk.cpp
        include/Games/MDPs/CrossingTraffic.h
        src/Games/MDPs/CrossingTraffic.cpp
        include/Utils/Distributions.h
        src/Utils/Distributions.cpp
        include/Utils/ValueIteration.h
        src/Utils/ValueIteration.cpp
        include/Agents/LookupTableAgent.h
        src/Agents/LookupTableAgent.cpp
        include/Games/MDPs/CooperativeRecon.h
        src/Games/MDPs/CooperativeRecon.cpp
        include/Games/MDPs/Traffic.h
        src/Games/MDPs/Traffic.cpp
        include/Games/MDPs/SkillsTeaching.h
        src/Games/MDPs/SkillsTeaching.cpp
        include/Games/MDPs/TriangleTireworld.h
        src/Games/MDPs/TriangleTireworld.cpp
        include/Games/MDPs/EarthObservation.h
        src/Games/MDPs/EarthObservation.cpp
        src/Games/MDPs/Manufacturer.cpp
        include/Games/MDPs/Manufacturer.h
        include/Games/Wrapper/FiniteHorizon.h
        src/Games/Wrapper/FiniteHorizon.cpp
        include/Agents/Oga/OgaAbstractNodes.h
        include/Agents/Oga/OgaGroundNodes.h
        src/Agents/Oga/OgaTree.cpp
        include/Agents/Oga/OgaUtils.h
        include/Agents/Oga/OgaTree.h
        src/Agents/Oga/OgaGroundNodes.cpp
        src/Agents/Oga/OgaAbstractNodes.cpp
        src/Agents/Oga/OgaUtils.cpp
        include/Utils/MiscAnalysis.h
        src/Utils/MiscAnalysis.cpp
        include/Utils/MemoryAnalysis.h
        src/Utils/MemoryAnalysis.cpp
        include/Agents/OneStepLookahead.h
        src/Agents/OneStepLookaheadAgent.cpp
        include/Utils/ModelMaker.h
        src/Utils/ModelMaker.cpp
        include/Utils/AgentMaker.h
        src/Utils/AgentMaker.cpp
        include/Games/Wrapper/RandomStart.h
        src/Games/Wrapper/RandomStart.cpp
)

#Debug executable
add_executable(IntraAbsDebug ${SOURCE_FILES})
target_compile_options(IntraAbsDebug PRIVATE
        ${CMAKE_CXX_FLAGS_DEBUG}   # typical debug flags (-O0 -g for gcc/clang)
)
target_link_options(IntraAbsDebug PRIVATE
        ${CMAKE_EXE_LINKER_FLAGS}         # global linker flags
        ${CMAKE_EXE_LINKER_FLAGS_DEBUG}   # typical debug linker flags
)

#Release executable
add_executable(IntraAbsRelease ${SOURCE_FILES})
target_compile_options(IntraAbsRelease PRIVATE
        -O3
        -DNDEBUG
)
target_link_options(IntraAbsRelease PRIVATE
        ${CMAKE_EXE_LINKER_FLAGS}
        ${CMAKE_EXE_LINKER_FLAGS_RELEASE}
)
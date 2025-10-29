# Investigating intra-abstraction policies for non-exact abstraction algorithms
## Purpose

This is the repository accompanying the paper "Investigating intra-abstraction policies for non-exact
abstraction algorithms" which contains the code to reproduce the experiments and the results of the paper.

## Cite this work

If you use this work, please cite it as:

```bibtex
@misc{schmöcker2025investigatingintraabstractionpoliciesnonexact,
      title={Investigating Intra-Abstraction Policies For Non-exact Abstraction Algorithms}, 
      author={Robin Schmöcker and Alexander Dockhorn and Bodo Rosenhahn},
      year={2025},
      eprint={2510.24297},
      archivePrefix={arXiv},
      primaryClass={cs.AI},
      url={https://arxiv.org/abs/2510.24297}, 
}
```

## Abstract

One weakness of Monte Carlo Tree Search (MCTS) is its sample efficiency which
can be addressed by building and using state and/or action abstractions in parallel
to the tree search such that information can be shared among nodes of the same
layer. The primary usage of abstractions for MCTS is to enhance the Upper Confidence
Bound (UCB) value during the tree policy by aggregating visits and returns
of an abstract node. However, this direct usage of abstractions does not take the
case into account where multiple actions with the same parent might be in the
same abstract node, as these would then all have the same UCB value, thus requiring
a tiebreak rule. In state-of-the-art abstraction algorithms such as pruned On
the Go Abstractions (pruned OGA), this case has not been noticed, and a random
tiebreak rule was implicitly chosen. In this paper, we propose and empirically
evaluate several alternative intra-abstraction policies, several of which outperform
the random policy across a majority of environments and parameter settings.

## Installation

To build the project from source, you will need a C++ compiler supporting the C++20 standard or higher (a lower standard probably works too but we have not tested that). The project
is self-contained and does not require any additional installation.

To compile with [CMake](https://cmake.org/) you need to have CMake installed on your system. A `CMakeLists.txt` file is already provided for configuring the build.

**Steps:**

1. **Clone the repository:**
    ```bash
    git clone https://github.com/codebro634/IntraAbstractionPolicies.git
    cd IntraAbstractionPolicies
    ```

2. **Create a build directory (optional but recommended):**
    ```bash
    mkdir build
    cd build
    ```

3. **Generate build files using CMake:**
    ```bash
    cmake -DCMAKE_CXX_COMPILER=/path/to/your/c++-compiler -DCMAKE_C_COMPILER=/path/to/your/c-compiler ..
    ```

4. **Compile the project:**
    ```bash
    cmake --build .
    ```
   *This will invoke the underlying build system (e.g., `make` or `ninja`) to compile the source code.*

If no errors occur, two compiled binaries `IntraAbsDebug` and `IntraAbsRelease` should now be available in the `build` directory. The former has been compiled with debug
compiler flags and the latter with aggressive optimization.

## Usage

The program is called with the following arguments:

`--seed`: The seed for the random number of generator. Running the program with the same seed will produce the same results.

`--n`: The number of episodes to run.

`--model`: The abbreviation for the model to use. Example values are
`sa` for SysAdmin, `gol` for Game of Life, `aa` for AcademicAdvising or  `tam` for Tamarisk. The ful list of abbreviations is found in main.cpp in the getModel method.

`--margs`: The arguments for the model. Mostly this is a game map to be specified which can be found in the
`resources` folder.

`--agent`:  Which agent to use. The only options are `mcts` and `oga`.

`--aargs`: The arguments for the agent. A list of required and optional arguments can be found in main.cpp in getAgent.

The following shows an example call of running standard OGA with 500 Mcts-iterations.

```bash
--seed 42 -n 10  --model gol --margs map=../resources/GameOfLifeMaps/1_Anand.txt  --agent oga --aargs iterations=500
```


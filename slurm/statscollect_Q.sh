#!/bin/bash
#SBATCH --job-name=benchmark
#SBATCH --output=./nobackup/slurm_outputs/slurm-%a.out
#SBATCH --error=./nobackup/slurm_outputs/slurm-%a.err
#SBATCH --time=11:59:00
#SBATCH --partition=cpu_short
#SBATCH --mem=2G
#SBATCH --cpus-per-task=1
#SBATCH --array=0-428
#SBATCH --verbose
#SBATCH --ntasks=1
#SBATCH --exclude=epyc1,epyc2,epyc3,epyc4,cc1l01

srun -c 1 -v ../cmake-build-debug/BenchmarkGamesRelease $((SLURM_ARRAY_TASK_ID)) 7200
#!/bin/bash
# Job name:
#SBATCH --job-name=janis_savio_test1
#
# Account:
#SBATCH --account=fc_hedm
#
# Partition:
#SBATCH --partition=savio2_htc
#
# Number of tasks needed for use case:
#SBATCH --ntasks=1
#
# Processors per task:
#SBATCH --cpus-per-task=1
#
# Wall clock limit:
#SBATCH --time=5:00:00
#
## Command(s) to run:
./Janis -m JanisTest3.mac

#!/bin/bash
# Job name:
#SBATCH --job-name=withfloor_neutron_all
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
#SBATCH --time=18:00:00
#
## Command(s) to run:
/global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/Janis -m /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/macros/withfloor_neutron.mac

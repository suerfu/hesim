#!/bin/bash
# Job name:
#SBATCH --job-name=janis_savio_analysis1
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
#SBATCH --time=15:00:00
#
## Command(s) to run:
root /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/analysis/neutron_analysis_1.C
root /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/analysis/neutron_analysis_2.C
root /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/analysis/neutron_analysis_3.C
root /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/analysis/neutron_analysis_4.C
root /global/home/users/yuanlq/McKinsey_Janis_Fridge/Janis-build/analysis/neutron_analysis_5.C

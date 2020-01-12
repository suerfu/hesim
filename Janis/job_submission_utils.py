import os, datetime

def Get_current_date():
    current_date = str(datetime.datetime.now())
    return current_date[:4] + current_date[5:7] + current_date[8:10]


def Get_iteration_directory(master_output_directory):
    id_number = 0

    test_directory = master_output_directory + '_' + str(id_number)

    while(os.path.isdir(test_directory) ):
        id_number += 1
        test_directory = master_output_directory + '_' + str(id_number)

    return test_directory

def Generate_mac(source):
    mac_file_base = '''# Macro for iron filter {0}
/run/initialize
/run/verbose 1
/tracking/verbose 0

/run/printProgress 10000

/generator/setMode {0}
'''

    mac_file_content = mac_file_base.format(source)

    return mac_file_content

def Generate_bash(output_directory, seed, base_macro_name, num_events, simulation_binary):
    bash_file_base = '''#!/bin/bash

date

cd {0}

cp {1} macros/{2}_${{SLURM_ARRAY_TASK_ID}}.mac

echo "/analysis/setFileName {3}_${{SLURM_ARRAY_TASK_ID}}" >> macros/{2}_${{SLURM_ARRAY_TASK_ID}}.mac
echo "/random/setSeeds $((${{SLURM_ARRAY_TASK_ID}}*{4})) $((${{SLURM_ARRAY_TASK_ID}}*13 + 1000))" >> macros/{2}_${{SLURM_ARRAY_TASK_ID}}.mac
echo "/run/beamOn {5}" >> macros/{2}_${{SLURM_ARRAY_TASK_ID}}.mac

cd root_files

{6} -m ../macros/{2}_${{SLURM_ARRAY_TASK_ID}}.mac

date
'''

    macro_name = base_macro_name[1:-4]
    base_name = macro_name[4:]

    bash_file_content = bash_file_base.format(output_directory, base_macro_name[1:], macro_name, base_name, seed, num_events, simulation_binary)

    return bash_file_content

def Generate_slrm(job_name, array_string, bash_file, out_file_directory):
    slrm_file_base = '''#!/bin/bash
# Job name:
#SBATCH --job-name={0}
#
# Account:
#SBATCH --account=fc_hedm
#
# Partition:
#SBATCH --partition=savio2_htc
#
# Indices:
#SBATCH --array={1}
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
{2} > {3}/{4}_${{SLURM_ARRAY_TASK_ID}}.out
'''

    base_name = bash_file.split('/')[-1][5:-3]

    slrm_file_content = slrm_file_base.format(job_name, array_string, bash_file, out_file_directory, base_name)

    return slrm_file_content

def Generate_analysis_slrm(job_name, root_path, analysis_script, out_file):
    slrm_file_base = '''#!/bin/bash
# Job name:
#SBATCH --job-name={0}
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
cd {1}
root -b {2} > {3}
'''

    slrm_file_content = slrm_file_base.format(job_name, root_path, analysis_script, out_file)

    return slrm_file_content


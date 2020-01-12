import os, sys, glob
import job_submission_utils as utils


# Set simulation specific info here
sim_name = 'HeDM'
sources = ['neutrons', 'gammas']
num_events_per_job = [1000000, 10000000]
seed = [14053, 45012]

duration='10:00:00'
array = '1-100'
sim_bin = '/global/home/users/abiekert/Iron_Filter/IronFilter/Iron_Filter-build/IronFilter'
analysis_script = '/global/home/users/abiekert/Iron_Filter/IronFilter/root_scripts/plot_source_spectrum.C'
base_name = 'iron_filter_'

# Setup the directories
master_output_path = '/global/scratch/abiekert/Iron_Filter/'
iteration_base_name = utils.Get_current_date()

output_path = utils.Get_iteration_directory(master_output_path + iteration_base_name)
os.system('mkdir -p ' + output_path)

macro_path = output_path + '/macros'
os.system('mkdir -p ' + macro_path)

root_path = output_path + '/root_files'
os.system('mkdir -p ' + root_path)

geant_outfile_path = output_path + '/geant_outfiles'
os.system('mkdir -p ' + geant_outfile_path)

slurm_outfile_path = output_path + '/slurm_outfiles'
os.system('mkdir -p ' + slurm_outfile_path)

results_path = output_path + '/results'
os.system('mkdir -p ' + results_path)

for i, source in enumerate(sources):

    # Generate base macro file
    mac_file_name = '/mac_' + base_name + source + '.mac'
    mac_file = open(output_path + mac_file_name, 'w')
    mac_file_content = utils.Generate_mac(source)
    mac_file.write( mac_file_content )
    mac_file.close()

    # Generate bash file
    bash_file_name = '/bash_' + base_name + source + '.sh'
    bash_file = open(output_path + bash_file_name, 'w')
    bash_file_content = utils.Generate_bash(output_path, seed[i], mac_file_name, num_events_per_job[i], sim_bin)
    bash_file.write( bash_file_content )
    bash_file.close()
    os.system('chmod u+x ' + output_path + bash_file_name)

    # Generate SLURM file
    slrm_file_name = '/slrm_' + base_name + source + '.slrm'
    slrm_file = open(output_path + slrm_file_name, 'w')
    slrm_file_content = utils.Generate_slrm(sim_name, array, output_path + bash_file_name, geant_outfile_path)
    slrm_file.write( slrm_file_content )
    slrm_file.close()
    os.system('chmod u+x ' + output_path + slrm_file_name)

# Generate analysis SLURM file
analysis_file_name = '/ana_' + base_name[:-1] + '.slrm'
analysis_file = open(output_path + analysis_file_name, 'w')
analysis_file_content = utils.Generate_analysis_slrm(sim_name, root_path, analysis_script, output_path.split('/')[-1] + '_results.txt')
analysis_file.write( analysis_file_content )
analysis_file.close()
os.system('chmod u+x ' + output_path + analysis_file_name)

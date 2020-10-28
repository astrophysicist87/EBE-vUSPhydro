#!/bin/bash
###############################################################################
##                                                                           ##
##                           Campus Cluster                                  ##
##                   Sample SERIAL Job Batch Script                          ##
##                                                                           ##
## SLURM Options (To view, run the following command below)                  ##
##                                                                           ##
##     man sbatch                                                            ##
##                                                                           ##
###############################################################################
#                                                                            
#SBATCH -t 06:00:00                  # Job run time (hh:mm:ss)
#SBATCH -N 1                        # Number of nodes
#SBATCH -J serial_job            # Name of batch job
#SBATCH -A qgp
#SBATCH -p qgp
#SBATCH -o serial.o%j              # Name of batch job output file
##SBATCH --error=serial.e%j              # Name of batch job error file
##SBATCH --mail-user=NetID@illinois.edu  # Send email notifications
##SBATCH --mail-type=BEGIN,END           # Type of email notifications to send
#                                                                            
###############################################################################
# Change to the directory from which the batch job was submitted
# Note: SLURM defaults to running jobs in the directory where
# they are submitted, no need for cd'ing to $SLURM_SUBMIT_DIR

#cd ${SLURM_SUBMIT_DIR}


# Run the serial code
./fo trentoinputPbPb2_211_0.dat PbPb2_211_0 0 0 0

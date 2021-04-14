#!/bin/bash

#SBATCH --partition=qgp             # Partition (job queue)
#SBATCH --job-name=EOS21_10            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=30:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/EOS21_10.out
#SBATCH --error=logs/EOS21_10.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jnorhos/v-USPhydro2


let i="$SLURM_ARRAY_TASK_ID"

srun /projects/jnorhos/v-USPhydro2/charm.sh dAu_EOS21_10 "$i" "$i" shear EOS21/10 trento dAu200GeV > runs/dAu_EOS21_10_"$i".dat



sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

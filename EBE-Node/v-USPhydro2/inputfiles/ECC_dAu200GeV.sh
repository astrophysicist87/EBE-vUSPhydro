#!/bin/bash


#SBATCH --partition=qgp             # Partition (job queue)
#SBATCH --array=0-10
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=3:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/dAu200GeV.out
#SBATCH --error=logs/dAu200GeV.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jnorhos/v-USPhydro2/inputfiles

let i="$SLURM_ARRAY_TASK_ID"

srun /projects/jnorhos/v-USPhydro2/inputfiles/npart trento/dAu200GeV/"$i" 0 999  > logs/dAu200GeV"$i".dat
srun /projects/jnorhos/v-USPhydro2/inputfiles/eccCM trento/dAu200GeV/"$i" 0 999 0.02 cgc >> logs/dAu200GeV"$i".dat



sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

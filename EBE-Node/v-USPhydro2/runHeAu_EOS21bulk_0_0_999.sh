#!/bin/bash


#SBATCH --partition=qgp             # Partition (job queue)
#SBATCH --job-name=HeAu_EOS21bulk_0_0            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=30:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/EOS21bulk_0.out
#SBATCH --error=logs/EOS21bulk_0.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jnorhos/v-USPhydro2


let i="$SLURM_ARRAY_TASK_ID"
 
srun /projects/jnorhos/v-USPhydro2/charm.sh HeAu_EOS21bulk_0 "$i" "$i" shear EOS21bulk/0 trento HeAu200GeV > runs/FILENAME_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

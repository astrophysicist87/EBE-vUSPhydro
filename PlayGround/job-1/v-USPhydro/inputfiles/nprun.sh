#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --array=0-35
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=2:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/arrayJob_%A_%a.out
#SBATCH --error=logs/arrayJob_%A_%a.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/inputfiles

let i="$SLURM_ARRAY_TASK_ID"

srun /home/projects/jn511_1/v-USPhydro/inputfiles/npart trento/XeXe5440TeVdef/"$i" 0 999  > logs/npout"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

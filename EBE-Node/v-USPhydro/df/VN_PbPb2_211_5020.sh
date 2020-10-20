#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --array=0-44
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=0:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/211_44.out
#SBATCH --error=logs/211_44.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df

let i="$SLURM_ARRAY_TASK_ID"

srun /home/projects/jn511_1/v-USPhydro/df/v1/fo trentoinputPbPb2_211_"$i".dat 0 999 chgv1.dat 0.2 3 1 trentoPbPb25020mul 1 > logs/PbPb2_211_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

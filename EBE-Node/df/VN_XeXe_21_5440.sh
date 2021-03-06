#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --array=0-34
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=0:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/21_34.out
#SBATCH --error=logs/21_34.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df

let i="$SLURM_ARRAY_TASK_ID"

srun /home/projects/jn511_1/v-USPhydro/df/v1/fo trentoinputXeXe_21_"$i".dat 0 999 omega.dat 0.2 3 1 trentoXeXe5440mul 1 > logs/XeXe_21_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

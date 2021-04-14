#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=ALL_BINN            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=36:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/ALL_BINN.out
#SBATCH --error=logs/ALL_BINN.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro2


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/TYP_BINN > runs/ALL_BINN_"$i".dat
srun /home/projects/jn511_1/v-USPhydro2/across.sh "$i" TYP  >> runs/ALL_BINN_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

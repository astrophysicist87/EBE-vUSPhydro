#!/bin/bash


#SBATCH --partition=jn511_1,main             # Partition (job queue)
#SBATCH --array=0-4
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=0:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/EOS21_4.out
#SBATCH --error=logs/EOS21_4.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jn511_1/v-USPhydro2/df

let i="$SLURM_ARRAY_TASK_ID"

srun /projects/jn511_1/v-USPhydro2/df/vns/fo trentoinputPbPb_EOS21_"$i".dat 0 999 chgv1.dat 0.2 3 1 trentoPbPb5020mul 0 > logs/PbPb_EOS21_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

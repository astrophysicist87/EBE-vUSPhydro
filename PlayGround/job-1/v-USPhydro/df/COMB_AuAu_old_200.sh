#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=0:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/old.out
#SBATCH --error=logs/old.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df

srun /home/projects/jn511_1/v-USPhydro/df/v1/ptorg out/trento/AuAu200GeV/shear/old_s3 0 29 omega trento200mul  > logs/AuAu_old.dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

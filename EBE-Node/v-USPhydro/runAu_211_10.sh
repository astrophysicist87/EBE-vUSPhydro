#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=211_10            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=18:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/AuAu_211_10.out
#SBATCH --error=logs/AuAu_211_10.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/211_10 > runs/AuAu_211_10_"$i".dat
srun /home/projects/jn511_1/v-USPhydro/charm.sh AuAu_211_10 "$i" "$i" shear EOS211_150/10 trento AuAu200GeV >> runs/AuAu_211_10_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

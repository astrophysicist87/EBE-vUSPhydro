#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=211_45            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=12:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/AuAu_211_45.out
#SBATCH --error=logs/AuAu_211_45.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/211_45 > runs/AuAu_211_45_"$i".dat
srun /home/projects/jn511_1/v-USPhydro/df/ebedf.sh AuAu_211_45 "$i" "$i" shear EOS211_150/45 trento AuAu200GeV >> runs/AuAu_211_45_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2
#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=TYP_BINN            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=14:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/AuAu_TYP_BINN.out
#SBATCH --error=logs/AuAu_TYP_BINN.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/TYP_BINN > runs/AuAu_TYP_BINN_"$i".dat
srun /home/projects/jn511_1/v-USPhydro/charm.sh AuAu_TYP_BINN "$i" "$i" shear EOSTYPE/BINN trento AuAu200GeV >> runs/AuAu_TYP_BINN_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

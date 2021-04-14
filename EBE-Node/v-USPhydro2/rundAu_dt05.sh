#!/bin/bash


#SBATCH --partition=jn511_1,main             # Partition (job queue)
#SBATCH --job-name=dAu_dt05_BINN_BINN            # Assign an short name to your job
#SBATCH --array=EVENTS
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=30:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/dt05_BINN.out
#SBATCH --error=logs/dt05_BINN.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jn511_1/v-USPhydro2


let i="$SLURM_ARRAY_TASK_ID"
 
srun /projects/jn511_1/v-USPhydro2/charm.sh dAu_dt05_BINN "$i" "$i" shear dt05/BINN trento dAu200GeV > runs/dAu_dt05_BINN_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

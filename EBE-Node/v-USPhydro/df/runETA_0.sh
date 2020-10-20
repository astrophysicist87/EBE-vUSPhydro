#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=0            # Assign an short name to your job
#SBATCH --array=0-499
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=10:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/eta_0.out
#SBATCH --error=logs/eta_0.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df


let j="$SLURM_ARRAY_TASK_ID"

echo logs/XeXe_0 > runs/XeXe_0_"$j".dat
srun /home/projects/jn511_1/v-USPhydro/df/charm.sh XeXeeta_21_e0 "$j" "$j" shear  EOS21/e0  trento XeXe5440TeVdef >> runs/XeXe_0_"$j".dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

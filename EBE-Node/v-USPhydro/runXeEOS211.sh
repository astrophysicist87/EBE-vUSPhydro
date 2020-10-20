#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=22:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/arrayJob_%A_%a.out
#SBATCH --error=logs/arrayJob_%A_%a.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd $HOME/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"+4000
srun $HOME/v-USPhydro/charm.sh XeXe_211 "$i" "$i" shear EOS211 trento XeXe5440TeV > runs/XeXe211_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

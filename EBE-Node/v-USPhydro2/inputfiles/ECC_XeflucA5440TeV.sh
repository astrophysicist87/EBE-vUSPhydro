#!/bin/bash


#SBATCH --partition=jn511_1,main             # Partition (job queue)
#SBATCH --array=0-1
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=3:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/XeflucA5440TeV.out
#SBATCH --error=logs/XeflucA5440TeV.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /projects/jn511_1/v-USPhydro2/inputfiles

let i="$SLURM_ARRAY_TASK_ID"

srun /projects/jn511_1/v-USPhydro2/inputfiles/npart trento/XeflucA5440TeV/"$i" 0 999  > logs/XeflucA5440TeV"$i".dat
srun /projects/jn511_1/v-USPhydro2/inputfiles/eccCM trento/XeflucA5440TeV/"$i" 0 999 0.05 cgc >> logs/XeflucA5440TeV"$i".dat



sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

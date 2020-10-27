#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=21_6            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=18:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/PbPb2_21_6.out
#SBATCH --error=logs/PbPb2_21_6.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/21_6 > runs/PbPb2_21_6_"$i".dat
srun /home/projects/jn511_1/v-USPhydro/charm.sh PbPb2_21_6 "$i" "$i" shear EOS21/6 trento PbPb5020TeV >> runs/PbPb2_21_6_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

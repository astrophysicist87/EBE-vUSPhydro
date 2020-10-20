#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=0.12            # Assign an short name to your job
#SBATCH --array=0-4
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=8:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/Pb2eta_0.12.out
#SBATCH --error=logs/Pb2eta_0.12.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"


srun /home/projects/jn511_1/v-USPhydro/ncharm.sh PbPb2eta_21_e0.12 "$i" "$i" shear  EOS21/e0.12  trento PbPb5020TeV > runs/PbPb2_0.12_"$i".dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

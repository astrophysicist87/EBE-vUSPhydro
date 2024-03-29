#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=21_0            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=12:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/Pb2_21_0.out
#SBATCH --error=logs/Pb2_21_0.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro2/df


let i="$SLURM_ARRAY_TASK_ID"
 echo logs/21_0 > runs/PbPb2_21_0_"$i".dat
srun /home/projects/jn511_1/v-USPhydro2/df/ebedf.sh PbPb2_21_0 "$i" "$i" shear EOS21/0 trento PbPb5020TeV >> runs/PbPb2_21_0_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

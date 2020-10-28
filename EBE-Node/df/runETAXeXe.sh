#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=XeXeeta            # Assign an short name to your job
#SBATCH --array=0-499
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=300                   # Real memory (RAM) required (MB)
#SBATCH --time=00:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/XeXeeta.out
#SBATCH --error=logs/XeXeeta.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df


let j="$SLURM_ARRAY_TASK_ID"

if [ $j -le 499 ]
then
    srun /home/projects/jn511_1/v-USPhydro/df/fo trentoinputXeXeeta_21_e0.04.dat  XeXe_21_e0.04  "$j"  neg 0 0  > runs/XeXe_0.04_"$j".dat
elif [ $j -gt 499 ] && [ $j -le 999 ]
then
    let i="$j"-500
    srun /home/projects/jn511_1/v-USPhydro/df/fo trentoinputPbPb2eta_21_e0.08.dat PbPb2_21_e0.08 "$i"  neg  0 0  > runs/PbPb2_0.08_"$i".dat
elif [ $j -gt 999 ] && [ $j -le 1499 ]
then
    let i="$j"-1000
    srun /home/projects/jn511_1/v-USPhydro/df/fo trentoinputPbPb2eta_21_e0.12.dat PbPb2_21_e0.12 "$i" neg 0 0  > runs/PbPb2_0.12_"$i".dat
elif [ $j -gt 1499 ] && [ $j -le 1999 ]
then
    let i="$j"-1500
    srun /home/projects/jn511_1/v-USPhydro/df/fo trentoinputPbPb2eta_21_e0.16.dat PbPb2_21_e0.16 "$i" neg 0 0  > runs/PbPb2_0.16_"$i".dat
else
    let i="$j"-2000
    srun /home/projects/jn511_1/v-USPhydro/df/fo trentoinputPbPb2eta_21_e0.2.dat PbPb2_21_e0.2 "$i" neg 0 0  > runs/PbPb2_0.2_"$i".dat
fi





sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

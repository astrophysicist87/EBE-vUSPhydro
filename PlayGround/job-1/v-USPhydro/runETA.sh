#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --job-name=EIN            # Assign an short name to your job
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=600                   # Real memory (RAM) required (MB)
#SBATCH --time=18:00:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/eta_EIN.out
#SBATCH --error=logs/eta_EIN.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro


let i="$SLURM_ARRAY_TASK_ID"

if [ $i -le 499 ]
then
 echo logs/PbPb2_EIN > runs/PbPb2_EIN_"$i".dat
srun /home/projects/jn511_1/v-USPhydro/charm.sh PbPb2eta_21_eEIN "$i" "$i" shear  EOS21/eEIN  trento PbPb5020TeV >> runs/PbPb2_EIN_"$i".dat
else
let j="$i"-500
echo logs/XeXe_EIN > runs/XeXe_EIN_"$j".dat
srun /home/projects/jn511_1/v-USPhydro/charm.sh XeXeeta_21_eEIN "$j" "$j" shear  EOS21/eEIN  trento XeXe5440TeVdef >> runs/XeXe_EIN_"$j".dat
fi

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

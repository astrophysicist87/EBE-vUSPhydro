#!/bin/bash


#SBATCH --partition=main             # Partition (job queue)
#SBATCH --array=0-999
#SBATCH --requeue                    # Return job to the queue if preempted
#SBATCH --nodes=1                    # Number of nodes you require
#SBATCH --ntasks=1                   # Total # of tasks across all nodes
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)
#SBATCH --mem=500                   # Real memory (RAM) required (MB)
#SBATCH --time=0:30:00              # Total run time limit (HH:MM:SS)
#SBATCH --output=logs/r16_0.out
#SBATCH --error=logs/r16_0.err
#SBATCH --export=ALL                 # Export you current env to the job env


# set data and working directories
cd /home/projects/jn511_1/v-USPhydro/df

let i="$SLURM_ARRAY_TASK_ID"

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/2/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/2/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/3/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/3/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/4/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/4/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/5/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/5/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/6/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/6/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

srun /home/projects/jn511_1/v-USPhydro/df/decays/reso  out/trento/AuAu200GeV/shear/EOS21_150/7/ev"$i"sbvc_dNdphidpp.dat out/trento/AuAu200GeV/shear/EOS21_16/7/ev"$i"dsbvc_dNdphidpp.dat reso16.inp   > logs/r16_"$i".dat

sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

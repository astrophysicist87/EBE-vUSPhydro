#!/bin/bash

#SBATCH --partition=main             # Partition (job queue)                                                                                                            
#SBATCH --requeue                    # Return job to the queue if preempted                                                                                             
#SBATCH --nodes=1                    # Number of nodes you require                                                                                                      
#SBATCH --ntasks=1                   # Total # of tasks across all nodes                                                                                                
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)                                                                                         
#SBATCH --mem=500                   # Real memory (RAM) required (MB)                                                                                                   
#SBATCH --time=0:50:00              # Total run time limit (HH:MM:SS)                                                                                                   
#SBATCH --output=logs/old.out                                                                                                                                           
#SBATCH --error=logs/old.err                                                                                                                                            
#SBATCH --export=ALL                 # Export you current env to the job env                                                                                            


# set data and working directories                                                                                                                                      
cd /home/projects/jn511_1/



srun  /home/projects/jn511_1/combine.sh v-USPhydro/df/out/trento/PbPb5020TeV/shear/old_t0 int_chgv1_pt0.2-3.dat 0 29 


cd /home/projects/jn511_1/v-USPhydro/df

srun /home/projects/jn511_1/v-USPhydro/df/out/cavg trentoinputPbPb2_old_0.dat 29 chgv1.dat trentoPbPb25020mul int_chgv1_pt0.2-3  >> logs/PbPb2_old.dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

#!/bin/bash

#SBATCH --partition=jn511_1,main             # Partition (job queue)                                                                                                            
#SBATCH --requeue                    # Return job to the queue if preempted                                                                                             
#SBATCH --nodes=1                    # Number of nodes you require                                                                                                      
#SBATCH --ntasks=1                   # Total # of tasks across all nodes                                                                                                
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)                                                                                         
#SBATCH --mem=500                   # Real memory (RAM) required (MB)                                                                                                   
#SBATCH --time=0:50:00              # Total run time limit (HH:MM:SS)                                                                                                   
#SBATCH --output=logs/EOS21_165.out                                                                                                                                           
#SBATCH --error=logs/EOS21_165.err                                                                                                                                            
#SBATCH --export=ALL                 # Export you current env to the job env                                                                                            


# set data and working directories                                                                                                                                      
cd /projects/jn511_1/



srun  /projects/jn511_1/combine.sh v-USPhydro2/df/out/trento/OO_3pF_65TeV/shear/EOS21_165 int_K+_pt0.2-3.dat 0 2 


cd /projects/jn511_1/v-USPhydro2/df

srun /projects/jn511_1/v-USPhydro2/df/out/cavg trentoinputOO_EOS21_165_0.dat 2 K+.dat trentoOO65mul int_chgv1_pt0.2-3  >> logs/OO_EOS21_165.dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

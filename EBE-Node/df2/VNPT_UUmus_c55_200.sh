#!/bin/bash

#SBATCH --partition=jn511_1,main             # Partition (job queue)                                                                                                            
#SBATCH --requeue                    # Return job to the queue if preempted                                                                                             
#SBATCH --nodes=1                    # Number of nodes you require                                                                                                      
#SBATCH --ntasks=1                   # Total # of tasks across all nodes                                                                                                
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)                                                                                         
#SBATCH --mem=500                   # Real memory (RAM) required (MB)                                                                                                   
#SBATCH --time=0:50:00              # Total run time limit (HH:MM:SS)                                                                                                   
#SBATCH --output=logs/c55.out                                                                                                                                           
#SBATCH --error=logs/c55.err                                                                                                                                            
#SBATCH --export=ALL                 # Export you current env to the job env                                                                                            


# set data and working directories                                                                                                                                      
cd /projects/jn511_1/



srun  /projects/jn511_1/combine.sh v-USPhydro2/df/out/trento/UUmus200GeV/shear/c55 int_chgv1_pt0.2-3.dat 0 1 


cd /projects/jn511_1/v-USPhydro2/df

srun /projects/jn511_1/v-USPhydro2/df/out/cavg trentoinputUUmus_c55_0.dat 1 chgv1.dat trentoUUmus200mul int_chgv1_pt0.2-3  >> logs/UUmus_c55.dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

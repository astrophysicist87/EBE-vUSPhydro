#!/bin/bash

#SBATCH --partition=jn511_1,main             # Partition (job queue)                                                                                                            
#SBATCH --requeue                    # Return job to the queue if preempted                                                                                             
#SBATCH --nodes=1                    # Number of nodes you require                                                                                                      
#SBATCH --ntasks=1                   # Total # of tasks across all nodes                                                                                                
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)                                                                                         
#SBATCH --mem=500                   # Real memory (RAM) required (MB)                                                                                                   
#SBATCH --time=0:50:00              # Total run time limit (HH:MM:SS)                                                                                                   
#SBATCH --output=logs/b2.out                                                                                                                                           
#SBATCH --error=logs/b2.err                                                                                                                                            
#SBATCH --export=ALL                 # Export you current env to the job env                                                                                            


# set data and working directories                                                                                                                                      
cd /projects/jn511_1/



srun  /projects/jn511_1/combine.sh v-USPhydro2/df/out/trento/XeXe5440TeVdef_b2/shear/EOS21 int_chgv1_pt0.2-3.dat 0 10 


cd /projects/jn511_1/v-USPhydro2/df

srun /projects/jn511_1/v-USPhydro2/df/out/cavg trentoinputXeXedef_b2_0.dat 10 chgv1.dat trentoXeXedef5440b2mul int_chgv1_pt0.2-3  >> logs/XeXedef_b2.dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

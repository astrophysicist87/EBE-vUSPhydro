#!/bin/bash

#SBATCH --partition=main             # Partition (job queue)                                                                                                            
#SBATCH --requeue                    # Return job to the queue if preempted                                                                                             
#SBATCH --nodes=1                    # Number of nodes you require                                                                                                      
#SBATCH --ntasks=1                   # Total # of tasks across all nodes                                                                                                
#SBATCH --cpus-per-task=1            # Cores per task (>1 if multithread tasks)                                                                                         
#SBATCH --mem=500                   # Real memory (RAM) required (MB)                                                                                                   
#SBATCH --time=0:50:00              # Total run time limit (HH:MM:SS)                                                                                                   
#SBATCH --output=logs/211.out                                                                                                                                           
#SBATCH --error=logs/211.err                                                                                                                                            
#SBATCH --export=ALL                 # Export you current env to the job env                                                                                            


# set data and working directories                                                                                                                                      
cd /home/projects/jn511_1/



srun  /home/projects/jn511_1/combine.sh v-USPhydro/df/out/trento/XeXe5440TeVdef/shear/EOS211 int_lambda_pt0.2-3.dat 0 3 


cd /home/projects/jn511_1/v-USPhydro/df

srun /home/projects/jn511_1/v-USPhydro/df/out/cavg trentoinputXeXedef_211_0.dat 3 lambda.dat trentoXeXedef5440mul int_chgv1_pt0.2-3  >> logs/XeXedef_211.dat


sleep 3
sacct --format NTasks,MaxRSS,Elapsed,AveRSS,AveCPU -j $SLURM_JOBID
sleep 2

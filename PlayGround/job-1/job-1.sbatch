#!/bin/bash
#SBATCH -N 1
#SBATCH -J EBE_vUSPhydro-1
#SBATCH -t 2:00:00
#SBATCH -A qgp
#SBATCH -p qgp
#SBATCH -o EBE_vUSPhydro.o%j
#SBATCH -e EBE_vUSPhydro.e%j
cd /projects/jnorhos/plumberg/EBE-vUSPhydro/PlayGround/job-1
(cd crank
    ulimit -n 1000
    python ./SequentialEventDriver_shell.py 1 1> RunRecord.txt 2> ErrorRecord.txt
    cp RunRecord.txt ErrorRecord.txt ../finalResults/
)
mv ./finalResults /projects/jnorhos/plumberg/EBE-vUSPhydro/RESULTS/job-1

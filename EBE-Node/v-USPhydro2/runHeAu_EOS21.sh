#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/
#PBS -l walltime=30:00:00
#PBS -N HeAu_EOS21_BINN
#PBS -o logs/HeAu_EOS21_BINN_LNUM.dat
#PBS -l nodes=1:ppn=1
#PBS -l naccesspolicy=shared
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2

#rm $PBS_O_WORKDIR/logs/*
#rm $PBS_O_WORKDIR/runs/*

./charm.sh HeAu_EOS21_BINN LNUM LNUM shear EOS21/BINN trento HeAu200GeV > runs/HeAu_EOS21_BINN_LNUM.dat  


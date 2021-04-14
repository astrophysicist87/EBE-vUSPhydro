#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/
#PBS -l walltime=30:00:00
#PBS -N RuRuA_EOS21_BINN
#PBS -o logs/RuRuA_EOS21_BINN_LNUM.dat
#PBS -l nodes=1:ppn=1
#PBS -l naccesspolicy=shared
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2

#rm $PBS_O_WORKDIR/logs/*
#rm $PBS_O_WORKDIR/runs/*

./charm.sh RuRuA_EOS21_BINN LNUM LNUM shear EOS21/BINN trento RuRuA200GeV > runs/RuRuA_EOS21_BINN_LNUM.dat  


#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/
#PBS -l walltime=30:00:00
#PBS -N vUSPtests
#PBS -l nodes=1:ppn=1
#PBS -l naccesspolicy=shared
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2/

module load gcc

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

./charm.sh RuRuA_EOS21_0 "$1" "$1" shear EOS21/0 trento RuRuA200GeV > runs/RuA_"$1".dat  &


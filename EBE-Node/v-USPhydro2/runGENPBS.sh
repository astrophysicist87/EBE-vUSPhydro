#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/
#PBS -l walltime=30:00:00
#PBS -N IDNUM
#PBS -o logs/FILENAME_LNUM.dat
#PBS -l nodes=1:ppn=1
#PBS -l naccesspolicy=shared
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2

#rm $PBS_O_WORKDIR/logs/*
#rm $PBS_O_WORKDIR/runs/*

./charm.sh IDNUM LNUM LNUM shear FOLDER/BINN trento DIRECTORY > runs/FILENAME_LNUM.dat  


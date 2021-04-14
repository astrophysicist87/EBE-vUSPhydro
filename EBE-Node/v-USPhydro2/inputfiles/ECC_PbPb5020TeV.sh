#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/inputfiles/
#PBS -l walltime=30:00:00
#PBS -N ECCS
#PBS -l nodes=1:ppn=1
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2/inputfiles

module load gcc

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

ls

for (( i=0; i<=65; i++))
    do
	./npart trento/PbPb5020TeV/"$i" 0 999  > logs/PbPb5020TeV"$i".dat
	./eccCM trento/PbPb5020TeV/"$i" 0 999 0.06 cgc >> logs/PbPb5020TeV"$i".dat
    done
  wait

#!/bin/bash 

#PBS -d /projects/jnorhos/v-USPhydro2/inputfiles/
#PBS -l walltime=30:00:00
#PBS -N ECCS
#PBS -l nodes=1:ppn=1
#PBS -q qgp
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2/inputfiles


#rm $PBS_O_WORKDIR/logs/*
#rm $PBS_O_WORKDIR/runs/*

ls

for (( i=0; i<=10; i++))
    do
	./npart trento/dAusix200GeV/"$i" 0 999  > logs/dAusix200GeV"$i".dat
	./eccCM trento/dAusix200GeV/"$i" 0 999 0.02 cgc >> logs/dAusix200GeV"$i".dat
    done
  wait

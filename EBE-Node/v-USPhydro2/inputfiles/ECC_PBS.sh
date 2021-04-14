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

for (( i=EVENTS0; i<=EVENTSF; i++))
    do
	./npart trento/DIRECTORY/"$i" 0 999  > logs/FILENAME"$i".dat
	./eccCM trento/DIRECTORY/"$i" 0 999 GRID cgc >> logs/FILENAME"$i".dat
    done
  wait

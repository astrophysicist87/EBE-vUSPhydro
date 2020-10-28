#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro/df
#PBS -l walltime=00:30:00
#PBS -N RUNID
#PBS -l nodes=1:ppn=1
#PBS -q secondary
#PBS -o logs/output.o
#PBS -e logs/error.e

cd /projects/jnorhos/v-USPhydro/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=START; i<=END; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputRUNID_"$i".dat 0 999 CHG.dat PTMIN 3 1 SRTFL PTVN > runs/RUNID_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/COMBNAME

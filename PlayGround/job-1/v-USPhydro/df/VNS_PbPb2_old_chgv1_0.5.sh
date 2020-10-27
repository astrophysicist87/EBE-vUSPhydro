#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro/df
#PBS -l walltime=00:30:00
#PBS -N PbPb2_old
#PBS -l nodes=1:ppn=1
#PBS -q secondary
#PBS -j oe
cd /projects/jnorhos/v-USPhydro/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=50; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputPbPb2_old_"$i".dat 0 999 chgv1.dat 0.5 3 1 trento5020mul 0 > runs/PbPb2_old_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/VNPT_PbPb2_old_chgv1_0.5.sh

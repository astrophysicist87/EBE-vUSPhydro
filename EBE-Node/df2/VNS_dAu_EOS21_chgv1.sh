#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro2/df
#PBS -l walltime=00:30:00
#PBS -N dAu
#PBS -l nodes=1:ppn=1
#PBS -q qgp
#PBS -j oe
cd /projects/jnorhos/v-USPhydro2/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=0; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputdAu_"$i".dat 0 999 EOS21.dat chgv1 3 1 200 0 > runs/dAu_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/VNPT_dAu_EOS21_chgv1.sh

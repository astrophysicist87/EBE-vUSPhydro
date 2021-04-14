#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro2/df
#PBS -l walltime=00:30:00
#PBS -N UUA_EOS21
#PBS -l nodes=1:ppn=1
#PBS -q secondary
#PBS -j oe
cd /projects/jnorhos/v-USPhydro2/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=1; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputUUA_EOS21_"$i".dat 0 999 chgv1.dat 0.2 3 1 trentoUUA200mul 0 > runs/UUA_EOS21_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/VNPT_UUA_EOS21_chgv1_0.2.sh

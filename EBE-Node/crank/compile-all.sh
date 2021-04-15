#! /usr/bin/env bash
(cd ..
for ii in "jaki_trento" "trento" "df" "v-USPhydro" "df2" "v-USPhydro2"
    do
    (cd $ii; make clean; make)
done

echo "Compiling finished."
echo "Next generate jobs using generate-jobs-XXX.sh."
)

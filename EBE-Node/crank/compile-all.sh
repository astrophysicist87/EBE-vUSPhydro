#! /usr/bin/env bash
(cd ..
for ii in "jaki_trento" "df" "v-USPhydro"
    do
    (cd $ii; make clean; make)
done

echo "Compiling finished."
echo "Next generate jobs using generate-jobs-XXX.sh."
)

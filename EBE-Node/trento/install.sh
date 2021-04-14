#! /bin/bash
#-----------

rm -rf build \
    && mkdir build \
    && cd build \
    && cmake \
        -DCMAKE_INSTALL_PREFIX=/projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/trento \
        -DCMAKE_PREFIX_PATH=/projects/jnorhos/plumberg/OSU_hydro .. \
    && make DESTDIR= install \
    && cd ..

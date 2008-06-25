#! /bin/sh

#
# Find the Eldora top directory (one directory up from where this script lives), 
# so that we can find the config files
#
topdir="${0%/*}/.."
# If we weren't run using an absolute path, the prepend the working directory
# to get an absolute path
if [[ ${topdir:0:1} != '/' ]]; then topdir="$PWD/$topdir"; fi

# location of DDS configurtion files
CONF=$ELDORADIR/conf

# The producer invocation
producer="$topdir/testdds/producer \
        --ORB $CONF/ORBSvc.conf \
        --DCPS $CONF/DDSClient.ini \
        --InfoRepo iiop://localhost:50000/DCPSInfoRepo \
        --delta 10000
        --raytopic EldoraRays  --tstopic EldoraTS"

echo "Starting producer"
$producer 






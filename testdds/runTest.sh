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
CONF=$topdir/conf

# where to save the IOR
REPOFILE=/tmp/eldoraRepo.ior

# The DCPSInforRepo invocation
dcpsinforepo="$DDS_ROOT/bin/DCPSInfoRepo  \
   -ORBSvcConf $CONF/tcp.conf \
   -ORBListenEndpoints iiop://192.168.2.2:50000 \
   -d $CONF/domain_ids"

# The producer invocation
producer="$topdir/testdds/producer \
        --ORB $CONF/tcp.conf \
        --DCPS $CONF/simpleConf.ini \
        --delta 10000 \
        --pulse EldoraPulses --ts EldoraTS"

# The consumer invocation
consume="$topdir/testdds/consumer --ORB $CONF/tcp.conf \
         --DCPS $CONF/simpleConf.ini \
         --pulse EldoraPulses --ts EldoraTS"

# kill existing jobs
pkill producer
pkill consumer
pkill DCPSInfoRepo

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill producer; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$dcpsinforepo &
sleep 2

echo "Starting producer"
$producer &

echo "Starting consumer 1"
$consume &

echo "Starting consumer 2"
$consume &

echo "Starting consumer 3"
$consume







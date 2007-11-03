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

# The consumer invocation
consume="$topdir/testdds/consumer --ORB $CONF/tcp.conf \
         --DCPS $CONF/simpleConf.ini \
         --pulse EldoraPulses --ts EldoraTS"

# kill existing jobs
pkill consumer


# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill producer; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting consumer 1"
$consume 






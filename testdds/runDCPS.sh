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

# The DCPSInforRepo invocation
dcpsinforepo="$DDS_ROOT/bin/DCPSInfoRepo  \
   -ORBSvcConf $CONF/tcp.conf \
   -ORBListenEndpoints iiop://192.168.2.2:50000 \
   -d $CONF/domain_ids"

# kill existing jobs
pkill producer
pkill consumer
pkill DCPSInfoRepo

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill producer; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$dcpsinforepo

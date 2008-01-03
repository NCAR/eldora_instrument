#! /bin/sh
#
# Usage runTest.sh [<testrr314_arg> ...]
#

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
   -NOBITS \
   -DCPSConfigFile $CONF/DCPSconf.ini \
   -ORBSvcConf $CONF/tcp.conf \
   -ORBListenEndpoints iiop://dcpsrepo:50000 \
   -d $CONF/domain_ids"

# The consumer invocation
consume="$topdir/testdds/consumer --ORB $CONF/tcp.conf \
         --DCPS $CONF/consumer.ini \
         --pulse EldoraPulses --ts EldoraTS"

produce="$topdir/redrapids/testrr314 \
   --ORB $CONF/tcp.conf \
   --DCPS $CONF/producer.ini \
   --pub $*"
 
# kill existing jobs
pkill consumer
pkill DCPSInfoRepo

# Remove existing IOR
rm -f $REPOFILE

# trap the signals on this script, and kill the jobs
trap 'echo -e "$0: SHUTTING DOWN"; trap "" ERR; \
	pkill consumer; pkill publisher; pkill DCPSInfoRepo' ERR SIGINT SIGTERM
	
echo "Starting DCPSInfoRepo"
$dcpsinforepo &

sleep 2
echo "Starting consumer 1"
$consume &

sleep 2
echo "Starting consumer 2"
$consume &

sleep 2
echo "starting rr314 producer"
$produce


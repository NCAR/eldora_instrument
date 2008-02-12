#! /bin/sh
#
# Usage runRR.sh [<rr314dual args> ...]
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
   -DCPSConfigFile $CONF/DCPSInfoRepo.ini \
   -ORBSvcConf $CONF/ORBSvc.conf \
   -ORBListenEndpoints iiop://dcpsrepo:50000 \
   -d $CONF/DDSDomainIds.conf"

# the RR314 application
rr314="$topdir/eldoradrx/rr314dual \
   --ORB $CONF/ORBSvc.conf \
   --DCPS $CONF/DDSClient.ini \
   --pub $*"
 
# trap the signals on this script, and kill the jobs
trap 'echo -e "$0: SHUTTING DOWN"; trap "" ERR; \
	pkill rr314dual' ERR SIGINT SIGTERM

# ignore hup so that we can log out and leave running
trap "" HUP
	
# start the DCPS info repo if it is not already running"
dcpspid=`pgrep DCPSInfoRepo`
if [ "$dcpspid" == "" ] ; then
   echo "Starting the DCPS info repository"
   $dcpsinforepo & 
fi

sleep 1

# kill existing rr314dual
pkill rr314dual

echo "Starting rr314. Hit control-c to terminate. That"
echo "will terminate all rr314 programs and the DCPSInfoRepo"
$rr314





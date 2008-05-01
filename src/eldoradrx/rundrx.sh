#!/bin/sh
#
# Usage rundrx.sh [<eldoradrx_arg> ...]
#

# source the .bashrc file, since KDE seems to strip 
# LD_LIBRARY_PATH when the script is setup to run as a 
# non-KDE application button on the kde menubar.
if [ -f ~/.bashrc ]
then 
   source ~/.bashrc
fi


# location of DDS configuration files
CONF=$ELDORADIR/conf

# The DCPSInforRepo invocation
dcpsinforepo="$DDS_ROOT/bin/DCPSInfoRepo  \
   -NOBITS \
   -DCPSConfigFile $CONF/DCPSInfoRepo.ini \
   -ORBSvcConf $CONF/ORBSvc.conf \
   -ORBListenEndpoints iiop://dcpsrepo:50000 \
   -d $CONF/DDSDomainIds.conf"

# the eldoradrx application
eldoradrx="$ELDORADIR/eldoradrx/eldoradrx \
   --ORB $CONF/ORBSvc.conf \
   --DCPS $CONF/DDSClient.ini \
   --pub $*"
 
# trap the signals on this script, and kill the jobs
trap 'echo -e "$0: SHUTTING DOWN"; trap "" ERR; \
	pkill eldoradrx' ERR SIGINT SIGTERM

# ignore hup so that we can log out and leave running
trap "" HUP
	
# start the DCPS info repo if it is not already running"
dcpspid=`pgrep DCPSInfoRepo`
if [ "$dcpspid" == "" ] ; then
   echo "Starting the DCPS info repository"
   $dcpsinforepo & 
fi

sleep 1

# kill existing eldoradrx
pkill eldoradrx

echo "Starting eldoradrx. Hit control-c to terminate. That"
echo "will terminate all eldoradrx programs"
$eldoradrx

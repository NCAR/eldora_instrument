#! /bin/sh

# location of DDS configurtion files
CONF=~/eldora/conf

# where to save the IOR
REPOFILE=~/eldoraRepo.ior

# The subscriber invocation
subscribe="/home/eldora/eldora/testDDS/subscriber --ORB $CONF/tcp.conf --DCPS $CONF/simpleConf.ini --topic EldoraPulses"

# kill existing jobs
pkill subscriber
pkill DCPSInfoRepo

# Remove existing IOR
rm -f $REPOFILE

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf $CONF/tcp.conf -d $CONF/domain_ids  -o $REPOFILE &
sleep 2

echo "Starting subscriber 1"
$subscribe &

echo "Starting subscriber 2"
$subscribe &

echo "Starting subscriber 3"
$subscribe

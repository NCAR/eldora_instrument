#! /bin/sh

# location of DDS configurtion files
CONF=~/eldora/conf

# where to save the IOR
REPOFILE=~/eldoraRepo.ior

# The publisher invocation
publish="./publisher -ORBSvcConf $CONF/tcp.conf -DCPSConfigFile $CONF/simpleConf.ini"

# The subscriber invocation
subscribe="./subscriber --ORB $CONF/tcp.conf --DCPS $CONF/simpleConf.ini --topic testtopic"

# kill existing jobs
pkill publisher
pkill subscriber
pkill DCPSInfoRepo

# Remove existing IOR
rm -f $REPOFILE

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf $CONF/tcp.conf -d $CONF/domain_ids  -o $REPOFILE &
sleep 2

echo "Starting publisher"
$publish &
sleep 2

echo "Starting subscriber 1"
$subscribe &

echo "Starting subscriber 2"
$subscribe &

echo "Starting subscriber 3"
$subscribe







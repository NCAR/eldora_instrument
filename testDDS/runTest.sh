#! /bin/sh

# location of DDS configurtion files
CONF=~/eldora/conf

# where to save the IOR
REPOFILE=~/eldoraRepo.ior

# The publisher invocation
producer="./producer --ORB $CONF/tcp.conf --DCPS $CONF/simpleConf.ini --delta 5000 --topic EldoraPulses"

# The consumer invocation
consume="./consumer --ORB $CONF/tcp.conf --DCPS $CONF/simpleConf.ini --pulse EldoraPulses --ts EldoraTS"

# kill existing jobs
pkill producer
pkill consumer
pkill DCPSInfoRepo

# Remove existing IOR
rm -f $REPOFILE

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill producer; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf $CONF/tcp.conf -d $CONF/domain_ids  -o $REPOFILE &
sleep 2

echo "Starting producer"
$producer &
sleep 2

echo "Starting consumer 1"
$consume &

echo "Starting consumer 2"
$consume &

echo "Starting consumer 3"
$consume







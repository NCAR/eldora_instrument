#! /bin/sh

# location of DDS configurtion files
CONF=~/eldora/conf

# where to save the IOR
REPOFILE=~/eldoraRepo.ior

# The consumer invocation
consume="/home/eldora/eldora/testdds/consumer --ORB $CONF/tcp.conf --DCPS $CONF/simpleConf.ini --pulse EldoraPulses --ts EldoraTS"
produce="./testrr314 --sim --pub --nci 50 --gates 501 --numiq 11"
# kill existing jobs
pkill consumer
pkill DCPSInfoRepo

# Remove existing IOR
rm -f $REPOFILE

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf $CONF/tcp.conf -d $CONF/domain_ids  -o $REPOFILE &

sleep 2
echo "Starting consumer 1"
$consume &

sleep 2
echo "Starting consumer 2"
$consume &

sleep 2
echo "starting rr314 producer"
$produce



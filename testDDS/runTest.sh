#! /bin/sh

pkill publisher
pkill subscriber
pkill DCPSInfoRepo

CONF=~/eldora/conf
REPOFILE=~/eldoraRepo.ior

rm -f $REPOFILE

sleep 1

trap "pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf $CONF/tcp.conf -d $CONF/domain_ids  -o $REPOFILE &

sleep 2

echo "Starting publisher"
./publisher -ORBSvcConf $CONF/tcp.conf -DCPSConfigFile $CONF/simpleConf.ini &

sleep 2

echo "Starting subscriber 1"
./subscriber -ORBSvcConf $CONF/tcp.conf -DCPSConfigFile $CONF/simpleConf.ini  &

echo "Starting subscriber 2"
./subscriber -ORBSvcConf $CONF/tcp.conf -DCPSConfigFile $CONF/simpleConf.ini  &

echo "Starting subscriber 3"
./subscriber -ORBSvcConf $CONF/tcp.conf -DCPSConfigFile $CONF/simpleConf.ini



#! /bin/sh

pkill DCPSInfoRepo
pkill publisher
pkill subscriber

trap "pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo -o repo.ior -d conf/domain_ids -ORBSvcConf conf/tcp.conf &

sleep 5

echo "Starting publisher"
./publisher -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini -u 10 &

sleep 5

echo "Starting subscriber"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

echo "Starting subscriber"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini



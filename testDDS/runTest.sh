#! /bin/sh

pkill DCPSInfoRepo
pkill publisher
pkill subscriber

trap "pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo -o repo.ior -d conf/domain_ids -ORBSvcConf conf/tcp.conf &

sleep 5

echo "Starting publisher"
./publisher -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini -u 1000 &

sleep 5

echo "Starting subscriber 1"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

echo "Starting subscriber 2"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

echo "Starting subscriber 3"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini



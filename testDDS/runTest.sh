#! /bin/sh

pkill publisher
pkill subscriber
pkill DCPSInfoRepo

sleep 1

trap "pkill subscriber; pkill publisher; pkill DCPSInfoRepo" SIGINT SIGTERM

echo "Starting DCPSInfoRepo"
$DDS_ROOT/bin/DCPSInfoRepo  -ORBSvcConf conf/tcp.conf -o repo.ior -d conf/domain_ids &

sleep 5

echo "Starting publisher"
./publisher -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

sleep 10

echo "Starting subscriber 1"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

echo "Starting subscriber 2"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini &

echo "Starting subscriber 3"
./subscriber -ORBSvcConf conf/tcp.conf -DCPSConfigFile conf/simpleConf.ini 



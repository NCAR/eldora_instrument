#! /bin/sh

# kill existing jobs
pkill producer
pkill consumer
pkill DCPSInfoRepo

# trap the signals on this script, and kill the jobs
trap "echo \"killing jobs\"; pkill consumer; pkill producer; pkill DCPSInfoRepo" SIGINT SIGTERM

./runDCPS.sh &
sleep 2

./runProducer.sh &
sleep 1

./runConsumer.sh &

./runConsumer.sh &

./runConsumer.sh







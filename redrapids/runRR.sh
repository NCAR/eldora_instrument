#! /bin/sh
#
# Usage runRR.sh [<testrr314_arg> ...]
#

#
# Find the Eldora top directory (one directory up from where this script lives), 
# so that we can find the config files
#
topdir="${0%/*}/.."
# If we weren't run using an absolute path, the prepend the working directory
# to get an absolute path
if [[ ${topdir:0:1} != '/' ]]; then topdir="$PWD/$topdir"; fi

# location of DDS configurtion files
CONF=$topdir/conf

produce="$topdir/redrapids/testrr314 \
   --ORB $CONF/tcp.conf \
   --DCPS $CONF/producer.ini \
   --pub $*"
 
# kill existing jobs
#pkill testrr314

# trap the signals on this script, and kill the jobs
trap 'echo -e "$0: SHUTTING DOWN"; trap "" ERR; \
	pkill testrr314' ERR SIGINT SIGTERM
	
echo "starting rr314"
$produce




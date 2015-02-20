IP_ADDRESS=""

if [ $# -eq 0 ]; then
	echo "usage: $0 <ip_address>"
	exit 0
fi

IP_ADDRESS="$1"

function copy_services {
	scp BB.* root@$1:/home/tbs/bin/
}

function copy_libs {
	scp ./lib/lib* root@$1:/home/tbs/lib/
}
function copy_www { 
	ssh root@$1 rm -r /home/tbs/www/public
	scp -r ./www root@$1:/home/tbs/
}


copy_libs $IP_ADDRESS
copy_services $IP_ADDRESS
copy_www $IP_ADDRESS



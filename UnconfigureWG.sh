/usr/local/share/openvswitch/scripts/ovs-ctl stop
ip link set s1 down
ovs-vsctl del-port s1 wg0
ip link set wg0 down
ip link delete wg0
rm -f privatekey
rm -f publickey
ip link add dev wg0 type wireguard
ip addr add 10.0.0.1/24 dev wg0
ovs-vsctl add-port s1 wg0
ip link set wg0 up
ip link set s1 up
/usr/local/share/openvswitch/scripts/ovs-ctl start


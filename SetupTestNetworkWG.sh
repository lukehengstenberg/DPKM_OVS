echo "Automatic setup script of OVS test environment."
echo "Starting ovs server, switch and managers."
/usr/local/share/openvswitch/scripts/ovs-ctl start
echo "Tearing down old bridge (if exists)."
ovs-vsctl --if-exists del-br s1
echo "Removing IP table rules (if exists)."
ip link set wg0 down
echo "Removing old WG interface and host (if exists)."
ip link delete wg0
#ip link delete eth10
#rmmod dummy
#echo "Creating a host."
#modprobe dummy
#lsmod | grep dummy
#ip link add eth10 type dummy
#ip addr add 192.168.100.8/24 dev eth10
#ip addr show eth10
echo "Creating a switch."
ovs-vsctl add-br s1
echo "Adding Unconfigured WireGuard interface."
ip link add dev wg0 type wireguard
ip addr add 10.0.0.1/24 dev wg0 
echo "Configuring switch address and ports."
ovs-vsctl add-port s1 wg0
#ovs-vsctl add-port s1 enp0s3
#ip addr flush dev enp0s3
ip addr add 192.168.0.8/24 dev s1
#ovs-vsctl add-port s1 eth10
ovs-vsctl show
echo "Setting up OpenFlow controller."
ovs-vsctl set-controller s1 tcp:192.168.0.9:6653
ovs-vsctl show
echo "Configuring the network."
ip link set wg0 up
#ip link set eth10 up
ip link set s1 up
ovs-ofctl show s1
ovs-appctl vlog/set warn
echo "Setup complete."


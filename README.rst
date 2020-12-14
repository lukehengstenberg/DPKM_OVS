.. NOTE(stephenfin): If making changes to this file, ensure that the
   start-after/end-before lines found in 'Documentation/intro/what-is-ovs'
   are kept up-to-date.

Data Plane Key Management (DPKM)
====================================
This GitHub Repository contains extensions to Open vSwitch for integration of the Data Plane Key Management scheme DPKM.
DPKM enables automated configuration of WireGuard network tunnels between nodes in the SDN data plane. OVS has been extended in multiple locations to decode and handle six new OpenFlow messages, one new OXM, and fifteen new error messages. DPKM_OVS nodes rely on a WireGuard interface integrated into a port before or at launch. Script is used to automatically setup/tear down WireGuard tunnels based on communication from the DPKM_Floodlight controller. Floodlight was extended to perform eight procedures for Configuring WireGuard, Unconfiguring WireGuard, Adding Peers, Deleting Peers, Starting Encrypted Communication, Ending Encrypted Communication, Rekeying, and Compromise/Key Revocation. New REST APIs were defined for initiating all procedures from an extended version of the Floodlight Web UI <https://github.com/lukehengstenberg/DPKM_Floodlight-webui>. Messages operate in a request/response pattern, where each DPKM message triggers OVS to execute a WireGuard command, and return a dpkm_status or error response. 
DPKM relies on the modified Floodlight controller to be set-up first with the instructions here <https://github.com/lukehengstenberg/DPKM_Floodlight>. 

DPKM was completed as part of an MSc project at Swansea University and is still a work in progress.
Extensive testing with multiple nodes is needed since the project was built and tested for a single controller with two nodes. Several issues exist in the OVS code which will be addressed as development continues. These include: 

- Unconfigure.sh script is forced to unconfigure WireGuard by setting the switch as down, removing the interface, and then relaunching with a blank interface. This causes an error to be thrown in the controller.
- Switch has to be called s1.
- Running the dpkm_mn_custom.py script builds a custom mininet topology but the port number for WireGuard has to be 1 or changed in the Floodlight code to correctly set the flow.
- WireGuard is layer three only so additional code is needed to encapsulate non-IP packets as IP. 
- Better native support for WireGuard related script in ovs daemons, ideally at the same level as IPsec.

The following files are noteworthy when trying to understand DPKM:

- /include/openflow/dpkm-ext.h: Openflow extensions for the Data Plane Key Management Protocol. This class defines the enu-merators and structs representing messages and flags for carrying out specific functionality. De-signed to match the format of messages in the floodlight controller.
- /include/openvswitch/ofp-errors.h: Added fifteen error messages toenum ofperr,lines 895-929.
- /include/openvswitch/ofp-msgs.h: Added message definitions to enum ofpraw and enum ofptype, lines 521-540, 592-598.
- /lib/ofp-util.c: Added OpenFlow extensions to decode DPKM messages. 
- /ofproto/ofproto.c: Main Openflow extensions for the Data Plane Key Management Protocol. The functions from lines 6270 onwards are used to modify the WireGuard configuration using script, handle messages received from the controller, and create status responses based on the state. 
- ConfigureWG.sh: Script for configuring WireGuard.
- SetupTestNetworkWG.sh: Script for quick setup of network topology.
- UnconfigureWG.sh: Script for unconfiguring WireGuard.
- dpkm_mn_custom.py: Script for custom mininet topology.

The project would benefit from a professionals expertise to clean-up code and integrate better native support.
For any questions or queries, contact the developer at this email <mailto:lukehengstenberg@gmail.com>.

DPKM OVS Node Setup Instructions
=================================
1. Install Ubuntu 18.04 and Setup Floodlight with instructions at <https://github.com/lukehengstenberg/DPKM_Floodlight>.
2. Login to the VM hosting the DPKM node and clone the git repo with ``git clone https://github.com/lukehengstenberg/DPKM_OVS.git``. Rename the directory to ``ovs`` for convenience.
3. Add the file names ``ConfigureWG.sh``, ``UnconfigureWG.sh``, ``SetupTestNetworkWG.sh``, ``dpkm_mn_custom.py`` to the file ``distfiles``. This will stop a compiler error when initially building OVS. In future these files need to be explicitly included in the distribution.
4. Modify the contents of ``SetupTestNetworkWG.sh`` to set the desired WireGuard IP address (line 22), change the switch IP address to the VM IP (line 27), and the controller IP address to match the Floodlight VM setup previously (line 31). Make sure to add the WireGuard IP to ``UnconfigureWG.sh``.
5. Move ``SetupTestNetworkWG.sh`` to preferred location and set as executable with ``chmod 111 SetupTestNetworkWG.sh``.
6. Move ``ConfigureWG.sh`` and ``UnconfigureWG.sh`` to the root directory with ``cp ConfigureWG.sh /``. As a root user change the file permissions to executable with ``chmod 111 ConfigureWG.sh``.
7. Install the full list of requirements on the official OVS website. <https://docs.openvswitch.org/en/latest/intro/install/general/#build-requirements>.
8. Python 3 can be installed from this link <https://phoenixnap.com/kb/how-to-install-python-3-ubuntu>>, and running the following commands should satisfy all dependencies. Get any missing packages based on compiler errors. 

``sudo apt-get install -y build-essential
sudo apt-get install -y fakeroot
sudo apt-get install -y debhelper
sudo apt-get install -y autoconf
sudo apt-get install -y automake
sudo apt-get install -y libssl-dev
sudo apt-get install -y bzip2
sudo apt-get install -y openssl
sudo apt-get install -y graphviz
sudo apt-get install -y python-all
sudo apt-get install -y procps
sudo apt-get install -y python-qt4
sudo apt-get install -y python-zopeinterface
sudo apt-get install -y python-twisted-conch
sudo apt-get install -y libtool
sudo apt-get install -y dh-autoreconf
sudo apt-get install -y python3-sphinx
sudo apt-get install -y libelf-dev``

9. In the ovs directory run ``./boot.sh`` followed by ``./configure`` to setup the OVS package. Consult the different configure options on the OVS website if extending development. It is recommended to enable compiler errors with ``--enable-Werror`` to assist with debugging.
10. Follow the building <https://docs.openvswitch.org/en/latest/intro/install/general/#building> and starting <https://docs.openvswitch.org/en/latest/intro/install/general/#starting> instructions on the OVS website to build and start the daemons/scripts.
11. Install WireGuard by running ``sudo apt update``, followed by ``sudo apt install wireguard``. WireGuard is supported out of the box on Ubuntu 20.4 but DPKM was built for Ubuntu 18.04.
12. Install mininet by running ``sudo apt update``, followed by ``sudo apt install mininet``. This is for testing the switch as a virtual topology with hosts.
13. The switch can either be launched directly with ``./SetupTestNetworkWG.sh`` or through mininet with ``./dpkm_mn_custom.py``. The ``SetupTestNetworkWG`` script will create a new switch with an integrated WireGuard interface but no hosts. The ``dpkm_mn_custom`` script will create a custom mininet topology with one switch, two hosts but no WireGuard interface. WireGuard will have to be created and included separately by running the following commands in order: ``sudo ip link add dev wg0 type wireguard``, ``sudo ip addr add <IP>/24 dev wg0``, ``sudo ip link set wg0 up``, ``sudo ovs-vsctl add-port s1 wg0``.
14. Confirm by running ``sudo ovs-vsctl`` to display the switch, controller connection, and ports. Running ``sudo wg`` should display the blank interface with a random listening-port.
15. WARNING: The ``dpkm_mn_custom.py`` file needs to be modified to include the controller IP and the dpid/mac values may need to be changed to avoid conflict with other switches. In Floodlight the out port in the ``constructFlowAdd`` method of the ``DpkmFlows.java`` class may need to be modified to the correct port number of WireGuard. The code currently assumes WireGuard is port 1, which is only the case in the ``SetupTestNetworkWG`` launch approach. Run the command ``sudo ovs-vsctl get Interface wg0 ofport`` to check the port number.
16. The basic DPKM node setup is now complete! The source code can be modified by opening ``ovs`` as a project in a text editor such as Atom. Any changes must be built by running ``sudo make`` and ``sudo make install``, followed by ``./SetupTestNetworkWG.sh`` or ``./dpkm_mn_custom.py`` to rebuild the network topology.

============
Open vSwitch
============

.. image:: https://travis-ci.org/openvswitch/ovs.png
    :target: https://travis-ci.org/openvswitch/ovs
.. image:: https://ci.appveyor.com/api/projects/status/github/openvswitch/ovs?branch=master&svg=true&retina=true
    :target: https://ci.appveyor.com/project/blp/ovs/history
.. image:: https://api.cirrus-ci.com/github/openvswitch/ovs.svg
    :target: https://cirrus-ci.com/github/openvswitch/ovs

What is Open vSwitch?
---------------------

Open vSwitch is a multilayer software switch licensed under the open source
Apache 2 license.  Our goal is to implement a production quality switch
platform that supports standard management interfaces and opens the forwarding
functions to programmatic extension and control.

Open vSwitch is well suited to function as a virtual switch in VM environments.
In addition to exposing standard control and visibility interfaces to the
virtual networking layer, it was designed to support distribution across
multiple physical servers.  Open vSwitch supports multiple Linux-based
virtualization technologies including Xen/XenServer, KVM, and VirtualBox.

The bulk of the code is written in platform-independent C and is easily ported
to other environments.  The current release of Open vSwitch supports the
following features:

- Standard 802.1Q VLAN model with trunk and access ports
- NIC bonding with or without LACP on upstream switch
- NetFlow, sFlow(R), and mirroring for increased visibility
- QoS (Quality of Service) configuration, plus policing
- Geneve, GRE, VXLAN, STT, and LISP tunneling
- 802.1ag connectivity fault management
- OpenFlow 1.0 plus numerous extensions
- Transactional configuration database with C and Python bindings
- High-performance forwarding using a Linux kernel module

The included Linux kernel module supports Linux 3.10 and up.

Open vSwitch can also operate entirely in userspace without assistance from
a kernel module.  This userspace implementation should be easier to port than
the kernel-based switch. OVS in userspace can access Linux or DPDK devices.
Note Open vSwitch with userspace datapath and non DPDK devices is considered
experimental and comes with a cost in performance.

What's here?
------------

The main components of this distribution are:

- ovs-vswitchd, a daemon that implements the switch, along with a companion
  Linux kernel module for flow-based switching.
- ovsdb-server, a lightweight database server that ovs-vswitchd queries to
  obtain its configuration.
- ovs-dpctl, a tool for configuring the switch kernel module.
- Scripts and specs for building RPMs for Citrix XenServer and Red Hat
  Enterprise Linux.  The XenServer RPMs allow Open vSwitch to be installed on a
  Citrix XenServer host as a drop-in replacement for its switch, with
  additional functionality.
- ovs-vsctl, a utility for querying and updating the configuration of
  ovs-vswitchd.
- ovs-appctl, a utility that sends commands to running Open vSwitch daemons.

Open vSwitch also provides some tools:

- ovs-ofctl, a utility for querying and controlling OpenFlow switches and
  controllers.
- ovs-pki, a utility for creating and managing the public-key infrastructure
  for OpenFlow switches.
- ovs-testcontroller, a simple OpenFlow controller that may be useful for
  testing (though not for production).
- A patch to tcpdump that enables it to parse OpenFlow messages.

What other documentation is available?
--------------------------------------

.. TODO(stephenfin): Update with a link to the hosting site of the docs, once
   we know where that is

To install Open vSwitch on a regular Linux or FreeBSD host, please read the
`installation guide <Documentation/intro/install/general.rst>`__. For specifics
around installation on a specific platform, refer to one of the `other
installation guides <Documentation/intro/install/index.rst>`__

For answers to common questions, refer to the `FAQ <Documentation/faq>`__.

To learn about some advanced features of the Open vSwitch software switch, read
the `tutorial <Documentation/tutorials/ovs-advanced.rst>`__.

Each Open vSwitch userspace program is accompanied by a manpage.  Many of the
manpages are customized to your configuration as part of the build process, so
we recommend building Open vSwitch before reading the manpages.

License
-------

The following is a summary of the licensing of files in this distribution.
As mentioned, Open vSwitch is licensed under the open source Apache 2 license.
Some files may be marked specifically with a different license, in which case
that license applies to the file in question.


Files under the datapath directory are licensed under the GNU General Public
License, version 2.

File build-aux/cccl is licensed under the GNU General Public License, version 2.

The following files are licensed under the 2-clause BSD license.
    include/windows/getopt.h
    lib/getopt_long.c
    lib/conntrack-tcp.c

The following files are licensed under the 3-clause BSD-license
    include/windows/netinet/icmp6.h
    include/windows/netinet/ip6.h
    lib/strsep.c

Files under the xenserver directory are licensed on a file-by-file basis.
Refer to each file for details.

Files lib/sflow*.[ch] are licensed under the terms of either the
Sun Industry Standards Source License 1.1, that is available at:
        http://host-sflow.sourceforge.net/sissl.html
or the InMon sFlow License, that is available at:
        http://www.inmon.com/technology/sflowlicense.txt

Contact
-------

bugs@openvswitch.org

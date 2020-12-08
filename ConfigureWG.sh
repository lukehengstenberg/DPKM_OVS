wg genkey | tee privatekey | wg pubkey > publickey
chmod 0 privatekey
chmod 444 publickey
wg set wg0 listen-port 51820 private-key ./privatekey

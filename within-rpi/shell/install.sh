mkdir /startup
cp -i ./enable_hotspot.sh /startup/enable_hotspot.sh
cp -i ./enable_ssh.sh /startup/enable_ssh.sh

bash ./fix_shm_perms.sh

cp -i ./Hotspot.nmconnection /etc/NetworkManager/system-connections/Hotspot.nmconnection
cp -i ./rc.local /etc/rc.local

echo "Reboot to apply changes and begin hotspot"
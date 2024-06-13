# Note: This script may not work due to git changing file endings.
# Files are short, can easily retype (only needs to be done once per installation)

mkdir /startup
cp -i ./enable_hotspot.sh /startup/enable_hotspot.sh
cp -i ./enable_ssh.sh /startup/enable_ssh.sh

bash ./fix_shm_perms.sh

# Copy the values by hand, in order to have correct UUID. Run ./enable_hotspot.sh first to generate starting file
#cp -i ./Hotspot.nmconnection /etc/NetworkManager/system-connections/Hotspot.nmconnection
cp -i ./rc.local /etc/rc.local

echo "Reboot to apply changes and begin hotspot"
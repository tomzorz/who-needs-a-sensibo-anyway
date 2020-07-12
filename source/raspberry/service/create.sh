sudo systemctl stop ac_control
sudo cp ac_control.service /etc/systemd/system/ac_control.service
sudo systemctl enable ac_control
sudo systemctl start ac_control
sudo systemctl status ac_control
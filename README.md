# fingerprint
STARHIGH BIO LRU 1500 Finger Reader 

Enable this fingerprint reader on Linux.

- Install:
On Ubuntu run:
git clone https://github.com/paulosgf/fingerprint.git
cd fingerprint
sudo cp fingerprint.desktop /usr/share/applications/
sudo cp 20-fingerprint.rules /etc/udev/rules.d/
sudo sudo usermod -aG users $USER
cd fingerprint/build
make
sudo make install
sudo make post_install
cd ..
cp fingerprint.png /usr/local/fingerprint/

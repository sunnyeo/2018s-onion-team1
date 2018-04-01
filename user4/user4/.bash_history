cd ~
gpg --gen-key 
ls
gpg --export-secret-keys --armor > user4.priv
ls
cat user4.priv 
gpg --export-key --armor > user4.priv
gpg --list-keys
gpg --export 88D68033 --armor > user4.pub
cat user4.pub 
gpg --armor --export 88D68033 > user4.pub
cat user4.p
cat user4.pub 
ls
pwd
sudo cp user4.p* /home/laura/Dropbox/master/2018s-onion-team1/src/TESTKEY
./dvonion 

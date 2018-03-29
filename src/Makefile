all:
	gcc -o dbserver dbserver.c
	gcc -o dvonion dvonion.c route.c key_manager.c msg_encrypt.c -lpthread -lncurses
	cp dvonion /home/user1/
	cp dvonion /home/user2/
	cp dvonion /home/user3/
	#copy public key
	cp TESTKEY/user*.pub /home/user1/
	cp TESTKEY/user*.pub /home/user2/
	cp TESTKEY/user*.pub /home/user3/
	#copy private key
	cp TESTKEY/user1.priv /home/user1/
	cp TESTKEY/user2.priv /home/user2/
	cp TESTKEY/user3.priv /home/user3/
	#gpgfile permission setting
	chmod -R 777 /home/user1/.gnupg
	chmod -R 777 /home/user2/.gnupg
	chmod -R 777 /home/user3/.gnupg
	echo "----------end of file------------" > OnionUser.db # initialization db file
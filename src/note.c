// How To Import Other Users' Public Keys
// There is also the possibility that the person you are wishing to communicate with has uploaded their key to a public key server. 
gpg --import name_of_pub_key_file


// 내 퍼블릭키를 익스포트하기
gpg --armor --export your_email@address.com

// 파일에 서명하기
gpg --encrypt --sign --armor -r person@email.com name_of_file

// 텍스트 디크립트 하기 : gpg치고 -> 암호화된값 타이핑하고 -> ctrl+D
gpg file_name

/*
Laura$ gpg --import hansh17.pub 
gpg: key 6DBC89AE: public key "Seongho Han (SH) <hansh09@kaist.ac.kr>" imported
gpg: Total number processed: 1
gpg:               imported: 1  (RSA: 1)
gpg: 3 marginal(s) needed, 1 complete(s) needed, PGP trust model
gpg: depth: 0  valid:   1  signed:   0  trust: 0-, 0q, 0n, 0m, 0f, 1u
Laura$ 
Laura$ 
Laura$ 
Laura$ 
Laura$ gpg --recipient 6DBC89AE --encrypt lll.txt 
gpg: 58875C4D: There is no assurance this key belongs to the named user

pub  2048R/58875C4D 2018-03-15 Seongho Han (SH) <hansh09@kaist.ac.kr>
 Primary key fingerprint: 4465 8F75 5832 0B8E 8593  3E6B 772B C0C9 6DBC 89AE
      Subkey fingerprint: CDE0 B4D4 3AF1 A651 9122  DFD1 9ECE 7FD2 5887 5C4D

It is NOT certain that the key belongs to the person named
in the user ID.  If you *really* know what you are doing,
you may answer the next question with yes.

Use this key anyway? (y/N) y
Laura$ ls
dbserver_client.c  foo.txt      lll.txt      msg_encrypt.c  msg_send.c    note.c            pem
dbserver_server.c  hansh17.pub  lll.txt.gpg  msg_send       msg_struct.h  OnionMessenger.c  test.c
Laura$ cat lll.txt.gpg 
�
  ���X�\M�Gcg��������|�u�;�j͇������N�Y�R�nk����	��b�l�4��PܩH�h#l�x�H��ʬj��k�
�O�f��,�[��I	>�w?�(3�g6�_�
LϿ�*r��G%�k(��4�DjM�C�Y�̰d�;�x�6��`�-nF��!�y�)0���WED�J>V缢u��g�}�\#w�/�*���g�A0x_׾���W�@�V�Z���'m�w�w`+��M
*/

/*
두번째시도
Laura$ gpg --import hansh17.pub 
gpg: key 6DBC89AE: "Seongho Han (SH) <hansh09@kaist.ac.kr>" not changed
gpg: Total number processed: 1
gpg:              unchanged: 1

"gpg: key" 문자열 다음으로 파싱해오면될것같은데...
*/




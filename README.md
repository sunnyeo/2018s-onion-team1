# 2018s-onion-team1

# Team member
- Jiwon Choi
- Seongho Han
- Mincheol son
- Dauren serkeshev

# Requirement
- message: encryption / decryption (input: message, receiver's github ID)
- network: send and receive information / refresh in
- server: update databases / give information to client
- client: manage github ID, IP adderess, GPG key of other users / send messages / decide receiver

- require to making a rule to decide routes (when A choose B as a receiver, how A reach B? through A-D-E-C-B? or A-C-D-E-B?)



# Requirement 
- Onion Messenger가 동작하기 위해서는 2개의 서버가 필요합니다. 
1. DB서버 : 현재 네트워크에 접속한 사람들의 목록을 제공해 주는 서버. 
2. 공개키 서버 : 사용자들의 공개키를 배포하는 서버. Github IndividualKeys 페이지를 사용하였습니다. 


# Onion Protocol
<imb src = "https://en.wikipedia.org/wiki/File:Onion_diagram.svg">
Onion 은 연속적인 암호화 레이어로 메시지를 "포장"하여 형성된 데이터 구조입니다.  
이러한 데이터는 목적지에 도달하기 전에 다수의 중간 컴퓨터에 의해 암호가 해독됩니다.  
원래 메시지는 한 노드에서 다음 노드로 전송 될 때 암호화 되어 전송됩니다. 
Circuit에 있는 어떤 노드도 Sender가 데이터의 출처가 되는 노드인지 아니면 다른 중개자 노드인지 알 수 없습니다.


# Protocol
모든 데이터는 파일의 형태로 전송됩니다. 
-----
| 전송 데이터 |
| line 1   | 다음 목적지의 IP    |
| line 2   | 다음 목적지의 Port  |
| line 3~  | 암호화된 데이터 블록 |



# Additional features
1. 네트워크에 들어온 사용자는 DB서버에 [자신의 IP, 자신의 Port, 자신의 GibhubID] 를 등록하고 접속을 알립니다. 
  
2. 사용자는 DB서버에서 네트워크 안에 현재 있는 사용자들의 [IP, Port, GibhubID] 목록을 얻어옵니다.  

3. 사용자는 DB서버에서 얻어온 사용자 목록들을 바탕으로 Github 서버에서 GibhubID.pub키를 다운받은 후, import합니다.  

4. 이 목록에서 Relay Point를 랜덤하게 결정합니다.  
   ex) A가 B로 메시지를 보내고 싶다면, Relay는 A-D-E-C-B 의 형식으로 랜덤하게 결정됩니다.  

5. file.txt 을 route node에 적합하게 암호화합니다.  
   ex) route가 A-D-E-C-B일 때, 노드의 순서에 따라 암호화는 4번 순차적으로 진행됩니다.  
       file1.txt = B의 IP + B의 Port + enc(file.txt,  B의 공개키)  
       file2.txt = C의 IP + C의 Port + enc(file1.txt, C의 공개키)  
       file3.txt = E의 IP + E의 Port + enc(file2.txt, E의 공개키)  
       file4.txt = D의 IP + D의 Port + enc(file3.txt, D의 공개키)  
	   
6. 암호화를 마친 file4.txt는 아래와 같이 전송되며 복호화됩니다.  
   A : file4.txt를 Introduction Points (D)에 전송합니다.  
   D : file4.txt를 D의 개인키로 복호화하여 file3.txt를 뽑아냅니다.  
       file3.txt를 E에게 전송합니다.  
   E : file3.txt를 E의 개인키로 복호화하여 file2.txt를 뽑아냅니다.  
       file2.txt를 C에게 전송합니다.  
   C : file2.txt를 C의 개인키로 복호화하여 file1.txt를 뽑아냅니다.  
       file1.txt를 B에게 전송합니다.  
   B : file1.txt를 B의 개인키로 복호화하여 file.txt를 뽑아냅니다.  

일련의 전송과정으로 네트워크에서는 네트워크에서는 메시지의 송신자, 수신자의 익명성이 보장되며  
암호화된 파일을 안전하게 전송할 수 있습니다.   
       
                  

# Work Distribution

-----
| Name             | Role                     |
|------------------|--------------------------|
| Jiwon Choi       | db Server & db Client    |
| Seongho Han      | PGP encryption & protorol|
| Mincheol son     | Manage PGP key & protorol|
| Dauren serkeshev | protocol, Create docker  |


2018-03-23
2018-03-24
2018-03-25
2018-03-26


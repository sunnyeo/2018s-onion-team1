# 2018s-onion-team1

# Team member
- Jiwon Choi(Team leader)
- Seongho Han
- Mincheol son
- Dauren serkeshev

# Requirement 
- Onion Messenger가 동작하기 위해서는 2개의 서버가 필요합니다.  

1. DB서버: 현재 네트워크에 접속한 사람들의 목록을 제공해주는 서버입니다. 

2. 공개키 서버: 사용자들의 공개키를 배포하는 서버. Github의 IndividualKeys 페이지를 사용하여 공개키 정보를 얻습니다. 


# Onion Router
![Onion](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e1/Onion_diagram.svg/1200px-Onion_diagram.svg.png)  
  
Onion은 연속적인 암호화 레이어로 메시지를 "포장"하여 형성된 데이터 구조입니다. 

원본 메시지는 송신자의 노드에서 다음 노드로 이동할 때 여러 겹으로 암호화 되어 전송됩니다.

암호화된 데이터는 노드를 거쳐갈 때마다 차례로 해독됩니다.

수신자의 노드에 도달했을 때 비로소 원본 메세지의 내용을 알 수 있습니다.

Circuit에 있는 어떤 노드도 이전 노드가 데이터의 출처가 되는 노드인지 아니면 다른 중개자 노드인지 알 수 없습니다. 



# Our Protocol 

모든 데이터는 파일형태로 전송됩니다. 이 때 사용하는 프로토콜은 아래와 같습니다. 

| line No. | Description      |
|----------|------------------|
| line 1   | 다음 중개자의 IP    |
| line 2   | 다음 중개자의 Port  |
| line 3~  | 다음 중개자의 공개키로 암호화된 데이터 블록 |



# Description of onion messenger
1. 네트워크에 들어온 사용자는 DB서버에 [자신의 IP, 자신의 Port, 자신의 GibhubID] 를 등록하고 접속을 알립니다. 
  
2. 사용자는 DB서버에서 현재 네트워크에 접속해있는 사용자들의 [IP, Port, GibhubID] 목록을 얻어옵니다.  

3. 사용자는 DB서버에서 얻어온 사용자 목록을 바탕으로 Github 서버에서 GibhubID.pub키를 다운받은 후, import합니다.  

4. 이 목록에서 Relay Point를 랜덤하게 결정합니다.  
   ex) A가 B에게 메시지를 보낼 때 Relay는 A-D-E-C-B 의 형식으로 랜덤하게 결정됩니다.  

5. file.txt을 route node에 적합하게 암호화합니다.  
   ex) route가 A-D-E-C-B일 때, 노드의 순서에 따라 암호화는 순차적으로 4번 진행됩니다.  
       file1.txt = B의 IP + B의 Port + enc(file.txt,  B의 공개키)  
       file2.txt = C의 IP + C의 Port + enc(file1.txt, C의 공개키)  
       file3.txt = E의 IP + E의 Port + enc(file2.txt, E의 공개키)  
       file4.txt = D의 IP + D의 Port + enc(file3.txt, D의 공개키)  
	   
6. 암호화가 완료된 file4.txt는 아래와 같이 전송되며 차례로 복호화됩니다.  
   A : file4.txt를 Introduction Points (D)에 전송합니다.  
   D : file4.txt를 D의 개인키로 복호화하여 file3.txt를 얻습니다.  
       file3.txt를 E에게 전송합니다.  
   E : file3.txt를 E의 개인키로 복호화하여 file2.txt를 얻습니다.  
       file2.txt를 C에게 전송합니다.  
   C : file2.txt를 C의 개인키로 복호화하여 file1.txt를 얻습니다.  
       file1.txt를 B에게 전송합니다.  
   B : file1.txt를 B의 개인키로 복호화하여 file.txt를 얻습니다.  

일련의 전송과정에서 송신자, 수신자의 익명성이 보장되며 암호화된 파일을 안전하게 전송할 수 있습니다. 

                  
# Work Distribution

| Name             | Role                     |
|------------------|--------------------------|
| Jiwon Choi       | DB Server & DB Client    |
| Seongho Han      | PGP encryption & protorol|
| Mincheol son     | Manage PGP key & protorol|
| Dauren serkeshev | protocol, Create docker  |



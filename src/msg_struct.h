#define MSGLEN 1000
#define ENC_MSGLEN 10000

typedef struct message{
	char from_username[10];
	char msg[MSGLEN];
}message;

typedef struct messageblock{
	char encrypted_message[ENC_MSGLEN];// 암호화된 message 블록을 저장한다
	struct sockaddr_in dest; // 다음 목적지의 ip, port를 포함한다. 
};



/*
struct sockaddr_in {
          short     sin_family;               // 주소 체계
          u_short  sin_port;                 // 16 비트 포트 번호
          struct     in_addr  sin_addr;   // 32 비트 IP 주소
          char       sin_zero[8];          // 전체 크기를 16 비트로 맞추기 위한 dummy
};
*/
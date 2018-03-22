
// 메시지블록을 PGP퍼블릭키로 암호화한다
// 우선은 플레인텍스트가 PGP퍼블릭키로 암호화/복호화 가능한지 먼저 보자. 

#include <openssl/rsa.h>
#include <openssl/rand.h>

int main()
{
	RSA *keyA, *keyB, *pkeyA, *pkeyB;
	unsigned char cipher_text[256];
	unsigned char plain_text_sender[]="This is RSA test Program";
	unsigned char plain_text_receiver[256];
	unsigned int num;
	int pkA_len, pkB_len;
	unsigned char *pkA,  *temp_pkA;
	unsigned char *pkB, *temp_pkB;

	// A의 RSA키 생성
	keyA=RSA_new();        // key 객체 생성
	keyA=RSA_generate_key(1024,3,NULL,NULL); // key 생성
	if(RSA_check_key(keyA)==1)     // key 유효성 확인
		printf("validate key\n");

	// B의 RSA키 생성
	keyB=RSA_new();        // key 객체 생성
	keyB=RSA_generate_key(1024,3,NULL,NULL); // key 생성
	if(RSA_check_key(keyB)==1)     // key 유효성 확인
		printf("validate key\n");


	// 상대방에게 전송할 공개키 추출
	// A의 공개키 추출
	pkA = (unsigned char *)malloc(1024);
	memset(pkA, 0, 1024);
	temp_pkA = pkA;
	pkA_len=i2d_RSAPublicKey(keyA, &pkA);  // 공개키 추출
	pkA = temp_pkA;


	// B의 공개키 추출
	pkB = (unsigned char *)malloc(1024);
	memset(pkB, 0, 1024);
	temp_pkB = pkB;
	pkB_len=i2d_RSAPublicKey(keyB, &pkB);  // 공개키 추출
	pkB = temp_pkB;
	printf("pkB : %s\n",pkB);
	printf("pkB size : %d\n",pkB_len);


	// 공개키는 서로 교환한 것으로 간주
	// A -> B로 데이터를 암호화하여 보내는 경우
	// B의 공개키 등록
	pkeyB = d2i_RSAPublicKey(NULL, (const unsigned char **)&pkB, (long)pkB_len);
	// B의 공개키로 데이터 암호화
	printf("plaintext=%s\n",plain_text_sender);
	num=RSA_public_encrypt(sizeof(plain_text_sender)-1, plain_text_sender, cipher_text, pkeyB, RSA_PKCS1_PADDING);
	// B가 암호화된 데이터를 받은 것으로 간주
	// B는 자신의 비밀키로 복호화
	num=RSA_private_decrypt(num, cipher_text, plain_text_receiver, keyB, RSA_PKCS1_PADDING);
	plain_text_receiver[num]='\0';
	printf("plaintext=%s\n",plain_text_receiver);

	memset(cipher_text, 0, 256);
	memset(plain_text_receiver, 0, 256);

	// B -> A로 데이터를 암호화하여 보내는 경우
	// A의 공개키 등록
	pkeyA = d2i_RSAPublicKey(NULL, (const unsigned char **)&pkA, (long)pkA_len);
	// B -> A, A의 공개키로 데이터 암호화
	printf("plaintext=%s\n",plain_text_sender);
	num=RSA_public_encrypt(sizeof(plain_text_sender)-1, plain_text_sender, cipher_text, pkeyA, RSA_PKCS1_PADDING);
	// A가 암호화된 데이터를 받은 것으로 간주
	// A는 자신의 비밀키로 복호화
	num=RSA_private_decrypt(num, cipher_text, plain_text_receiver, keyA, RSA_PKCS1_PADDING);
	plain_text_receiver[num]='\0';
	printf("plaintext=%s\n",plain_text_receiver);
	RSA_free(keyA); 
	RSA_free(keyB); 
	RSA_free(pkeyA); 
	RSA_free(pkeyB); 
	//free(pkA); 
	//free(pkB);   

}

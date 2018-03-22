// How To Import Other Users' Public Keys
// There is also the possibility that the person you are wishing to communicate with has uploaded their key to a public key server. 
gpg --import name_of_pub_key_file


// 내 퍼블릭키를 익스포트하기
gpg --armor --export your_email@address.com

// 파일에 서명하기
gpg --encrypt --sign --armor -r person@email.com name_of_file

// 텍스트 디크립트 하기 : gpg치고 -> 암호화된값 타이핑하고 -> ctrl+D
gpg file_name


1. 서버에서 현재있는사람들 목록을받아옴
2. 사람들의공개키를 git에서받아와서 등록함
3. 사람들의 등록된키를이용해서 텍스트인크립트하기 -> 텍스트인크립트하기 -> 텍스트인크립트하기...
4. 
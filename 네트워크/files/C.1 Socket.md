## 1. 소켓(Socket)의 본질

- **개념:** 네트워크 통신을 위해 운영체제(OS) 커널이 제공하는 추상화된 통신 창구(엔드포인트)입니다.
- **존재 이유:** 개발자가 L1~L4(물리선, 랜카드, IP, TCP/UDP 등)의 복잡한 하위 알고리즘을 직접 구현하지 않고, OS가 만든 소켓이라는 파일 인터페이스(File Descriptor)에 **`send` / `recv` (C) 또는 `sendall` / `recv` (Python)** 함수만 호출하여 데이터를 주고받을 수 있게 해줍니다.

## 2. 4-Tuple (4가지 조합)이란?

인터넷 세상에서 **단 하나의 예외도 없이 특정 통신(연결)을 고유하게 식별하기 위한 4가지 정보의 조합**입니다.
$$\text{4-Tuple} = \Big[ \text{출발지 IP}, \;\text{출발지 Port}, \;\text{목적지 IP}, \;\text{목적지 Port} \Big]$$
### 왜 '목적지 IP + Port'만으로는 부족할까?

내 PC(`192.168.1.12`)에서 네이버 웹서버(`223.130.192.247:443`)에 접속하는 크롬 탭을 2개 열었을 때:
- **크롬 1번 탭:** $[192.168.1.12 : \mathbf{51234}] \;\leftrightarrow\; [223.130.192.247 : 443]$
- **크롬 2번 탭:** $[192.168.1.12 : \mathbf{51235}] \;\leftrightarrow\; [223.130.192.247 : 443]$
    
목적지 IP/Port가 동일하더라도, **내 OS가 임시 포트(Dynamic Port)를 서로 다르게 할당**하여 4-Tuple 식별자를 다르게 만들기 때문에 두 대화 창구가 엉키지 않고 각각 처리됩니다.
## 3. 코드(C & Python)로 보는 소켓 생성 과정

### ① C언어 (POSIX Socket API)

C언어에서는 구조체(`sockaddr_in`)와 시스템 콜을 통해 OS 커널 메모리에 직접 4-Tuple을 등록합니다.
```c
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // 1. socket(): OS 커널에 L4(TCP) 통신용 파일 디스크립터(fd) 생성 요청
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 서버 자신의 IP (0.0.0.0)
    server_addr.sin_port = htons(8080);      // 바인딩할 서버 포트

    // 2. bind(): 4-Tuple 중 [서버 IP : 서버 Port]를 이 소켓에 귀속
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 3. listen(): 클라이언트의 3-Way Handshake 요청을 받아들일 대기 상태 전환
    listen(server_fd, 5);

    // 4. accept(): 접속한 클라이언트의 [클라이언트 IP : 클라이언트 Port]를 받아와 4-Tuple 완성
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    
    // 이제 client_fd는 독립된 4-Tuple을 가지는 전용 대화 소켓이 됨
}
```
### ② Python (`socket` 모듈)

파이썬은 4-Tuple을 `(IP, Port)` 튜플 형태로 감싸서 객체지향적으로 제공합니다.

```Python
import socket

# ==============================================================================
# 1. 문지기 소켓(Listen Socket) 생성 및 네트워크 설정
# ==============================================================================
# - AF_INET: IPv4 주소 체계 사용
# - SOCK_STREAM: L4 TCP 프로토콜 사용 (신뢰성 있는 연결 지향 통신)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# [소켓 옵션 설정] SOL_SOCKET 레벨의 SO_REUSEADDR 옵션을 1(True)로 활성화
# - 서버 종료 후 'TIME_WAIT' 상태에 빠진 포트 번호를 재부팅 없이 즉시 재사용하기 위함
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# [주소 바인딩 & 대기 상태 전환]
# - '0.0.0.0': 내 PC의 모든 네트워크 인터페이스(LAN, Wi-Fi, Loopback)의 요청을 수신
# - 8080: 서비스할 포트 번호 지정
server_socket.bind(('0.0.0.0', 8080))
server_socket.listen()

print("서버 가동 중... (웹 브라우저로 http://localhost:8080 에 접속해보세요)")

# ==============================================================================
# 2. 클라이언트 접속 수락 및 전용 통신 소켓(Connected Socket) 생성
# ==============================================================================
# accept(): 클라이언트의 TCP 3-Way Handshake 요청을 수락할 때까지 블로킹(대기)
# - client_socket  : 해당 클라이언트와 1:1 통신을 전담할 독립된 4-Tuple 소켓
# - client_address : 클라이언트의 (IP, Port) 튜플 정보
client_socket, client_address = server_socket.accept()
print(f"\n[+] 클라이언트 접속 성공! 주소: {client_address}")

# ==============================================================================
# 3. HTTP 요청(Request) 데이터 수신 및 출력
# ==============================================================================
# recv(1024): 소켓 버퍼에서 최대 1024 바이트만큼의 데이터 읽기
# - 브라우저가 보낸 L7 HTTP Request 메시지(바이너리)를 UTF-8 문자열로 디코딩
request = client_socket.recv(1024)
print("\n[내 PC로 들어온 HTTP 요청 패킷 데이터]:")
print(request.decode('utf-8'))

# ==============================================================================
# 4. HTTP 응답(Response) 패킷 생성 및 전송
# ==============================================================================
# L7 HTTP/1.1 프로토콜 규격에 맞춘 응답 메시지 작성
# - Status Line: HTTP/1.1 200 OK (성공)
# - Header     : Content-Type (HTML 문서, UTF-8 인코딩)
# - Body       : 실제 브라우저 화면에 렌더링될 HTML 태그 (\n\n 개행으로 헤더와 구분)
http_response = """HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8

<h1>Hello! Direct Socket Server!</h1>
"""

# sendall(): 작성한 문자열을 바이너리 데이터로 인코딩하여 클라이언트 소켓으로 완벽히 전송
client_socket.sendall(http_response.encode('utf-8'))

# ==============================================================================
# 5. 통신 종료 및 자원 해제
# ==============================================================================
# - client_socket.close(): 해당 클라이언트와의 1:1 대화 세션 종료
# - server_socket.close(): 서버 문지기 소켓을 닫아 새로운 접속 수신 중단
client_socket.close()
server_socket.close()
```

## 4. 💡 보충: OS 커널 내부에서는 이 4-Tuple을 어떻게 다룰까?

네트워크 프로그래밍과 서버 구조를 깊이 이해하기 위해 추가로 알아두면 좋은 **커널 내부 동작 원리**입니다.

### 1. 소켓 해시 테이블 (Socket Hash Table)
랜카드에 패킷이 들어오면, OS 커널은 패킷 헤더에서 `[출발지 IP, 출발지 Port, 목적지 IP, 목적지 Port]`를 추출한 뒤 커널 내부의 해시 테이블(TCP Table)을 검색합니다.

해시 키값으로 단 한 번에 "이 패킷을 수신해야 하는 소켓 파일 디스크립터(fd)"를 찾아내어 해당 소켓의 버퍼(`recv_buffer`)에 데이터를 채워 넣어 줍니다.

### 2. 듣기 소켓(Listen Socket) vs 통신 소켓(Connected Socket)
서버를 작성할 때 포트는 `8080` 하나만 열어두는데 수천 명의 클라이언트가 동시에 연결될 수 있는 이유입니다.
- **Listen Socket (`server_fd`):** `bind()`와 `listen()`으로 생성되는 소켓으로, 오직 '새로운 접속 요청(SYN 패킷)을 받기 위한 문지기 역할'만 수행합니다.
- **Connected Socket (`client_fd`):** `accept()`가 호출되는 순간, 문지기 소켓은 그대로 두고 커널이 '클라이언트 4-Tuple 전용의 새로운 소켓'을 내부에 복사/생성해 반환합니다.
    
    따라서 수만 명의 클라이언트가 `8080` 포트로 들어와도, 각각의 **클라이언트 IP/Port가 다르므로** 모두 서로 다른 독립된 소켓(File Descriptor)으로 완벽히 분리됩니다.
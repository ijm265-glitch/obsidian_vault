**쿠키(Cookie) & 세션(Session)** 방식은 토큰 기반 인증(OAuth, JWT 등)이 등장하기 전부터 현재까지도 널리 쓰이는 가장 전통적인 웹 로그인 및 상태 관리 메커니즘임

HTTP는 기본적으로 **Stateless(무상태성)** 및 **Connectionless(비연결성)** 특징을 가지므로, 서버는 직전 요청을 보낸 사람이 누구인지 기억하지 못합니다. 이 한계를 극복하기 위해 나온 것이 쿠키와 세션입니다.

### 1. 쿠키(Cookie)와 세션(Session)의 역할 분담

- **쿠키 (Cookie):** **클라이언트(브라우저) 측**에 저장되는 작은 텍스트 데이터 조각입니다.
- **세션 (Session):** **서버 측** 메모리(또는 DB/In-memory 저장소)에 저장되는 사용자 상태 데이터입니다.
> 민감한 사용자 정보(아이디, 권한 등)는 서버(세션)에 안전하게 보관하고, 클라이언트(브라우저)에는 해당 세션을 찾아갈 수 있는 임의의 열쇠(Session ID)만 쿠키 형태로 들려주는 방식

### 2. 쿠키 & 세션 동작 원리 (5단계)
#### ① 로그인 및 세션 데이터 생성 (Create)

1. **클라이언트 → 서버:** 아이디/비밀번호 전송
2. **서버 내부 처리:**
    - DB에서 아이디/비밀번호 검증 성공
    - 고유한 난수 문자열 생성: `session_id = "abc123"`
    - 세션 딕셔너리에 데이터 등록:
        ```python
        # 서버 딕셔너리에 Session ID를 Key로 저장
        session_store["abc123"] = {"user_id": 42, "role": "USER"}
        ```
3. **서버 → 클라이언트:** 응답 헤더에 `Set-Cookie: JSESSIONID=abc123` 탑재하여 반환

#### ② 쿠키 저장 및 자동 전송 (Store & Send)

1. **브라우저:** 응답의 `Set-Cookie` 헤더를 읽어 브라우저 쿠키 저장소에 `"JSESSIONID": "abc123"` 형태로 기록
2. **이후 요청:** 사용자가 페이지를 이동하거나 요청을 보낼 때마다, 브라우저가 자동으로 요청 헤더에 담아서 전송
    ```http
    GET /api/my-page HTTP/1.1
    Host: example.com
    Cookie: JSESSIONID=abc123
    ```
#### ③ 서버의 세션 데이터 조회 및 검증 (Read & Verify)

1. **서버 내부 처리:**
    - 요청 헤더의 `Cookie`에서 `"JSESSIONID"` 값인 `"abc123"`을 꺼냄
    - 딕셔너리에서 해당 Key 조회:
        ```python
        session_id = request.cookies.get("JSESSIONID")  # "abc123"
        
        # Key 존재 여부 검증
        if session_id in session_store:
            user_data = session_store[session_id]
        
            # 만료 시간 체크 후 정상 처리
            if is_not_expired(user_data["expires_at"]):
                current_user = user_data["user_id"]  # 42번 유저로 인정!
            else:
                del session_store[session_id]  # 만료되었으면 삭제
                return "401 Unauthorized (세션 만료)"
        else:
            return "401 Unauthorized (로그인 필요)"
        ```
#### ④ 로그아웃 및 세션 파기 (Delete)

1. **사용자가 로그아웃 클릭 시:**
    - 서버는 딕셔너리에서 해당 Key 삭제: `del session_store["abc123"]`
    - 브라우저 쿠키를 만료시키기 위해 `Set-Cookie: JSESSIONID=; Max-Age=0` 응답 전송
2. **결과:** 이제 클라이언트가 실수나 악의로 `"abc123"` 쿠키를 다시 보내더라도, 서버 딕셔너리에는 Key가 존재하지 않으므로 인증이 거부됩니다.

### 3. 쿠키 & 세션 방식의 한계점

이 방식은 웹 초기에 매우 유용했으나, 현대의 분산 시스템과 모바일 환경이 발전하면서 다음과 같은 뚜렷한 한계에 부딪혔습니다.

#### ① 서버 확장성(Scalability) 저하 (가장 큰 한계)

- **문제:** 서버 메모리에 세션 상태를 저장하므로, 트래픽이 늘어나 서버를 여러 대(Scale-out)로 늘릴 때 문제가 발생합니다.
- **상황:** A 서버에서 로그인하여 세션이 생성된 사용자가 다음 요청을 Load Balancer에 의해 B 서버로 보낼 경우, B 서버에는 세션 정보가 없어 **로그인이 풀려버립니다.**
- **우회책:** `Sticky Session`(특정 사용자는 항상 특정 서버로만 연결)이나 `Redis` 같은 중앙 집중식 세션 저장소를 추가로 구축해야 하므로 아키텍처 복잡성과 비용이 커집니다.
#### ② CSRF (Cross-Site Request Forgery) 공격에 취약

- 브라우저는 **요청 시 해당 도메인의 쿠키를 자동으로 첨부**하는 특성이 있습니다.
- 사용자가 로그인된 상태에서 해커가 만든 악성 사이트에 접속하면, 사용자 몰래 내 서비스로 요청(예: 비밀번호 변경, 송금)을 보낼 때 쿠키(Session ID)가 자동으로 같이 넘어가 **사용자 본인인 것처럼 요청이 처리**될 위험이 있습니다.
#### ③ CORS (Cross-Origin Resource Sharing) 및 모바일/App 환경 제약

- **CORS 문제:** 프론트엔드(React, Vue)와 백엔드(API Server) 도메인이 다를 경우, 쿠키 전송 제약(`SameSite`, `Credentials` 옵션 설정 필요)이 복잡해집니다.
- **모바일 웹/앱 환경:** 웹 브라우저가 아닌 iOS/Android Native App은 쿠키를 기본적으로 자동 관리해 주지 않기 때문에 헤더 기반 인증에 비해 구현이 까다롭습니다.
#### ④ 서버 메모리 부하

- 접속 중인 동시 사용자 수가 수십~수백만 명으로 늘어나면 세션 저장소가 사용하는 메모리 양이 비대해져 서버 성능에 부담을 줍니다.

### 4. 정리: 토큰(Token) 기반 인증으로 넘어가는 이유

쿠키 & 세션 방식은 "서버가 상태를 기억해야 한다(Stateful)"는 것이 본질적인 한계입니다.

이를 해결하기 위해 서버가 상태를 갖고 있지 않아도 되는 **"Stateless한 토큰 기반 인증(JWT, OAuth)"** 방식이 대세로 자리 잡게 되었습니다. 서버는 토큰이 유효한지 암호학적으로 검증만 할 뿐, 사용자 로그인 상태를 직접 저장하지 않게 됩니다.
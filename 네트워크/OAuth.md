OAuth(Open Authorization)는 내 비밀번호를 주지 않고도, A 서비스가 B 서비스에 있는 내 정보에 접근할 수 있게 허가해 주는 표준 프로토콜

- **HTTP 프로토콜 핵심**
    - [[HTTP 메시지 구조 (헤더와 바디)|HTTP Request / Response 구조 (Header, Body, Status Code)]]
    - [[HTTP 메서드와 상태코드|HTTP Method (`GET`, `POST`의 차이점 및 사용 목적)]]
    - [[`Redirect` 동작 원리 (3xx 상태 코드와 `Location` 헤더)]]
        
- **보안 프로토콜 (HTTPS/TLS)**
    - [[HTTP와 HTTPS의 차이 (암호화 통신의 필요성)]]
    - [[대칭키/비대칭키(공개키) 암호화의 기본 개념]]
        

### Phase 2. 인증(Authentication) vs 인가(Authorization) & 세션 관리

- **[[Auth의 두 축 구분하기]]**
    - **인증 (Authentication):** "너 누구니?" (신원 확인 - 예: 로그인)
    - **인가 (Authorization):** "너 이거 할 권한 있니?" (권한 부여 - 예: 읽기/쓰기 허용)
    - _핵심:_ OAuth는 기본적으로 **'인가(Authorization)'** 프로토콜이라는 점 파악하기.
        
- **[[Cookie & Session 방식 동작 원리 및 한계점|전통적인 웹 인증/인가 방식]]**
    - Cookie & Session 방식 동작 원리 및 한계점
    - Token 기반 인증 방식이 등장한 배경
        
### Phase 3. 토큰(Token) 기반 보안 및 암호화

OAuth는 권한을 부여받았다는 증표로 '토큰'을 발급하고 검증합니다.

- **JWT (JSON Web Token)**
    - [[JWT의 구조 (`Header` . `Payload` . `Signature`)]]
    - [[`Access Token` vs `Refresh Token`의 역할과 유효기간 관리 전략]]
    - 토큰을 어디에 저장할 것인가? (LocalStorage vs HttpOnly Cookie)
        
- **[[보안 공격 기법에 대한 이해]]**
    - **CSRF (Cross-Site Request Forgery):** 타인의 권한을 도용하여 요청을 보낼 때 발생하는 위협 (OAuth의 `state` 파라미터가 필요한 이유)
    - **PKCE (Proof Key for Code Exchange):** 모바일/Single Page App에서 Authorization Code를 탈취당하지 않기 위해 사용하는 보안 확장 표준
### Phase 4. OAuth 2.0 핵심 동작 메커니즘

- **[[OAuth 2.0의 4가지 주체 (Roles)]]**
    1. **Resource Owner:** 사용자 (서비스 이용자)
    2. **Client:** 우리가 만드는 애플리케이션
    3. **Authorization Server:** 권한을 인증하고 토큰을 발급하는 서버 (예: Google, Kakao 인증 서버)
    4. **Resource Server:** 실제 사용자 데이터가 있는 서버 (예: Google Calendar API)
        
- **[[핵심 승인 방식]] (Grant Types)**
    - **Authorization Code Grant (권한 부여 코드 승인 방식):** 가장 대표적이고 안전한 웹 애플리케이션용 플로우
    - Client Credentials Grant (클라이언트 자격 증명 방식): 사용자 개입 없이 서버 대 서버 통신 시 사용
        
- **[[OIDC (OpenID Connect)]]**
    - OAuth 2.0(인가) 위에서 동작하는 **인증(Authentication)** 레이어
    - `ID Token`의 개념과 소셜 로그인 구현 시 OAuth 2.0 + OIDC가 쓰이는 방식

### Phase 5. 실전 구현 및 연동
- **OAuth Provider 연동 실습**
    - Google Cloud Console 또는 Kakao Developers에서 Client ID / Client Secret 발급받기
    - `Redirect URI`, `Scope`(접근 권한 범위) 설정해 보기
        
- **플로우 직접 따라가기**
    1. 로그인 요청 URL 생성 (`client_id`, `redirect_uri`, `response_type=code`, `scope`, `state`)
    2. 사용자 동의 후 `Authorization Code` 수신
    3. Backend에서 Authorization Code를 Client Secret과 함께 넘겨 `Access Token` 교환
    4. Access Token으로 API 호출하여 사용자 정보 조회
        
- **Network 탭 분석**
    - 개발자 도구 Network 탭을 켜고 실제 소셜 로그인 진행 과정에서의 HTTP 요청/응답, Redirect 흐름 눈으로 확인하기
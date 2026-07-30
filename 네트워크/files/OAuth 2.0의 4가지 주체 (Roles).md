# OAuth 2.0 핵심 주체 (Roles)

OAuth 2.0 프로토콜(RFC 6749)에서 정의하는 **4가지 시스템 구성 요소**와 각각의 기술적 역할 규정.
## 1. Resource Owner (자원 소유자)
- **정의:** 보호된 리소스(사용자 데이터)에 대한 접근 권한을 부여할 수 있는 **주체(사용자)**.
- **주요 역할:**
    - Authorization Server를 통해 자신의 신원 인증(Authentication) 수행.
    - Client가 요청한 접근 권한 범위(`Scope`)를 확인하고 **승인(Consent)**.
- **보안적 특징:** Client 시스템에 자신의 계정 자격 증명(ID/Password)을 절대 직접 노출하지 않음
## 2. Client (클라이언트)
- **정의:** Resource Owner의 승인을 받아 보호된 리소스에 접근하려는 **애플리케이션(서비스)**
- **주요 역할:**
    - Authorization Server에 인증 및 `Access Token` 발급 요청.
    - 발급받은 `Access Token`을 HTTP Header에 실어 Resource Server의 API 호출.
        
- **분류 (보안 수준 기준):**
    - **Confidential Client:** `Client Secret`을 안전하게 은닉할 수 있는 서버 사이드 애플리케이션 (Node.js, Spring, Django 등).
    - **Public Client:** 코드가 클라이언트에 노출되어 `Client Secret`을 보관할 수 없는 애플리케이션 (SPA, Mobile Native App 등).
        

## 3. Authorization Server (권한 부여 서버)

- **정의:** Resource Owner를 인증하고, 권한 승인을 검증하여 Client에게 **토큰을 발급하는 서버**.
- **주요 역할:**
    - Resource Owner의 로그인 및 권한 동의 인터페이스 제공.
    - `Authorization Code` 생성 및 발급.
    - Client 자격 증명(`client_id`, `client_secret`) 및 Code 검증.
    - `Access Token` / `Refresh Token` 발급 및 만료 처리.
        
- **주요 Endpoint:**
    - `Authorization Endpoint`: `/oauth/v2/authorize` (사용자 승인 처리)
    - `Token Endpoint`: `/oauth/v2/token` (토큰 교환 및 발급)
        
## 4. Resource Server (리서버 / 자원 서버)
- **정의:** Resource Owner의 **보호된 데이터(API)를 호스팅하는 서버**.
- **주요 역할:**
    - Client 요청의 HTTP Header(`Authorization: Bearer <Access Token>`)에서 토큰 추출.
    - `Access Token`의 유효성, 만료 여부, 허용 권한 범위(`Scope`) 검증.
    - 토큰이 유효한 경우 요청된 데이터(JSON 등) 반환.
        
- **검증 방식:**
    - **Introspection:** Authorization Server에 직접 토큰 유효성 조회 요청.
    - **Self-Contained (JWT):** 서명(Signature)을 자체적으로 검증하여 승인.
## 주체 간 상호작용 데이터 흐름 (Data Flow)

```
[ Resource Owner ]
       │  ▲
  (1)  │  │ (2)
  인증  │  │ Authorization Code
  동의  ▼  │
[ Client ] ──────── (3) Code + Secret ────────► [ Authorization Server ]
       │   ◄─────── (4) Access Token ───────────
       │
       └─────────── (5) Access Token ───────────► [ Resource Server ]
           ◄─────── (6) Protected Data ──────────
```

1. **(1)~(2)** Client가 브라우저를 통해 Resource Owner를 Authorization Server로 리다이렉트하여 인증 및 권한 승인을 획득(`Authorization Code` 수신).
2. **(3)~(4)** Client가 Backend에서 Authorization Server의 Token Endpoint로 `Authorization Code`와 자격 증명을 전송하여 `Access Token`으로 교환.
3. **(5)~(6)** Client가 `Access Token`을 사용하여 Resource Server의 API를 호출하고 데이터를 수신.
## 핵심 요약

- **OAuth 2.0의 본질:** Client가 Resource Owner의 ID/PW 없이 **Authorization Server가 발급한 Access Token을 사용해 Resource Server의 API에 접근**하는 인가(Authorization) 메커니즘.
- **Server 분리:** 대규모 시스템 구축 시 Authorization Server(토큰 발급/검증)와 Resource Server(실제 API)는 기능 및 데이터베이스 단위로 분리되어 운영됨.
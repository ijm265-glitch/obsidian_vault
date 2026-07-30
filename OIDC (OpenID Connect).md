OIDC(OpenID Connect)는 쉽게 말해 "OAuth 2.0이라는 건물 위에 올린 인증(Authentication) 층"입니다.

기존 OAuth 2.0이 "이 사람이 특정 리소스에 접근할 권한(Authorization)이 있는가?"에 집중했다면, OIDC는 여기에 "이 사용자가 대체 누구인가?(Authentication)"를 증명하는 기능을 표준화하여 얹은 것입니다.


## 1. OAuth 2.0과 OIDC의 차이점

* **OAuth 2.0 (권한 부여)**: "사용자가 내 사진첩 접근을 허용했으니, Access Token으로 사진을 가져오자." (사용자가 누구인지는 관심 없고, 권한 열쇠만 챙김)
* **OIDC (신원 인증)**: "사용자가 로그인했으니, **ID Token**을 까봐서 이 사람의 이름, 이메일, 고유 ID가 뭔지 확인하자."

OAuth 2.0만으로 로그인을 구현하려면 Access Token을 받은 뒤 사용자 정보 API(`/userinfo`)를 매번 추가 호출해야 했습니다. OIDC는 이 과정을 표준화하여 **로그인 성공 시 Access Token과 함께 ID Token을 바로 넘겨주도록** 만든 것입니다.

## 2. ID Token의 개념과 구조

**ID Token**은 사용자의 프로필 정보가 담긴 **JWT(JSON Web Token)** 형태의 디지털 신분증입니다.

JWT 포맷이므로 **Header, Payload, Signature**가 `.`으로 연결되어 있으며, 클라이언트(우리 서버)는 서버 인증 키 없이도 토큰 내부의 Payload(Claims)를 디코딩하여 사용자 정보를 바로 확인할 수 있습니다.

### ID Token 주요 데이터 (Payload 예시)

```json
{
  "iss": "https://accounts.google.com",  // 발급자 (Issuer)
  "sub": "10923849123849123894",         // 사용자 고유 식별자 (Subject)
  "aud": "my-client-id",                 // 수신자 (Client ID)
  "exp": 1722345600,                     // 만료 시간 (Expiration Time)
  "iat": 1722342000,                     // 발급 시간 (Issued At)
  "email": "user@example.com",           // 사용자 이메일
  "name": "홍길동"                         // 사용자 이름
}

```

## 3. 소셜 로그인 시 OAuth 2.0 + OIDC가 작동하는 방식

소셜 로그인(Google, Kakao, Apple 등)을 구현할 때 Scope에 `openid`를 추가하면 OIDC 플로우가 활성화됩니다.

```
[사용자]             [클라이언트 App]                   [인증 서버 (Google 등)]
   |                       |                                    |
   |-- 1. 소셜 로그인 클릭 ->|                                    |
   |                       |-- 2. 인증 요청 (scope=openid 포함) ->|
   |<---------------------- 3. 로그인 및 동의 화면 제공 ------------|
   |-- 4. 로그인 완료 ------>|                                    |
   |                       |<-- 5. Authorization Code 전달 ------|
   |                       |                                    |
   |                       |-- 6. Code (+ PKCE/Secret) 전달 ---->|
   |                       |<-- 7. Access Token + ID Token 발급 -|
   |                       |                                    |
   |                       |-- 8. ID Token 검증 (Signature, iss, aud)
   |                       |-- 9. 사용자 로그인 처리 완료 (세션/토큰 생성)

```

1. **요청 단계**: 클라이언트가 인증 서버로 요청을 보낼 때 `scope=openid profile email`처럼 **`openid` 키워드**를 넣어 전송합니다.
2. **코드 교환**: 앞서 다룬 **Authorization Code Grant** 플로우를 동일하게 거쳐 코드를 얻고, 이를 인증 서버에 보내 토큰을 요청합니다.
3. **토큰 응답**: 인증 서버는 **Access Token**과 함께 ID Token(JWT)을 동시에 반환합니다.
4. **검증 및 로그인**: 클라이언트는 ID Token의 서명(Signature)과 만료일, `aud`(내 Client ID와 일치하는지)를 검증한 뒤, 토큰 내부의 `sub`(고유 ID) 및 `email` 정보를 활용해 자사 서비스의 회원가입/로그인 처리를 완료합니다.

### 한눈에 정리

* **OAuth 2.0**: 권한 부여 프레임워크 (Access Token 발급)
* **OIDC**: OAuth 2.0 기반의 신원 인증 표준 (ID Token 발급)
* **ID Token**: 서명된 JWT 신분증 (사용자 식별용)

소셜 로그인에서 OIDC를 사용하면 별도의 사용자 정보 API 호출을 줄일 수 있고, 규격화된 JWT 검증만으로 안전하게 사용자 신원을 확인할 수 있다는 큰 장점이 있습니다.
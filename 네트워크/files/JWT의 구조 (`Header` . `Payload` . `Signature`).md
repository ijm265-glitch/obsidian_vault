JWT는 점(`.`)을 구분자로 하여 **3개의 독립된 문자열**이 연결된 구조를 가집니다.

```
Header.Payload.Signature
```

실제 JWT를 받으면 `eyJhbGciOi... . eyJzdWIiOi... . SflKxwRJJS...` 와 같이 점 두 개로 구분된 길다란 문자열 형태로 전달됩니다.

### 1. Header (헤더)
헤더는 "이 토큰을 어떻게 검증할 것인가?"에 대한 메타데이터(알고리즘 및 토큰 타입)를 담고 있습니다.
#### JSON 예시

```JSON
{
  "alg": "HS256",
  "typ": "JWT"
}
```

- **`alg` (Algorithm):** 서명(Signature)을 해시/암호화할 때 어떤 알고리즘을 사용했는지 지정합니다. (예: `HS256` = HMAC SHA-256, `RS256` = RSA SHA-256)
    
- **`typ` (Type):** 토큰의 타입을 지정합니다. (항상 `JWT`)
### 2. Payload (페이로드)
페이로드는 "토큰에 담을 실제 사용자 정보 및 데이터"를 담는 곳입니다. 이 안에 들어가는 데이터의 조각 하나하나를 클레임(Claim)이라고 부릅니다.

```JSON
{
  "sub": "1234567890",
  "name": "홍길동",
  "role": "USER",
  "iat": 1722300000,
  "exp": 1722303600
}
```

클레임은 크게 3가지 종류로 나뉩니다.

1. **등록된 클레임 (Registered Claims):** 토큰 표준에서 권장하는 정의된 키값입니다.
    - `sub` (Subject): 토큰 식별자 (보통 유저 PK/ID)
    - `iss` (Issuer): 토큰 발급자 (예: `[https://auth.example.com](https://auth.example.com)`)
    - `iat` (Issued At): 토큰 발급 시간 (Unix Timestamp)
    - `exp` (Expiration Time): **토큰 만료 시간** (Unix Timestamp)
        
2. **공개 클레임 (Public Claims):** 충돌 방지를 위해 URI 형태로 만드는 키값입니다.
    
3. **비공개/사용자 정의 클레임 (Private Claims):** 개발자가 서비스에 필요해서 임의로 정의한 키값입니다. (예: `role`, `email`, `name`)

**주의**
- Header와 Payload는 **암호화된 것이 아니라 단순히 `Base64URL`로 인코딩만 되어 있는 텍스트**이므로 민감정보가 포함되면 안됨

### 3. Signature (서명)
서명은 "이 토큰이 발급된 후 중간에서 위·변조되지 않았음을 증명"하는 데이터입니다.
JWT의 핵심 안전장치로, 서버가 가진 비밀키(Secret Key)를 사용해 생성됩니다.
#### 서명 생성 공식
```
HMACSHA256(
  base64UrlEncode(Header) + "." +
  base64UrlEncode(Payload),
  secret_key
)
```
1. Header를 Base64URL로 인코딩합니다.
2. Payload를 Base64URL로 인코딩합니다.
3. 두 값을 점(`.`)으로 연결합니다.
4. 서버만 알고 있는 비밀키(`secret_key`)를 섞어서 선택된 알고리즘(예: HS256)으로 해싱합니다.

### 4. 서버가 JWT를 검증하는 과정

클라이언트가 HTTP 요청 헤더에 `Authorization: Bearer <TOKEN>`을 실어 보냈을 때 서버의 동작 방식입니다.
1. 전달받은 JWT에서 **Header**와 **Payload**를 분리합니다.
2. 서버가 가지고 있는 **비밀키**를 사용해, 전달받은 Header와 Payload로 **새로운 서명값을 직접 다시 계산**해 봅니다.
3. 서버가 방금 직접 만든 서명과 토큰에 적혀 온 Signature를 비교합니다.
    - **같다면:** "중간에 데이터가 수정되지 않은 진짜 토큰이 맞구나!" 하고 인정합니다.
    - **다르다면:** 누군가 Payload 내용을 조작했거나 다른 키로 만든 가짜 토큰이므로 `401 Unauthorized`로 즉시 거부합니다.
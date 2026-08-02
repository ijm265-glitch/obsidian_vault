### 1. HTTP Basic Auth의 동작 원리

보안이 적용된 엔드포인트에 접속할 때 브라우저와 ESP32는 다음과 같이 통신합니다.

1. **클라이언트(브라우저) 요청:** 인증 정보 없이 접속 시도
    
2. **서버(ESP32) 응답 (401 거부):** `WWW-Authenticate: Basic realm="Restricted"` 헤더와 함께 **401 Unauthorized** 에러 반환
    
3. **클라이언트 행동:** 브라우저가 사용자에게 팝업 창을 띄워 ID와 Password를 입력받음
    
4. **클라이언트 재요청:** 입력받은 `ID:Password`를 Base64로 인코딩하여 `Authorization: Basic <Base64문자열>` 헤더를 달아서 다시 전송
    
5. **서버 승인 (200 OK):** 헤더의 인코딩 값을 검증하고 일치하면 영상이나 페이지를 전송


### 2. C++ 인증 검증 함수 작성

기존 `app_httpd.cpp` 파일의 상단(전역 변수들이 있는 곳 아래)에 모든 핸들러에서 공통으로 사용할 인증 검증 함수(Helper Function)를 하나 모듈화하여 작성합니다.
```cpp
// Base64 인코딩된 인증 정보 (예: admin / 1234)
// 형식: "Basic <Base64(ID:Password)>"
const char* HTTP_AUTH_INFO = "Basic YWRtaW46MTIzNA==";

// 인증 확인 헬퍼 함수
static esp_err_t check_auth(httpd_req_t *req) {
    char auth_header[100];
    
    // 1. HTTP 요청에서 "Authorization" 헤더 값을 추출
    if (httpd_req_get_hdr_value_str(req, "Authorization", auth_header, sizeof(auth_header)) == ESP_OK) {
        // 2. 수신된 헤더 값과 미리 지정된 인증 정보 비교
        if (strcmp(auth_header, HTTP_AUTH_INFO) == 0) {
            return ESP_OK; // 인증 성공
        }
    }

    // 3. 인증 실패 또는 헤더가 없는 경우 401 응답 전송
    httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"ESP32 Secure Camera\"");
    httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "401 Unauthorized: Access Denied");
    return ESP_FAIL;
}
```

### 3. 스트리밍 및 주요 엔드포인트에 인증 로직 적용

**영상 스트리밍 보호 적용 (`stream_handler`)**
```cpp
static esp_err_t stream_handler(httpd_req_t *req) {
    // ---- [보안 로직 추가] ----
    if (check_auth(req) != ESP_OK) {
        log_e("Stream access denied: Unauthorized");
        return ESP_FAIL; // 인증 실패 시 즉시 연결 차단
    }
    // --------------------------

    camera_fb_t *fb = NULL;
    struct timeval _timestamp;
    esp_err_t res = ESP_OK;
    
    // ... (이후 기존의 스트리밍 코드 그대로 유지) ...
}
```

`index_hendler`나 `cmd_handler`에도 동일하게 보호 로직을 적용할 것 
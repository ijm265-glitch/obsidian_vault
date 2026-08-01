SP32S3-CAM
[[DCHP - esp32가 공유기로부터 ip를 할당받는 과정|DHCP]]
Arduino IDE의 CameraWebServer에 ssid와 password를 입력하여 Wi-Fi Station 모드로 작동하게 함
공유기로부터 DHCP를 통해 사설 ip를 전달받음 
그 사설 ip의 80번 포트에 카메라 비디오 스트리밍 서버를 띄움

이 경우 DHCP로 사설 ip를 할당 받기 때문에 주변 환경에 따라 ip가 변경될 우려가 있음

이에 따라 공유기 관리자 페이지에 접속
- Windows환경에서는 command prompt를 실행해 ipconfig를 입력하여 현재 ESP32가 Station으로 접속해있는 무선랜의 default gateway를 확인. 브라우저에 이를 입력하여 공유기 관리자 페이지에 접속한다.

Advanced Setup - Network - LAN Setup에서 Station List를 보면 현재 Station mode로 작동중인 esp32의 MAC Addreess를 확인할 수 있음

이를 복사하여 Advanced Setup - Network - DHCP Server Setup으로 들어가 Reserved Addresses부분에 우측 상단 추가 버튼은 눌러 esp32에 할당할 private ip와 mac address를 입력하여 esp32가 private ip를 요청할 때 마다 고정된 ip를 할당하도록 한다.
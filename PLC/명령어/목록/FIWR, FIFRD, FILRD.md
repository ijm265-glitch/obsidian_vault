FIWR D0 D1000
> File Write
>d0의 값을 d1000을 data table로 만들어서 data table에 쓰기작업을 수행한다
> d1000 디바이스의 값은 data table의 요소 숫자이며 d1001 디바이스에 쓰기 작업을 수행한다.
> 이미 d1001 디바이스에 값이 존재한다면 d1002 디바이스에 쓰기 작업을 수행한다.

FIFRD D1000 D1
> File First ReaD
> d1000 data table의 첫번째 요소인 d1001의 값을 d1으로 복사한다.
> data table을 stack 구조로 활용하는 방법으로 이 경우 상위 디바이스에 존재하는 값들이 디바이스 주소를 하나 낮추어 이동한다

FILRD D1000 D
> File Last ReaD
> d1000 data table의 마지막 요소값을 d1으로 복사한다.
> data table을 queue 구조로 활용하는 방법
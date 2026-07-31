### 1. 삼각함수 형태의 푸리에 급수 (Real-valued Form)

우리가 직관적으로 이해할 수 있는 코사인($\cos$)과 사인($\sin$) 파동의 합입니다.

$$f(t) = a_0 + \sum_{n=1}^{\infty} \left( a_n \cos(n\omega_0 t) + b_n \sin(n\omega_0 t) \right)$$

- $a_0$: 신호의 **직류(DC) 성분** (전체 평균값)
    
- $a_n$: $n$번째 **코사인 파동의 크기**
    
- $b_n$: $n$번째 **사인 파동의 크기**
    

#### 계수 $a_n, b_n$을 구하는 방법 (직교성 활용)

앞서 배운 **직교성** 덕분에, $a_n$을 구하고 싶다면 양변에 $\cos(n\omega_0 t)$를 곱해서 한 주기($T$) 동안 적분하면 됩니다. 다른 주파수 및 사인 성분들은 모조리 $0$으로 날아가고 $a_n$만 남습니다.

$$a_n = \frac{2}{T} \int_{0}^{T} f(t) \cos(n\omega_0 t) \, dt$$

$$b_n = \frac{2}{T} \int_{0}^{T} f(t) \sin(n\omega_0 t) \, dt$$
---
### 2. 가교 역할: 오일러 공식 (Euler's Formula)

삼각함수 형태는 직관적이지만, $\cos$과 $\sin$ 두 갈래로 나뉘어 있어 **수학적으로 다루기가 매우 번거롭고 연산이 복잡**합니다. 이를 하나로 통합해 주는 열쇠가 바로 **오일러 공식**입니다.

$$e^{j\theta} = \cos\theta + j\sin\theta$$

이 식으로부터 $\cos$과 $\sin$을 복소 지수 함수로 재정의할 수 있습니다.

$$\cos\theta = \frac{e^{j\theta} + e^{-j\theta}}{2}, \quad \sin\theta = \frac{e^{j\theta} - e^{-j\theta}}{2j}$$

즉, 실수인 $\cos$과 $\sin$ 파동은 **시계 방향과 반시계 방향으로 회전하는 두 개의 복소 회전 벡터($e^{j\theta}$와 $e^{-j\theta}$)의 조합**으로 완벽하게 대체될 수 있습니다. ($\theta=\omega t$)

> $e^{j\theta}$: CCW (정회전)
> $e^{-j\theta}$: CW (역회전)
> $e^{j\theta} + e^{-j\theta} = (\cos\theta + j\sin\theta) + (\cos\theta - j\sin\theta)=2cos(\theta)$
> $e^{j\theta} - e^{-j\theta} = (\cos\theta + j\sin\theta) - (\cos\theta - j\sin\theta)=2j\cdot sin(\theta)$
> 이 때문에 푸리에 변환이나 푸리에 급수를 복소수로 다룰 때 음의 주파수의 개념이 나타남 
> 현실의 신호 (실수 신호)를 만들기 위해서는 반시계로 회전하는 $+\omega$ 주파수 성분과 시계방향으로 회전하는 $-\omega$ 주파수 성분이 짝을 이루어 존재해야만 하기 때문 
> 
---

### 3. 복소 푸리에 급수 (Complex Exponential Form)

오일러 공식을 대입하여 정리하면, 삼각함수의 덧셈식이 단 하나의 매끄러운 **복소 지수 함수 합**으로 표현됩니다.

$$f(t) = \sum_{n=-\infty}^{\infty} c_n e^{j n \omega_0 t}$$

이 식이 바로 **디지털 신호처리와 푸리에 변환(FT/DFT)에서 표준으로 사용하는 복소 푸리에 급수**입니다.

- 범위가 $1 \sim \infty$에서 $-\infty \sim \infty$ (음의 주파수 포함)로 확장됩니다.
    
- 삼각함수의 $a_n, b_n$ 두 개로 나뉘어 있던 계수가 하나의 복소수 계수 $c_n$으로 [[삼각함수의 계수를 복소수 계수로 합치기|통합]]됩니다.
    

#### 복소 계수 $c_n$의 도출 (기저 $e^{j n \omega_0 t}$와의 내적)

양변에 $e^{j n \omega_0 t}$의 직교 기저인 $e^{-j n \omega_0 t}$를 곱하고 한 주기 동안 적분(내적)하면, 오직 $n$번째 성분만 남고 모두 $0$이 됩니다.

$$c_n = \frac{1}{T} \int_{0}^{T} f(t) e^{-j n \omega_0 t} \, dt$$
---
### 4. 복소 계수 $c_n$이 담고 있는 정보

$c_n$은 복소수(Complex Number)입니다. 이 복소수 하나가 오토튠과 신호처리에서 그토록 중요하게 다루던 크기(Magnitude)와 **위상(Phase)** 정보를 모두 지니고 있습니다.

$$c_n = \vert{}c_n\vert{} e^{j \phi_n}$$

1. **크기 (Magnitude, $\vert{}c_n\vert{}$):**
    
    $n\omega_0$ 주파수 성분이 신호 안에 **얼마나 강하게(배음의 세기)** 들어있는가?
    
    $$\vert{}c_n\vert{} = \sqrt{\text{Re}(c_n)^2 + \text{Im}(c_n)^2}$$
    
2. **위상 (Phase, $\phi_n$):**
    
    $n\omega_0$ 주파수 성분이 시간축상에서 **어디서부터 시작하는가(파동의 타이밍)**?
    
    $$\phi_n = \text{atan2}(\text{Im}(c_n), \text{Re}(c_n))$$
    
    이 파동이 $t=0$ 일 때 **어느 각도(위치)에서 출발하는지**를 뜻합니다. 예를 들어 $\phi_n = 0$이면 코사인처럼 산(+)의 최고점에서 출발하고, $\phi_n = -90^\circ$이면 원점에서 위로 올라가는 파동으로 시작합니다.
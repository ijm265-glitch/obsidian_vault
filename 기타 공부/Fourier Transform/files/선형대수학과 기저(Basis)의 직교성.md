### 1. 내적(Inner Product)이란 무엇인가?

내적은 두 벡터를 입력받아 하나의 스칼라(숫자)를 반환하는 연산입니다.

#### (1) 유한 차원 복소 벡터 공간에서의 내적

디지털 신호 $x = [x[0], x[1], \dots, x[N-1]]^T$ 와 같이 길이가 $N$인 복소수 벡터 공간 $\mathbb{C}^N$에서 두 벡터 $\mathbf{u}, \mathbf{v}$의 내적은 다음과 같이 정의됩니다.

$$\langle \mathbf{u}, \mathbf{v} \rangle = \mathbf{v}^H \mathbf{u} = \sum_{i=0}^{N-1} u[i] \cdot \overline{v[i]}$$

_(단, $\overline{v[i]}$는 복소켤레(Complex Conjugate)이며, $H$는 켤레 전치(Hermitian Transpose)를 의미합니다.)

> 복소수 벡터는 $V=[2+i, -3i, 7+i]$ 등의 형태를 가진 요소가 복소수를 포함하는 벡터
> 켤레복소수 (Conjugate) 는 복소평면에서 x축 대칭의 형태를 하고 있음 
> 자기 자신과의 내적은 에너지(길이)를 얻도록 정의되었음 즉 위상은 상쇄되고 순수한 실수의 값임
> 서로 다른 벡터의 내적은 위상이 상쇄되지 않음. 대신 두 신호의 위상차가 얻어짐

#### (2) 연속 함수 공간에서의 내적 (무한 차원)

시간 $t \in [a, b]$ 구간에서 정의된 두 복소수 함수 $f(t), g(t)$가 있을 때, 덧셈과 실수/복소수 배가 가능하므로 함수들 역시 하나의 벡터 공간을 이룹니다. 이 공간에서의 내적은 이산 합($\sum$)을 연속 적분($\int$)으로 확장한 형태입니다.

$$\langle f, g \rangle = \int_{a}^{b} f(t) \cdot \overline{g(t)} \, dt$$

> 복소수 함수는 입력이 무엇이던간에 출력으로 복소수를 내놓는 함수
> $f(1+i)=1+2i$ 
> $f(t)=e^{it}$

#### (3) 내적의 직관적 의미: "닮음의 정도 (Similarity / Projection)"

신호처리 관점에서 내적은 "한 신호 안에 다른 신호의 성분이 얼마나 포함되어 있는가?"를 측정하는 정류/투영(Projection) 연산입니다.

- 내적값이 크다 $\to$ 두 신호가 비슷한 패턴을 가짐 (상관관계가 높음)
    
- 내적값이 0이다 $\to$ 두 신호는 서로 완전히 독립적이며 아무런 연관이 없음

---
### 2. 직교성(Orthogonality)의 정의와 성질

두 벡터(또는 함수)의 내적값이 $0$일 때, 두 대상은 **서로 직교(Orthogonal)한다**고 정의합니다.

$$\langle \mathbf{u}, \mathbf{v} \rangle = 0 \iff \mathbf{u} \perp \mathbf{v}$$

#### 직교 기저(Orthogonal Basis)의 위력

$N$차원 공간에서 서로 직교하는 $N$개의 기저 벡터 집합 ${\mathbf{e}_0, \mathbf{e}_1, \dots, \mathbf{e}_{N-1}}$이 존재한다고 해봅시다.

어떤 임의의 복소 신호 $\mathbf{x}$든 이 [[기저들의 선형 결합]](Linear Combination)으로 표현할 수 있습니다.

$$\mathbf{x} = c_0 \mathbf{e}_0 + c_1 \mathbf{e}_1 + \dots + c_{N-1} \mathbf{e}_{N-1} = \sum_{k=0}^{N-1} c_k \mathbf{e}_k$$

이때 계수 $c_k$를 구하는 방법이 파격적으로 쉬워집니다. 연립방정식을 풀 필요 없이, **양변에 내가 구하고 싶은 기저 $\mathbf{e}_k$를 내적**해주기만 하면 됩니다.

$$\langle \mathbf{x}, \mathbf{e}_k \rangle = \left\langle \sum_{m=0}^{N-1} c_m \mathbf{e}_m, \; \mathbf{e}_k \right\rangle = c_k \langle \mathbf{e}_k, \mathbf{e}_k \rangle \quad (\because m \neq k \text{일 때 } \langle \mathbf{e}_m, \mathbf{e}_k \rangle = 0)$$

$$\therefore c_k = \frac{\langle \mathbf{x}, \mathbf{e}_k \rangle}{\Vert{}\mathbf{e}_k\Vert{}^2}$$

만약 기저 벡터의 크기 $\Vert{}\mathbf{e}_k\Vert{}$까지 $1$로 맞춘 정규 직교 기저(Orthonormal Basis)라면, 계수는 단순히 $c_k = \langle \mathbf{x}, \mathbf{e}_k \rangle$ (신호와 기저의 내적)이 됩니다.

---

### 3. 푸리에 해석으로의 연결: 복소 지수 함수의 직교성

푸리에 변환의 핵심 아이디어는 "세상의 모든 신호를 정현파(복소 지수 함수 $e^{j\omega t}$)라는 [[기저 벡터들의 합으로 분해]]하자"입니다. 이것이 가능한 이유는 복소 지수 함수들이 **서로 직교하기 때문**입니다.

주기 $T$를 가지는 기본 주파수 $\omega_0 = \frac{2\pi}{T}$에 대해, $k$번째 Harmonic를 $\phi_k(t) = e^{j k \omega_0 t}$라 할 때, 한 주기 $[0, T]$ 동안 두 함수 $\phi_k(t)$와 $\phi_m(t)$의 내적을 구해봅시다.

$$\langle \phi_k, \phi_m \rangle = \int_{0}^{T} e^{j k \omega_0 t} \cdot \overline{e^{j m \omega_0 t}} \, dt = \int_{0}^{T} e^{j (k - m) \omega_0 t} \, dt$$

- **$k = m$ 일 때 (자기 자신과의 내적):**
    
    $$\int_{0}^{T} e^{j \cdot 0} \, dt = \int_{0}^{T} 1 \, dt = T$$
    
- **$k \neq m$ 일 때 (서로 다른 주파수 간의 내적):**
    
    $$\int_{0}^{T} e^{j (k - m) \omega_0 t} \, dt = \left[ \frac{e^{j (k - m) \omega_0 t}}{j (k - m) \omega_0} \right]_{0}^{T} = \frac{e^{j (k-m) 2\pi} - 1}{j (k-m) \omega_0} = \frac{1 - 1}{j (k-m) \omega_0} = 0$$
    

$$\therefore \langle \phi_k, \phi_m \rangle = T \cdot \delta_{km} \quad (\delta_{km} \text{은 크로네커 델타})$$
$$\delta_{ij} = \begin{cases} 1 & \text{if } i = j \\ 0 & \text{if } i \neq j \end{cases}$$

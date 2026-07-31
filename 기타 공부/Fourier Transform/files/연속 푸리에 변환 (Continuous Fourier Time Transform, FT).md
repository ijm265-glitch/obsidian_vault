### 1. 비주기 신호로의 확장 ($T \to \infty$)

비주기 신호 $f(t)$는 "주기 $T$가 무한히 길어서 단 한 번만 나타나고 다시는 반복되지 않는 신호"로 바라볼 수 있습니다.

푸리에 급수에서 주기가 무한대($T \to \infty$)로 늘어나면 두 가지 결정적인 변화가 일어납니다.

1. **주파수 간격의 조밀화 ($\Delta f \to 0$):**
    
    기본 주파수 $\omega_0 = \frac{2\pi}{T}$ 이므로, $T \to \infty$가 되면 이웃한 조화 성분 간의 주파수 간격 $\Delta \omega$가 무한히 좁아집니다.
    
    즉, $1\omega_0, 2\omega_0, 3\omega_0$ 처럼 이산적(Discrete)으로 뚝뚝 떨어져 있던 주파수 성분들이 연속적인(Continuous) 주파수 축 $\omega$로 변합니다.
    
2. **합($\sum$)이 적분($\int$)으로 변환:**
    
    무한히 촘촘해진 이산 주파수들의 합(복소 푸리에 급수 $\sum$)은 자연스럽게 연속 영역에서의 적분($\int$)으로 바뀝니다. -> 역 푸리에 변환
    

#### 연속 푸리에 변환 쌍 (Continuous Fourier Transform Pair)

이 극한 과정을 거치면 비주기 신호 $f(t)$와 주파수 스펙트럼 $F(\omega)$ 사이의 관계식이 탄생합니다.

- **[[순방향 푸리에 변환]] (Forward FT: 시간 $\to$ 주파수):**
    
    $$F(\omega) = \mathcal{F}\{f(t)\} = \int_{-\infty}^{\infty} f(t) e^{-j\omega t} \, dt$$
    
    _(시간 영역 신호 $f(t)$에 연속적인 주파수 기저 $e^{-j\omega t}$를 내적하여, 주파수 $\omega$ 성분의 세기 $F(\omega)$를 추출)_
    
- **역 푸리에 변환 (Inverse FT: 주파수 $\to$ 시간):**
    
    $$f(t) = \mathcal{F}^{-1}\{F(\omega)\} = \frac{1}{2\pi} \int_{-\infty}^{\infty} F(\omega) e^{j\omega t} \, d\omega$$
    
    _(연속적인 주파수 성분 $F(\omega)$들에 회전 파동 $e^{j\omega t}$를 곱해 연속적으로 다시 합쳐서 시간 신호 $f(t)$를 복원)_

---

### 2. 시간 영역과 주파수 영역 간의 쌍대성 (Duality)

푸리에 변환식과 역변환식을 잘 비교해 보면, 부호($-j$와 $+j$) 및 상수 factor($1/2\pi$)를 제외하고는 수학적 형태가 완벽하게 대칭(Symmetric)을 이룹니다. 이를 쌍대성(Duality)이라고 합니다.

$$\text{만약 } f(t) \stackrel{\mathcal{F}}{\longleftrightarrow} F(\omega) \quad \text{이면,} \quad F(t) \stackrel{\mathcal{F}}{\longleftrightarrow} 2\pi f(-\omega)$$

> **💡 직관적 의미:**
> 
> **"시간 영역에서의 특정한 성질이나 파형은, 주파수 영역에서도 정확히 같은 형태의 반대 성질로 나타난다."**

#### 대표적인 쌍대성 예시

1. **시간축 좁힘 vs 주파수축 넓힘 (Scaling Property):**
    
    오디오 신호를 2배 빠르게 재생(시간축 압축: $f(2t)$)하면, 주파수 스펙트럼은 고음역대로 2배 퍼집니다($F(\omega/2)$).
    
2. **사각파(Rectangular pulse) vs 싱크 함수(Sinc function):**
    
    시간 영역에서 딱 끊기는 사각 파동 $rect(t)$를 푸리에 변환하면 주파수 영역에서는 $sinc(\omega)$ 형태가 됩니다. 반대로 시간 영역의 싱크 함수를 푸리에 변환하면 주파수 영역에서 완벽한 사각 필터(Ideal Low-pass Filter)가 됩니다.
    
3. **불확정성 원리 (Uncertainty Principle):**
    
    신호가 시간 영역에서 매우 찰나의 순간에 국한되면(예: 임펄스 신호 $\delta(t)$), 주파수 영역에서는 모든 주파수에 걸쳐 평평하게 넓게 퍼집니다($F(\omega) = 1$). **시간과 주파수 해상도를 동시에 완벽하게 좁히는 것은 불가능합니다.**
    
---
### 3. 파세발의 정리 (Parseval's Theorem)

파세발 정리는 "신호를 시간 영역에서 관찰하여 모두 더하든(에너지 계산), 주파수 영역으로 변환하여 모두 더하든 신호가 가진 전체 에너지의 총량은 완벽하게 같다"는 신호처리의 에너지 보존 법칙입니다.

$$\text{Total Energy} = \int_{-\infty}^{\infty} \vert{}f(t)\vert{}^2 \, dt = \frac{1}{2\pi} \int_{-\infty}^{\infty} \vert{}F(\omega)\vert{}^2 \, d\omega$$

_(단, 주파수 단위로 $f$($\text{Hz}$)를 사용할 경우 $\omega = 2\pi f$, $d\omega = 2\pi df$ 이므로 상수가 사라져 $\int_{-\infty}^{\infty} \vert{}f(t)\vert{}^2 dt = \int_{-\infty}^{\infty} \vert{}F(f)\vert{}^2 df$ 가 됩니다.)_

#### 파세발 정리가 중요한 이유

- **$\vert{}F(\omega)\vert{}^2$ 의 의미 (에너지 스펙트럼 밀도, Energy Spectral Density):**
    
    $\vert{}F(\omega)\vert{}^2$은 복소수 $F(\omega)$의 크기 제곱입니다. 이 값은 "주파수 $\omega$ 근처에 전체 에너지 중 얼마만큼의 에너지가 몰려 있는가?"를 보여줍니다.
    
- 이 정리 덕분에 우리는 오디오나 진동 데이터의 시간 파형을 일일이 제곱해서 적분하지 않고도, **FFT 스펙트럼의 크기 제곱만 합쳐서 신호의 전체 전력/에너지를 손쉽게 측정**할 수 있습니다.
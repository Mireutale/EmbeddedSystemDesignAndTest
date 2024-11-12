//RCC reset & clock control
#define RCC_APB2ENR (*(volatile unsigned int *)0x40021018)

//GPIOx_CRL -> 하위 핀 (0 - 7)
//GPIOx_CRH -> 상위 핀 (8 - 15)
//GPIOx_IDR -> GPIO 포트의 핀에 입력된 상태를 읽는 레지스터
//GPIOx_BSRR -> 각 핀을 설정하거나 리셋
#define GPIOA_CRL (*(volatile unsigned int *)0x40010800)
#define GPIOA_IDR (*(volatile unsigned int *)0x40010808)

#define GPIOB_CRH (*(volatile unsigned int *)0x40010C04)
#define GPIOB_IDR (*(volatile unsigned int *)0x40010C08)

#define GPIOC_CRL (*(volatile unsigned int *)0x40011000)
#define GPIOC_CRH (*(volatile unsigned int *)0x40011004)
#define GPIOC_IDR (*(volatile unsigned int *)0x40011008)
#define GPIOC_BSRR (*(volatile unsigned int *)0x40011010)

void delay() {
  int i;
  for(i = 0; i < 10000000; ++i) {}
}

int main(void) {
  // port A, B, C에 clock 인가 -> set 1
  RCC_APB2ENR |= 0x1C; // 0x04 + 0x08 + 0x10
  
  // pin A0, B10, C4, C13 input(1000) mode 설정
  // pin C8, C9 output(0011) mode 설정
  
  // A0, key4
  // A0 -> 0번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOA_CRL &= 0xFFFFFFF0;
  GPIOA_CRL |= 0x00000008;
  
  // B10, key2
  // B10 -> 10번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOB_CRH &= 0xFFFFF0FF;
  GPIOB_CRH |= 0x00000800;
  
  // C4, key1
  // C4 -> 4번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOC_CRL &= 0xFFF0FFFF;
  GPIOC_CRL |= 0x00080000;
  
  // C8, C9, C13(key3)
  // C8 -> 정방향 회전
  // C9 -> 역방향 회
  // c13 -> 13번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOC_CRH &= 0xFF0FFF00;
  GPIOC_CRH |= 0x00800033;
  //GPIOC_BSRR |= 0x03000000;
  
  while (1) {
    // key1(PC4) push -> 모터 정방향 회전
    if (!(GPIOC_IDR & 0x10)) {
      GPIOC_BSRR |= 0x0100; // C8 set
      GPIOC_BSRR |= 0x02000000; // C9 reset
    }
    
    // key2(PB10) push -> 모터 역방향 회전
    if (!(GPIOB_IDR & 0x0400)) {
      GPIOC_BSRR |= 0x01000000; // C8 reset
      GPIOC_BSRR |= 0x0200; // C9 set
    }
    
    // key3(PC13) push -> 정방향 2초(딜레이) 역방향 2초(딜레이) 회전 후 정지
    
    if (!(GPIOC_IDR & 0x2000)) {
      GPIOC_BSRR |= 0x01000000; // C8 reset
      GPIOC_BSRR |= 0x02000000; // C9 reset
      
      GPIOC_BSRR |= 0x0100; // C8 set
      GPIOC_BSRR |= 0x02000000; // C9 reset
      delay();
      GPIOC_BSRR |= 0x01000000; // C8 reset
      GPIOC_BSRR |= 0x0200; // C9 set
      delay();
      GPIOC_BSRR |= 0x01000000; // C8 reset
      GPIOC_BSRR |= 0x02000000; // C9 reset
    }
    
    // key4(PA0) push -> 모터 정지
    if (!(GPIOA_IDR & 0x01)) {
      GPIOC_BSRR |= 0x01000000; // C8 reset
      GPIOC_BSRR |= 0x02000000; // C9 reset
    }
  }

  return 0;
}

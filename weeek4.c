//RCC reset & clock control
#define RCC_APB2ENR (*(volatile unsigned int *)0x40021018)

//GPIOx_CRL -> 하위 핀 (0 - 7)
//GPIOx_CRH -> 상위 핀 (8 - 15)
//GPIOx_IDR -> GPIO 포트의 핀에 입력된 상태를 읽는 레지스터
//GPIOx_BSRR -> 각 핀을 설정하거나 리셋
// A, B, C까지는 key -> IDR로 값 인식
#define GPIOA_CRL (*(volatile unsigned int *)0x40010800)
#define GPIOA_IDR (*(volatile unsigned int *)0x40010808)

#define GPIOB_CRH (*(volatile unsigned int *)0x40010C04)
#define GPIOB_IDR (*(volatile unsigned int *)0x40010C08)

#define GPIOC_CRL (*(volatile unsigned int *)0x40011000)
#define GPIOC_CRH (*(volatile unsigned int *)0x40011004)
#define GPIOC_IDR (*(volatile unsigned int *)0x40011008)

//D포트에 LED를 BSRR로 설정
#define GPIOD_CRL (*(volatile unsigned int *)0x40011400)
#define GPIOD_BSRR (*(volatile unsigned int *)0x40011410)

int main(void) {
  // port A, B, C, D에 clock 인가 -> set 1
  // 2번, 3번, 4번, 5번
  // 0x04 + 0x08 + 0x10 + 0x20
  RCC_APB2ENR |= 0x3C;
  // key 4, 2, 1, 3
  // pin A0, B10, C4, C13 input(1000) 설정
  // LED 1, 2, 3, 4
  // pin D2, D3, D4, D7 output(0001) mode 설정

  // A0 , key4 (0-7)
  // A0 -> 0번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOA_CRL &= 0xFFFFFFF0;
  GPIOA_CRL |= 0x00000008;

  // B10, key2 (8-15)
  // B10 -> 10번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOB_CRH &= 0xFFFFF0FF;
  GPIOB_CRH |= 0x00000800;

  // C4, key1
  // C4 -> 4번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOC_CRL &= 0xFFF0FFFF;
  GPIOC_CRL |= 0x00080000;

  // C13, key3
  // c13 -> 13번째 비트를 1000으로 수정 -> push-pull + input 모드로 변경
  GPIOC_CRH &= 0xFF0FFFFF;
  GPIOC_CRH |= 0x00800000;

  // D2, D3, D4, D7
  // 2, 3, 4,7비트를 0001로 수정 -> analog + output 모드로 변경
  GPIOD_CRL &= 0x0FF000FF;
  GPIOD_CRL |= 0x10011100;
  
  // PD2, PD3, PD4, PD7
  // LED1, 2, 3, 4 off -> port set bits
  GPIOD_BSRR |= 0x9C; // 0x80 + 0x10 + 0x08 + 0x04

  while (1) {
    // C4, key1(PC4) push -> LED1, 3 on
    if (!(GPIOC_IDR & 0x10)) {
      //BR2, BR4 -> 1로 변경
      //LED1(PD2), LED3(PD4)을 ON
      GPIOD_BSRR |= 0x140000; 
      //18번, 20번 비트 1 -> 2번, 4번 BR값 1로 변경 -> 위에서 set(꺼짐)으로 변경한 ODRx bit를 reset
      // 0x100000 + 0x040000 == 0000 0000 0001 0100 0000 0000 0000 0000
    }
    // B10, key2(PB10) push -> LED 1, 3 off
    if (!(GPIOB_IDR & 0x0400)) {
      //BS2, BS4 -> 1로 변경
      //LED1(PD2), LED3(PD4)을 OFF
      GPIOD_BSRR |= 0x14; 
      //2번, 4번 비트 1 -> BS값 1로 변경 -> set(꺼짐)으로 설정
      // 0x10 + 0x04 = ... 0001 0100
    }
    // C13, key3(PC13) push -> LED 2, 4 on
    if (!(GPIOC_IDR & 0x2000)) {
      GPIOD_BSRR |= 0x880000; // 0x800000 + 0x080000
    }
    // A0, key4(PA0) push -> LED 2, 4 off
    if (!(GPIOA_IDR & 0x01)) {
      GPIOD_BSRR |= 0x88; // 0x80 + 0x08
    }
  }


  return 0;
}

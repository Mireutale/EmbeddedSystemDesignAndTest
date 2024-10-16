Software  
Use [IAR Imbedded Workbench for Arm](https://www.iar.com/ko/products/architectures/arm/iar-embedded-workbench-for-arm/iar-embedded-workbench-for-arm-free-trial-version)  
Hardware  
STM32 board GPIO control

---
환경 설정
프로젝트 생성 및 설정
1. 원하는 경로에 원하는 이름으로 폴더 생성 (ex: project_test)
>• 단, 한글경로는 포함되지 않도록 유의
>• project_test/ 폴더에 실험 제공 파일 (CoreSupport, libraries, user 등) 복사  

2. IAR Embedded Workbench IDE 실행
>•Project – Create New Project
>•Empty project – OK
>•생성한 project_test 폴더에 원하는 프로젝트 명 (ex: test) 으로 저장  

3. 프로젝트 오른쪽 클릭 – Add – Add Group으로
>project_test 폴더의 구조와 같이 만들기
>>• 필수는 아니지만 보기 좋게 하기 위함  
>>• 하나하나 추가해야 함  
>>• 프로젝트 오른쪽 클릭 – Add – Add

>Files으로 .c, .h, .s 파일 모두 추가  
>>• 이건 필수  

4. 프로젝트 오른쪽 클릭 - Options
>General Options – Target 탭 – Device
>>•ST – STM32F1 – STM32F107 – ST STM32F107VC

>General Options – Library Configuration 탭
>>• Library - Normal  
>>• CMSIS - Use CMSIS

>Runtime Checking – C/C++ Compiler
>>• List 탭  
>>• Output list file  
>>>• Assembler mnemonics  
>>>• Diagnostics  

>>• Preprocessor 탭  
>>>• Additional include directories: (one per line)
>>>>\$PROJ\_DIR\$\libraries\CMSIS\DeviceSupport
>>>>\$PROJ\_DIR\$\libraries\CMSIS\DeviceSupport  
>>>>\$PROJ\_DIR\$\libraries\CMSIS\DeviceSupport\Startup  
>>>>\$PROJ\_DIR\$\libraries\STM32F10x_StdPeriph_Driver_v3.5\inc  
>>>>\$PROJ\_DIR\$\libraries\STM32F10x_StdPeriph_Driver_v3.5\src  
>>>>\$PROJ\_DIR\$\user  
>>>>\$PROJ\_DIR\$\user\inc

>>>• Defined symbols: (one per line)  
>>>>USE_STDPERIPH_DRIVER  
>>>>STM32F10X_CL

>Runtime Checking – Output Converter
>>• Generate additional output  
>>>• Output format – Raw binary

>Runtime Checking – Linker – List 탭  
>>• 모두 체크

>user/main.c에 원하는 코드 작성
>>• F7 (Make) 로 컴파일  
>>• 첫 컴파일 시 저장하는 창이 뜸  
>>• project_test/ 폴더에 프로젝트 명으로 저장  

5. 보드에 포팅  
>options - Runtime checking – Debugger – Setup 탭
>>• Driver – J-link/J-Trace
>options - Runtime checking – Debugger – Download
>>• Verify download
>>• Use flash loader(s)
>Download and Debug
>>• 동의 묻는 창 뜨면 Accept
>>• 보드 flash에 프로그램 다운로드 하면서 디버깅 모드로 들어감
>Stop Debugging
>>• 디버깅 모드에서 나오기
>>• 보드 전원 껐다 켜기
>>• 원하는 대로 동작하는지 확인

# Object Pool
![image](https://user-images.githubusercontent.com/68372094/161510722-d34ba906-4840-4adf-b182-304e9a5b1bf6.png)
* VirtualAlloc 함수를 통해 메모리를 페이지 단위로 직접 예약/할당하여 사용하고 반납시 해제한다.
* BufferOver Flow 검출에 용이하다. new/delete 의 경우는 내부 로직이 wrapping 되어 직접 운영체제 Low Level에서 조작이 힘들다. 따라서 경우에 따라 delete한 메모리에 접근시 Crash가 나지 않는 경우가 있다. 

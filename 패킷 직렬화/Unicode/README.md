
# Unicode

* CP949, UTF8, UTF16은 인코딩 방식, 일종의 문자열 표현 알고리즘
* 유니코드 등은 문자열 셋
```cpp
char str[100] = "가"; //CP949 한글 2바이트, 로마 1바이트
char str2[100] = u8"가";//UTF8, 유니코드 문자열,한글 3바이트, 로마 1바이트
WCHAR str3[100] = L"가"; //UTF16 한글, 유니코드 문자열, 로마 모두 2바이트
```

![image](https://user-images.githubusercontent.com/68372094/158943756-dcc10550-db5c-4baf-8a39-5ef7abc0c7f1.png)
***
![image](https://user-images.githubusercontent.com/68372094/158944650-270b2be3-f718-4e6a-8151-3f4a4054386a.png)
![image](https://user-images.githubusercontent.com/68372094/163394210-208925f9-8c78-4ee2-9a91-add7e39ea1f7.png)
![image](https://user-images.githubusercontent.com/68372094/163394256-cdf63037-bfda-493c-aa50-631eae6a9fab.png)



## Cache
* RAM 보다 CPU에 가까우며 속도가 빠르다. 처리 속도(레지스트리>캐시>램>하드디스크)
* RAM이나 하드디스크에서 CPU에서 매번 처리할 데이터를 꺼내오는 것은 지연시간이 오래 걸린다.
* CPU가 RAM에서 데이터를 꺼내기 전 캐시를 확인한다.
* Tmeporal Locality =>한번 사용된 데이터는 재사용될 확률이 높다.
* Spatial Locality => 사용된 데이터의 주변 데이터는 사용될 확률이 높다.
	
* Spatial Locality의 예(Cache Friendly Code)
```cpp
INT32 buffer[10000][10000];
int main(int argc, char *argv[])
{

    memset(buffer, 0, sizeof(buffer));
	//열 단위로 접근
	{
		UINT64 start = GetTickCount();
		INT64 sum = 0;
		for(int i=0; i<10000; i++)
			for (int j = 0; j < 10000; j++)
			{
				sum += buffer[i][j];
			}
		UINT64 end = GetTickCount();
		cout << end - start << endl;
	}

	//행 단위로 접근
	{
		UINT64 start = GetTickCount();
		INT64 sum = 0;
		for (int i = 0; i<10000; i++)
			for (int j = 0; j < 10000; j++)
			{
				sum += buffer[j][i];
			}
		UINT64 end = GetTickCount();
		cout << end - start << endl;
	}

}
```
![image](https://user-images.githubusercontent.com/68372094/150241439-105f7d46-38d8-4fb2-9c4f-788bfc46fcc4.png)

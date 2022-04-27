# STLAllocator
* STL 컨테이너는 선언시에 Allocator을 커스터마이징 해줄 수 있다.   
* Allocator을 따로 설정하지 않을 경우 Default Allocator로 설정된다.   
* 커스터마이즈 Allocator을 설정하려면 allocate() 와 deallocate() value_Type을 정의한 클래스를 만든다.
* 컨테이너를 선언하면서 두번째 템플릿인자로 생성한 클래스를 넘긴다.

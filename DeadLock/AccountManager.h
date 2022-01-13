#pragma once
#include <mutex>

class Account
{

};

class AccountManager
{
public:
	static AccountManager *instance()
	{
		static AccountManager instance;
		return &instance;
	}

	Account *GetAccount(int32 id)
	{
		//아이디를 주면 뱉어준다.
		lock_guard<mutex> guard(_mutex);
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;


	AccountManager();
	~AccountManager();
};


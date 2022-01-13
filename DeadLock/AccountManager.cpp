#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	//accountLock
	lock_guard<mutex> guard(_mutex);

	//userlock
	User *user = UserManager::instance()->GetUser(100);
}

AccountManager::AccountManager()
{
}


AccountManager::~AccountManager()
{
}

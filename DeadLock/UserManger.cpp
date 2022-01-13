#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"
void UserManager::ProcessSave()
{
	//userlock
	lock_guard<mutex> guard(_mutex);
	//accountlock
	Account * account = AccountManager::instance()->GetAccount(100);
}

UserManager::UserManager()
{
}


UserManager::~UserManager()
{
}

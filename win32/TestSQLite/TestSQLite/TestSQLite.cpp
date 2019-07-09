// TestSQLite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "../../../SQLiteHelper.h"

int main()
{
	CSQLiteHelper helper;
	std::wstring s = L"hhhhh2.db";
	helper.open(s);

	std::wstring screateT1 = L"create table T4(id int not null primary key, t_name text not null, tt2 float not null,tt3 datetime not null)";
	//helper.exec(screateT1);
	wchar_t sss[MAX_PATH];
	for (int i = 0; i < 10; i++)
	{
		std::wstring insertvl = L"insert into T4 values(";
		wsprintf(sss, L"%d,\0", i+10);
		insertvl += sss;
		insertvl += L"\'ÖÐÎÄÊäÈë\',3.111,'2019-07-01 15:30:30')";
		helper.exec(insertvl);
	}
	//helper.tableExist(L"T1");
	helper.exec(L"select* from T4");

	int i = 0;
	system("pause");

	helper.close();
    return 0;
}


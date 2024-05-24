#pragma once
//заголовочный файл landlord.cpp Ц содержит объ€влени€
//классов и т.п.
#pragma warning (disable:4786) //дл€ множеств (только компил€торы microsoft)
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm> //дл€ sort()
#include <numeric> //дл€ accumulate()
#include "landlord.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

void getaLine(string& inStr); // получение строки текста
char getaChar(); // получение символа

class tenant
{
private:
public:
	string name; // им€ жильца
	int aptNumber; // номер комнаты жильца

	tenant(string n, int aNo);
	~tenant();
	int getAptNumber();
	// нужно дл€ использовани€ во множестве
	friend bool operator < (const tenant&, const tenant&);
	friend bool operator == (const tenant&, const tenant&);
	// дл€ операций ввода-вывода
	friend ostream& operator << (ostream&, const tenant&);
}; 

class compareTenants //функциональный объект дл€ сравнени€ имен жильцов
{
public:
	bool operator () (tenant*, tenant*) const;
};

class tenantList
{
private:
	// установить указатели на жильцов
	set<tenant*, compareTenants> setPtrsTens;	
	set<tenant*, compareTenants>::iterator iter;
public:
	~tenantList(); // деструктор
	// (удаление жильцов)
	void insertTenant(tenant*); // внесение жильца в список
	int getAptNo(string); // возвращает номер комнаты
	void display(); // вывод списка жильцов

	// «апись жильцов в XML файл
	void saveToXML(const std::string& filename);

	// —читывание жильцов из XML файла
	void loadFromXML(const std::string& filename);

	void deleteTenant(const std::string& name);

	void saveToXML(const std::string& filename, const std::string& elementName);

};

class tenantInputScreen
{
private:
	tenantList* ptrTenantList;
	string tName;
	int aptNo;
public:
	tenantInputScreen(tenantList* ptrTL) : ptrTenantList(ptrTL)
	{}
	void getTenant();
};

// одна строка таблицы прибыли: адрес и 12 мес€чных оплат
class rentRow
{
private:
public:
	int aptNo;
	float rent[12];
	rentRow(int); // конструктор с одним параметром
	void setRent(int, float); // запись платы за мес€ц
	float getSumOfRow(); // сумма платежей
	// из одной строки
	// нужно дл€ сохранени€ во множестве
	friend bool operator < (const rentRow&, const rentRow&);
	friend bool operator == (const rentRow&, const rentRow&);
	// дл€ вывода
	friend ostream& operator << (ostream&, const rentRow&);
};

class compareRows //функциональный объект сравнени€ объектов rentRows
{
public:
	bool operator () (rentRow*, rentRow*) const;
};

class rentRecord
{
private:
	// множество указателей на объекты rentRow (по одному на жильца)
	set<rentRow*, compareRows> setPtrsRR;
	set<rentRow*, compareRows>::iterator iter;
public:
	~rentRecord();
	void insertRent(int, int, float);
	void display();
	float getSumOfRents(); // сумма всех платежей

	void saveToXML(const std::string& filename);

	void loadFromXML(const std::string& filename);

	void deleteRent(int aptNo);
};

class rentInputScreen
{
private:
	tenantList* ptrTenantList;
	rentRecord* ptrRentRecord;
	string renterName;
	float rentPaid;
	int month;
	int aptNo;
public:
	rentInputScreen(tenantList* ptrTL, rentRecord* ptrRR) :
		ptrTenantList(ptrTL),
		ptrRentRecord(ptrRR)
	{}
	void getRent(); //арендна€ плата одного
	// жильца за один мес€ц
}; 

class expense
{
public:
	int month, day;
	string category, payee;
	float amount;
	expense() { }
	expense(int m, int d, string c, string p, float a) :
		month(m), day(d), category(c), payee(p), amount(a)
	{}
	// нужно дл€ хранени€ во множестве
	friend bool operator < (const expense&, const expense&);
	friend bool operator == (const expense&, const expense&);
	// нужно дл€ вывода
	friend ostream& operator << (ostream&, const expense&);
}; 

class compareDates //функциональный объект сравнени€ затрат
{
public:
	bool operator () (expense*, expense*) const;
};

class compareCategories //функциональный объект сравнени€ затрат
{
public:
	bool operator () (expense*, expense*) const;
};

class expenseRecord
{
private:
	//вектор указателей на расходы
	vector<expense*> vectPtrsExpenses;
	vector<expense*>::iterator iter;
public:
	~expenseRecord();
	void insertExp(expense*);
	void display();
	float displaySummary(); 

	void saveToXML(const std::string& filename);

	void loadFromXML(const std::string& filename);
};

class expenseInputScreen
{
private:
	expenseRecord* ptrExpenseRecord;
public:
	expenseInputScreen(expenseRecord*);
	void getExpense();
};

class annualReport
{
private:
	rentRecord* ptrRR;
	expenseRecord* ptrER;
	float expenses, rents;
public:
	annualReport(rentRecord*, expenseRecord*);
	void display();
};

class userInterface
{
private:
	tenantList* ptrTenantList;
	tenantInputScreen* ptrTenantInputScreen;
	rentRecord* ptrRentRecord;
	rentInputScreen* ptrRentInputScreen;
	expenseRecord* ptrExpenseRecord;
	expenseInputScreen* ptrExpenseInputScreen;
	annualReport* ptrAnnualReport;
	char ch;
public:
	userInterface();
	~userInterface();
	void interact();
};
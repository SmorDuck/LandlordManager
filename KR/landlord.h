#pragma once
//������������ ���� landlord.cpp � �������� ����������
//������� � �.�.
#pragma warning (disable:4786) //��� �������� (������ ����������� microsoft)
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm> //��� sort()
#include <numeric> //��� accumulate()
#include "landlord.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

void getaLine(string& inStr); // ��������� ������ ������
char getaChar(); // ��������� �������

class tenant
{
private:
public:
	string name; // ��� ������
	int aptNumber; // ����� ������� ������

	tenant(string n, int aNo);
	~tenant();
	int getAptNumber();
	// ����� ��� ������������� �� ���������
	friend bool operator < (const tenant&, const tenant&);
	friend bool operator == (const tenant&, const tenant&);
	// ��� �������� �����-������
	friend ostream& operator << (ostream&, const tenant&);
}; 

class compareTenants //�������������� ������ ��� ��������� ���� �������
{
public:
	bool operator () (tenant*, tenant*) const;
};

class tenantList
{
private:
	// ���������� ��������� �� �������
	set<tenant*, compareTenants> setPtrsTens;	
	set<tenant*, compareTenants>::iterator iter;
public:
	~tenantList(); // ����������
	// (�������� �������)
	void insertTenant(tenant*); // �������� ������ � ������
	int getAptNo(string); // ���������� ����� �������
	void display(); // ����� ������ �������

	// ������ ������� � XML ����
	void saveToXML(const std::string& filename);

	// ���������� ������� �� XML �����
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

// ���� ������ ������� �������: ����� � 12 �������� �����
class rentRow
{
private:
public:
	int aptNo;
	float rent[12];
	rentRow(int); // ����������� � ����� ����������
	void setRent(int, float); // ������ ����� �� �����
	float getSumOfRow(); // ����� ��������
	// �� ����� ������
	// ����� ��� ���������� �� ���������
	friend bool operator < (const rentRow&, const rentRow&);
	friend bool operator == (const rentRow&, const rentRow&);
	// ��� ������
	friend ostream& operator << (ostream&, const rentRow&);
};

class compareRows //�������������� ������ ��������� �������� rentRows
{
public:
	bool operator () (rentRow*, rentRow*) const;
};

class rentRecord
{
private:
	// ��������� ���������� �� ������� rentRow (�� ������ �� ������)
	set<rentRow*, compareRows> setPtrsRR;
	set<rentRow*, compareRows>::iterator iter;
public:
	~rentRecord();
	void insertRent(int, int, float);
	void display();
	float getSumOfRents(); // ����� ���� ��������

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
	void getRent(); //�������� ����� ������
	// ������ �� ���� �����
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
	// ����� ��� �������� �� ���������
	friend bool operator < (const expense&, const expense&);
	friend bool operator == (const expense&, const expense&);
	// ����� ��� ������
	friend ostream& operator << (ostream&, const expense&);
}; 

class compareDates //�������������� ������ ��������� ������
{
public:
	bool operator () (expense*, expense*) const;
};

class compareCategories //�������������� ������ ��������� ������
{
public:
	bool operator () (expense*, expense*) const;
};

class expenseRecord
{
private:
	//������ ���������� �� �������
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
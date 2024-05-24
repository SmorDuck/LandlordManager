//landlord.cpp
//моделирует финансы для жилого дома
#include "landlord.h" //для объявлений класса, и т.д.
#include "landlord.h"
#include "tinyxml2.h"
using namespace std;
void getaLine(string& inStr) // получение строки текста
{
	char temp[21];
	cin.get(temp, 20, '\n');
	cin.ignore(20, '\n');
	inStr = temp;
}

char getaChar() // получение символа
{
	char ch = cin.get();
	cin.ignore(80, '\n');
	return ch;
}

tenant::tenant(string n, int aNo) : name(n), aptNumber(aNo){}

tenant::~tenant() {}

int tenant::getAptNumber()
{
	return aptNumber;
}

bool operator < (const tenant& t1, const tenant& t2)
{
	return t1.name < t2.name;
}

bool operator == (const tenant& t1, const tenant& t2)
{
	return t1.name == t2.name;
}

ostream& operator << (ostream& s, const tenant& t)
{
	s << t.aptNumber << '\t' << t.name << endl; return s;
}

void tenantInputScreen::getTenant() // получение данных о жильцах
{
	cout << "Введите имя жильца (Например: Maks Lavrov): ";
	getaLine(tName);
	cout << "Введите номер комнаты (Например: 101): ";
	cin >> aptNo;
	cin.ignore(80, '\n'); // создать жильца
	tenant* ptrTenant = new tenant(tName, aptNo);
	ptrTenantList->insertTenant(ptrTenant); // занести в список жильцов

	ptrTenantList->loadFromXML("tenantList.xml");

	// сохранить данные в XML файл
	ptrTenantList->saveToXML("tenantList.xml");
}

bool compareTenants::operator () (tenant* ptrT1,
	tenant* ptrT2) const
{
	return *ptrT1 < *ptrT2;
}

tenantList::~tenantList() // деструктор
{
	while (!setPtrsTens.empty()) // удаление всех жильцов,
	{ // удаление указателей из
	// множества
		iter = setPtrsTens.begin();
		delete* iter;
		setPtrsTens.erase(iter);
	}
} 

void tenantList::insertTenant(tenant* ptrT)
{
	setPtrsTens.insert(ptrT); //вставка
}

int tenantList::getAptNo(string tName) // имя присутствует в списке?
{
	int aptNo;
	tenant dummy(tName, 0);
	iter = setPtrsTens.begin();
	while (iter != setPtrsTens.end())
	{
		aptNo = (*iter)->getAptNumber(); // поиск жильца
		if (dummy == **iter++) // в списке?
			return aptNo; // да
	}
	return -1; // нет
}

void tenantList::display() // вывод списка жильцов
{
	loadFromXML("tenantList.xml"); // считывание данных из XML файла

	cout << "\nApt#\tИмя жильца\n-------------------\n";
	if (setPtrsTens.empty())
		cout << "***Нет жильцов***\n";
	else
	{
		iter = setPtrsTens.begin();
		while (iter != setPtrsTens.end())
			cout << **iter++;
	}
}

void tenantList::saveToXML(const std::string& filename)
{
	XMLDocument doc;
	XMLNode* rootNode = doc.NewElement("TenantList");
	doc.InsertFirstChild(rootNode);

	for (const auto& tenantPtr : setPtrsTens)
	{
		XMLElement* tenantElement = doc.NewElement("Tenant");

		// Имя жильца
		XMLElement* nameElement = doc.NewElement("Name");
		nameElement->SetText(tenantPtr->name.c_str());
		tenantElement->InsertEndChild(nameElement);

		// Номер комнаты жильца
		XMLElement* aptNumberElement = doc.NewElement("AptNumber");
		aptNumberElement->SetText(tenantPtr->aptNumber);
		tenantElement->InsertEndChild(aptNumberElement);

		rootNode->InsertEndChild(tenantElement);
	}
	doc.SaveFile(filename.c_str());
}

void tenantList::loadFromXML(const std::string& filename)
{
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	XMLNode* rootNode = doc.FirstChildElement("TenantList");
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	for (XMLElement* tenantElement = rootNode->FirstChildElement("Tenant"); tenantElement; tenantElement = tenantElement->NextSiblingElement("Tenant"))
	{
		const char* name = nullptr;
		int aptNumber = 0;

		XMLElement* nameElement = tenantElement->FirstChildElement("Name");
		if (nameElement)
			name = nameElement->GetText();

		XMLElement* aptNumberElement = tenantElement->FirstChildElement("AptNumber");
		if (aptNumberElement)
			aptNumber = atoi(aptNumberElement->GetText());

		if (name && aptNumber != 0)
		{
			tenant* ptrTenant = new tenant(name, aptNumber);
			setPtrsTens.insert(ptrTenant);
		}
	}
}

void tenantList::saveToXML(const std::string& filename, const std::string& elementName)
{
	XMLDocument doc;
	XMLNode* rootNode = doc.NewElement("TenantList");
	doc.InsertFirstChild(rootNode);

	for (const auto& tenantPtr : setPtrsTens)
	{
		if (tenantPtr->name == elementName) // Изменено: проверяем имя жильца
			continue;

		XMLElement* tenantElement = doc.NewElement("Tenant");

		// Имя жильца
		XMLElement* nameElement = doc.NewElement("Name");
		nameElement->SetText(tenantPtr->name.c_str());
		tenantElement->InsertEndChild(nameElement);

		// Номер комнаты жильца
		XMLElement* aptNumberElement = doc.NewElement("AptNumber");
		aptNumberElement->SetText(tenantPtr->aptNumber);
		tenantElement->InsertEndChild(aptNumberElement);

		rootNode->InsertEndChild(tenantElement);
	}

	doc.SaveFile(filename.c_str());
}

void tenantList::deleteTenant(const std::string& name)
{
	tenant dummy(name, 0);
	iter = setPtrsTens.begin();
	while (iter != setPtrsTens.end())
	{
		if (dummy == **iter)
		{
			delete* iter;
			setPtrsTens.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
}

void deleteFromXML(const std::string& filename, const std::string& elementName)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	tinyxml2::XMLNode* rootNode = doc.FirstChildElement();
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	// Поиск элементов с заданным именем и их удаление
	tinyxml2::XMLElement* element = rootNode->FirstChildElement(elementName.c_str());
	while (element)
	{
		tinyxml2::XMLElement* nextElement = element->NextSiblingElement(elementName.c_str());
		rootNode->DeleteChild(element);
		element = nextElement;
	}

	// Сохранение изменений в файл XML
	doc.SaveFile(filename.c_str());
}

void rentRecord::saveToXML(const std::string& filename)
{
	XMLDocument doc;
	XMLNode* rootNode = doc.NewElement("RentRecord");
	doc.InsertFirstChild(rootNode);

	for (const auto& rentRowPtr : setPtrsRR)
	{
		XMLElement* rentRowElement = doc.NewElement("RentRow");

		XMLElement* aptNoElement = doc.NewElement("AptNo");
		aptNoElement->SetText(rentRowPtr->aptNo);
		rentRowElement->InsertEndChild(aptNoElement);

		for (int i = 0; i < 12; ++i)
		{
			XMLElement* rentElement = doc.NewElement("Rent");
			rentElement->SetAttribute("month", i + 1);
			rentElement->SetText(rentRowPtr->rent[i]);
			rentRowElement->InsertEndChild(rentElement);
		}

		rootNode->InsertEndChild(rentRowElement);
	}

	doc.SaveFile(filename.c_str());
}


void rentRecord::loadFromXML(const std::string& filename)
{
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	XMLNode* rootNode = doc.FirstChildElement("RentRecord");
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	for (XMLElement* rentRowElement = rootNode->FirstChildElement("RentRow"); rentRowElement; rentRowElement = rentRowElement->NextSiblingElement("RentRow"))
	{
		int aptNo = 0;
		rentRow* ptrRentRow = nullptr;

		XMLElement* aptNoElement = rentRowElement->FirstChildElement("AptNo");
		if (aptNoElement)
			aptNo = atoi(aptNoElement->GetText());

		if (aptNo != 0)
		{
			ptrRentRow = new rentRow(aptNo);

			for (XMLElement* rentElement = rentRowElement->FirstChildElement("Rent"); rentElement; rentElement = rentElement->NextSiblingElement("Rent"))
			{
				int month = 0;
				float amount = 0.0;

				rentElement->QueryIntAttribute("month", &month);
				amount = atof(rentElement->GetText());

				if (month >= 1 && month <= 12)
					ptrRentRow->setRent(month - 1, amount);
			}
		}

		if (ptrRentRow != nullptr)
			setPtrsRR.insert(ptrRentRow);
	}
}

void rentRecord::deleteRent(int aptNo)
{
	iter = setPtrsRR.begin();
	while (iter != setPtrsRR.end())
	{
		if ((*iter)->aptNo == aptNo)
		{
			delete* iter;
			setPtrsRR.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
}


void deleteFromXMLl(const std::string& filename, const std::string& elementName)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	tinyxml2::XMLNode* rootNode = doc.FirstChildElement();
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	// Поиск элементов с заданным именем и их удаление
	tinyxml2::XMLElement* element = rootNode->FirstChildElement(elementName.c_str());
	while (element)
	{
		tinyxml2::XMLElement* nextElement = element->NextSiblingElement(elementName.c_str());
		rootNode->DeleteChild(element);
		element = nextElement;
	}

	// Сохранение изменений в файл XML
	doc.SaveFile(filename.c_str());
}

void expenseRecord::saveToXML(const std::string& filename)
{
	XMLDocument doc;
	XMLNode* rootNode = doc.NewElement("ExpenseRecord");
	doc.InsertFirstChild(rootNode);

	for (const auto& expensePtr : vectPtrsExpenses)
	{
		XMLElement* expenseElement = doc.NewElement("Expense");

		XMLElement* monthElement = doc.NewElement("Month");
		monthElement->SetText(expensePtr->month);
		expenseElement->InsertEndChild(monthElement);

		XMLElement* dayElement = doc.NewElement("Day");
		dayElement->SetText(expensePtr->day);
		expenseElement->InsertEndChild(dayElement);

		XMLElement* categoryElement = doc.NewElement("Category");
		categoryElement->SetText(expensePtr->category.c_str());
		expenseElement->InsertEndChild(categoryElement);

		XMLElement* payeeElement = doc.NewElement("Payee");
		payeeElement->SetText(expensePtr->payee.c_str());
		expenseElement->InsertEndChild(payeeElement);

		XMLElement* amountElement = doc.NewElement("Amount");
		amountElement->SetText(expensePtr->amount);
		expenseElement->InsertEndChild(amountElement);

		rootNode->InsertEndChild(expenseElement);
	}

	doc.SaveFile(filename.c_str());
}

void expenseRecord::loadFromXML(const std::string& filename)
{
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	XMLNode* rootNode = doc.FirstChildElement("ExpenseRecord");
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	for (XMLElement* expenseElement = rootNode->FirstChildElement("Expense"); expenseElement; expenseElement = expenseElement->NextSiblingElement("Expense"))
	{
		int month = 0;
		int day = 0;
		std::string category;
		std::string payee;
		float amount = 0.0;

		XMLElement* monthElement = expenseElement->FirstChildElement("Month");
		if (monthElement)
			month = atoi(monthElement->GetText());

		XMLElement* dayElement = expenseElement->FirstChildElement("Day");
		if (dayElement)
			day = atoi(dayElement->GetText());

		XMLElement* categoryElement = expenseElement->FirstChildElement("Category");
		if (categoryElement)
			category = categoryElement->GetText();

		XMLElement* payeeElement = expenseElement->FirstChildElement("Payee");
		if (payeeElement)
			payee = payeeElement->GetText();

		XMLElement* amountElement = expenseElement->FirstChildElement("Amount");
		if (amountElement)
			amount = atof(amountElement->GetText());

		expense* ptrExpense = new expense(month, day, category, payee, amount);
		vectPtrsExpenses.push_back(ptrExpense);
	}
	
}

void deleteFromXMLk(const std::string& filename, const std::string& elementName)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Ошибка при загрузке XML файла." << std::endl;
		return;
	}

	tinyxml2::XMLNode* rootNode = doc.FirstChildElement();
	if (!rootNode)
	{
		std::cout << "Некорректный формат XML файла." << std::endl;
		return;
	}

	tinyxml2::XMLElement* element = rootNode->FirstChildElement(elementName.c_str());
	while (element)
	{
		tinyxml2::XMLElement* nextElement = element->NextSiblingElement(elementName.c_str());
		rootNode->DeleteChild(element);
		element = nextElement;
	}

	doc.SaveFile(filename.c_str());
}

rentRow::rentRow(int an) : aptNo(an) //1-арг. конструктор
{
	fill(&rent[0], &rent[12], 0);
}

void rentRow::setRent(int m, float am)
{
	rent[m] = am;
}

float rentRow::getSumOfRow() // сумма арендных платежей
// в строке
{
	return accumulate(&rent[0], &rent[12], 0);
}

bool operator < (const rentRow& t1, const rentRow& t2)
{
	return t1.aptNo < t2.aptNo;
}

bool operator == (const rentRow& t1, const rentRow& t2)
{
	return t1.aptNo == t2.aptNo;
}

ostream& operator << (ostream& s, const rentRow& an)
{
	s << an.aptNo << '\t'; // вывести номер комнаты
	for (int j = 0; j < 12; j++) // вывести 12 арендных платежей
	{
		if (an.rent[j] == 0)
			s << " 0 ";
		else
			s << an.rent[j] << " ";
	}
	s << endl;
	return s;
}

bool compareRows::operator () (rentRow* ptrR1,
	rentRow* ptrR2) const
{
	return *ptrR1 < *ptrR2;
}

rentRecord::~rentRecord() 
{
	while (!setPtrsRR.empty()) // удалить строки с платежами,
	{ // удалить указатели из множества
		iter = setPtrsRR.begin();
		delete* iter;
		setPtrsRR.erase(iter);
	}
}

void rentRecord::insertRent(int aptNo, int month, float amount)
{
	rentRow searchRow(aptNo); // временная строкаvс тем же aptNo
	iter = setPtrsRR.begin(); // поиск setPtrsRR
	while (iter != setPtrsRR.end())
	{
		if (searchRow == **iter) // rentRow найден?
		{ // да, заносим
			(*iter)->setRent(month, amount); // строку в
			// список
			return;
		}
		else
			iter++;
	} // не найден
	rentRow* ptrRow = new rentRow(aptNo);// новая строка
	ptrRow->setRent(month, amount); // занести в нее платеж
	setPtrsRR.insert(ptrRow); // занести строку в вектор
}

float rentRecord::getSumOfRents() // сумма всех платежей
{
	float sumRents = 0.0;
	iter = setPtrsRR.begin();
	while (iter != setPtrsRR.end())
	{
		sumRents += (*iter)->getSumOfRow();
		iter++;
	}
	return sumRents;
}

void rentInputScreen::getRent()
{
	cout << "Введите имя жильца: ";
	getaLine(renterName);
	aptNo = ptrTenantList->getAptNo(renterName);
	if (aptNo > 0) // если имя найдено,
	{ // получить сумму платежа
		cout << "Введите сумму платежа (345.67): ";
		cin >> rentPaid;
		cin.ignore(80, '\n');
		cout << "Введите номер месяца оплаты (1-12): ";
		cin >> month;
		cin.ignore(80, '\n');
		if (month > 0 && month < 13)
		{
			month--; // (внутренняя нумерация 0-11)
			ptrRentRecord->insertRent(aptNo, month, rentPaid);
		}
		else 
		{
			cout << "Неизвестное число. Введите месяц 1-12\n";
		}
	}
	else // возврат
	{
		cout << "Такого жильца нет.\n";
	}
	
	ptrRentRecord->loadFromXML("RentRecord.xml");

	ptrRentRecord->saveToXML("RentRecord.xml");
} 

void rentRecord::display()
{
	loadFromXML("RentRecord.xml");

	cout << "\nAptNo\tЯнв Фев Мар Апр Май Июн "
		<< "Июл Авг Сен Окт Ноя Дек\n"
		<< "---------------------------------"
		<< "---------------------------------\n";
	if (setPtrsRR.empty())
		cout << "***Нет платежей!***\n";
	else
	{
		iter = setPtrsRR.begin();
		while (iter != setPtrsRR.end())
			cout << **iter++;
	}
}

bool operator < (const expense& e1, const expense& e2)
{ // сравнивает даты
	if (e1.month == e2.month) // если тот же месяц,
		return e1.day < e2.day; // сравнить дни
	else // иначе,
		return e1.month < e2.month; // сравнить месяцы
}

bool operator == (const expense& e1, const expense& e2)
{
	return e1.month == e2.month && e1.day == e2.day;
}

ostream& operator << (ostream& s, const expense& exp)
{
	s << exp.month << '/' << exp.day << '\t' << exp.payee << '\t'
		;
	s << exp.amount << '\t' << exp.category << endl;
	return s;
}

bool compareDates::operator () (expense* ptrE1,
	expense* ptrE2) const
{
	return *ptrE1 < *ptrE2;
}

bool compareCategories::operator () (expense* ptrE1,
	expense* ptrE2) const
{
	return ptrE1->category < ptrE2->category;
}

expenseRecord::~expenseRecord() // деструктор
{
	while (!vectPtrsExpenses.empty()) // удалить объекты expense
	{ // удалитьcуказатели на вектор
		iter = vectPtrsExpenses.begin();
		delete* iter;
		vectPtrsExpenses.erase(iter);
	}
}

void expenseRecord::insertExp(expense* ptrExp)
{
	vectPtrsExpenses.push_back(ptrExp);
}

void expenseRecord::display()
{	
	loadFromXML("ExpenseRecord.xml");

	cout << "\nДата\tПолучатель\t\tСумма\tКатегория\n"
		<< "----------------------------------------\n";
	if (vectPtrsExpenses.size() == 0)
		cout << "***Расходов нет***\n";
	else
	{
		sort(vectPtrsExpenses.begin(), // сортировка по дате
			vectPtrsExpenses.end(), compareDates());
		iter = vectPtrsExpenses.begin();
		while (iter != vectPtrsExpenses.end())
			cout << **iter++;
	}
}

float expenseRecord::displaySummary() // используется приcсоставлении годового отчета
{
	float totalExpenses = 0; // сумма, все категории
	if (vectPtrsExpenses.size() == 0)
	{
		cout << "\tВсе категории\t0\n";
		return 0;
	}
	// сортировать по категории
	sort(vectPtrsExpenses.begin(),
		vectPtrsExpenses.end(), compareCategories());
	// по каждой категории сумма записей
	iter = vectPtrsExpenses.begin();
	string tempCat = (*iter)->category;
	float sumCat = 0.0;
	while (iter != vectPtrsExpenses.end())
	{
		if (tempCat == (*iter)->category)
			sumCat += (*iter)->amount; // та же категория
		else
		{ // другая
			cout << '\t' << tempCat << '\t' << sumCat << endl;
			totalExpenses += sumCat; // прибавить предыдущую категорию
			tempCat = (*iter)->category;
			sumCat = (*iter)->amount;// прибавить конечное значение суммы
		}
		iter++;
	} 
	totalExpenses += sumCat; // прибавить сумму конечной категории
	cout << '\t' << tempCat << '\t' << sumCat << endl;
	return totalExpenses;
} 

expenseInputScreen::expenseInputScreen(expenseRecord* per) :
	ptrExpenseRecord(per) {}


void expenseInputScreen::getExpense()
{
	int month, day;
	string category, payee;
	float amount;
	cout << "Введите месяц (1-12): ";
	cin >> month;
	cin.ignore(80, '\n');
	if (month > 0 && month < 13)
	{
		cout << "Введите день (1-31): ";
		cin >> day;
		cin.ignore(80, '\n');
		if (day > 0 && day < 32)
		{
			cout << "Введите категорию расходов (Ремонт, Налоги): ";
			getaLine(category);
			cout << "Введите получателя " << "(Prostokvashino): ";
			getaLine(payee);
			cout << "Введите сумму (39.95): ";
			cin >> amount;
			cin.ignore(80, '\n');
			expense* ptrExpense = new
				expense(month, day, category, payee, amount);
			ptrExpenseRecord->insertExp(ptrExpense);
		}
		else
			cout << "Неизвестное число. Введите день 1-31\n";
	}
	else
		cout << "Неизвестное число. Введите месяц 1-12\n";

	//ptrExpenseRecord->loadFromXML("ExpenseRecord.xml");

	ptrExpenseRecord->saveToXML("ExpenseRecord.xml");
}

annualReport::annualReport(rentRecord* pRR,
	expenseRecord* pER) : ptrRR(pRR), ptrER(pER) {}

void annualReport::display()
{
	cout << "Годовой отчет\n--------------\n";
	cout << "Доходы\n";
	cout << "\tАрендная плата\t\t";
	rents = ptrRR->getSumOfRents();
	cout << rents << endl;
	cout << "Расходы\n";
	expenses = ptrER->displaySummary();
	cout << "\nБаланс\t\t\t" << rents - expenses << endl;
}

userInterface::userInterface()
{
	ptrTenantList = new tenantList;
	ptrRentRecord = new rentRecord;
	ptrExpenseRecord = new expenseRecord;
}

userInterface::~userInterface()
{
	delete ptrTenantList;
	delete ptrRentRecord;
	delete ptrExpenseRecord;
}

void userInterface::interact()
{
	string name;
	int num;
	int AptNo;
	string category, payee;


	while (true)
	{
		cout << "Для ввода данных нажмите 'i', \n"
			<< " 'c' для удаления конкретного жильца их списка, \n"
			<< " 'u' для удаления конкретной арендной платы, \n"
			<< " 'o' для удаления всех жильцов, \n"
			<< " 'l' для удаления всех записей арендной платы, \n"
			<< " 'k' для удаления всех записей расходов, \n"
			<< " 'd' для вывода отчета, \n"
			<< " 'q' для выхода: ";
		ch = getaChar();
		if (ch == 'i')
		{
			cout << " 't' для добавления жильца, \n"
				<< " 'r' для записи арендной платы, \n"
				<< " 'e' для записи расходов: ";
			ch = getaChar();
			switch (ch)
			{
			case 't': ptrTenantInputScreen =
				new tenantInputScreen(ptrTenantList);
				ptrTenantInputScreen->getTenant();
				delete ptrTenantInputScreen;
				break;
			case 'r': ptrRentInputScreen =
				new rentInputScreen(ptrTenantList,
					ptrRentRecord);
				ptrRentInputScreen->getRent();
				delete ptrRentInputScreen;
				break;
			case 'e': ptrExpenseInputScreen =
				new expenseInputScreen(ptrExpenseRecord);
				ptrExpenseInputScreen->getExpense();
				delete ptrExpenseInputScreen;
				break;
			default: cout << "Неизвестная функция\n";
				break;
			}
		}
		else if (ch == 'c')
		{
			cout << "Введите имя жильца (например: Andrey Filipov) ";
			getaLine(name);
			ptrTenantList->deleteTenant(name);
			ptrTenantList->saveToXML("tenantList.xml", name);
		}
		else if (ch == 'u') 
		{
			cout << "Введите номер комнаты для удаления арендной платы: ";
			cin >> AptNo;
			ptrRentRecord->deleteRent(AptNo);
			ptrRentRecord->saveToXML("RentRecord.xml");

		}
		else if (ch == 'l') 
			deleteFromXMLl("RentRecord.xml", "RentRow");
		else if (ch == 'k') 
			deleteFromXMLk("ExpenseRecord.xml", "Expense");
		else if (ch == 'd') 
		{
			cout << " 't' для вывода жильцов, \n"
				<< " 'r' для вывода арендной платы\n"
				<< " 'e' для вывода расходов, \n"
				<< " 'a' для вывода годового отчета: ";
			ch = getaChar();
			switch (ch)
			{
			case 't': ptrTenantList->display();
				break;
			case 'r': ptrRentRecord->display();
				break;
			case 'e': ptrExpenseRecord->display();
				break;
			case 'a':
				ptrRentRecord->loadFromXML("RentRecord.xml");
				ptrExpenseRecord->loadFromXML("ExpenseRecord.xml");

				ptrAnnualReport = new annualReport(ptrRentRecord, ptrExpenseRecord);
				ptrAnnualReport->display();
				delete ptrAnnualReport;
				break;
			default: cout << "Неизвестная функция вывода\n";
				break;
			} 
		} 
		else if (ch == 'o') 	
			deleteFromXML("tenantList.xml", "Tenant");
		else if (ch == 'q')
			return;
		else
			cout << "Неизвестная функция. Нажимайте только 'i', 'd' или 'q'\n";
	} 
} 
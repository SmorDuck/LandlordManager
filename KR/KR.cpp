// lordApp.cpp
// файл, поставляемый клиенту.
#include "landlord.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int main()
{
    setlocale(LC_ALL, "RUS");

	userInterface theUserInterface;
	theUserInterface.interact();
	return 0;
}
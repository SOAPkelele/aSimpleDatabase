typedef struct
{
	char FirstName[20];
	char LastName[20];
	char SecondName[20];
}FullName;

typedef struct
{
	float Salary;
	float Taxes;
	float Bonus;
}MoneyInfo;

typedef struct
{
	char DepName[20];
	FullName Person;
	MoneyInfo MoneyFlow;
}StuffUnit;

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include <io.h>
#include "Header.h"
#define _CRT_SECURE_NO_WARNINGS

//глобальные переменные
int size;
StuffUnit Unit;
StuffUnit* StructPtr;
FILE* f;

//прототипы функций
int DefineSize();
void Fill();
void Add();
void DeleteByPos(int pos);
void DeleteMenu();

int Search();
void SearchMenu();
int SearchBy(int(*Srch)(void *), void *field);
int SearchByFirstName(void* name);
int SearchByLastName(void* name);
int SearchBySecondName(void* name);
int SearchByDepartment(void* name);
int SearchBySalary(void* a);
int SearchByBonus(void* a);
int SearchByTaxes(void* a);

void Change();
void Sort();
int cmpString(const void* a, const void* b);
int SortBySalary(int i);
int SortByBonus(int i);
int SortByTaxes(int i);
int SortByFirstName(int i);
int SortByLastName(int i);
int SortBySecondName(int i);
int SortByDepatment(int i);
void bubble_sort(int(*Sort)(int));

void OutputByPos(int pos);
void OutputAll();

void ShowMenu();
void ClearAll();

//Синтаксис объявления указателей на функцию
//<возвращаемый тип> (* <имя>)(<тип аргументов>); 

int main()
{
	setlocale(LC_ALL, "RUS");
	f = fopen("DB.bin", "r+b");
	if (f != NULL)
	{
		ShowMenu();
		fclose(f);
	}
	else printf("Не удалось открыть базу данных.\n");
	return 0;
}

int DefineSize()
{
	rewind(f);
	size = 0;
	while (fread(&Unit, sizeof(StuffUnit), 1, f) == 1)
	{
		size++;
	}
	return size;
}

void Fill()
{
	int choice;
	while (1)
	{
		fseek(f, 0, SEEK_END);
		Add();
		printf("\tЗапись успешно добавлена!\n\n");
		printf("\tПродолжить? Да - 1, Нет - 2.\n\t");
		printf("Ваш выбор: ");
		scanf("%d", &choice);
		printf("\n");
		if (choice == 1)
			continue;
		else if (choice == 2)
			break;
	}
}

void Add()
{
	while (1)
	{
		printf("\tФамилия: ");
		scanf("%s", Unit.Person.LastName);
		if (strlen(Unit.Person.LastName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\tИмя: ");
		scanf("%s", Unit.Person.FirstName);
		if (strlen(Unit.Person.FirstName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\tОтчество: ");
		scanf("%s", Unit.Person.SecondName);
		if (strlen(Unit.Person.SecondName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\tПодразделение: ");
		scanf("%s", Unit.DepName);
		if (strlen(Unit.DepName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n\t");
		}
		else break;
	}
	while (1)
	{
		printf("\tОсновная зарплата: ");
		scanf("%f", &Unit.MoneyFlow.Salary);
		if (Unit.MoneyFlow.Salary >= 0)
			break;
		else
		{
			printf("Некорректный ввод, повторите.\n");
			continue;
		}
	}
	while (1)
	{
		printf("\tПремия: ");
		scanf("%f", &Unit.MoneyFlow.Bonus);
		if (Unit.MoneyFlow.Bonus >= 0)
			break;
		else
		{
			printf("Некорректный ввод, повторите.\n");
			continue;
		}
	}
	while (1)
	{
		printf("\tНалоги за последний год: ");
		scanf("%f", &Unit.MoneyFlow.Taxes);
		if (Unit.MoneyFlow.Taxes >= 0)
			break;
		else
		{
			printf("Некорректный ввод, повторите.\n");
			continue;
		}
	}
	fwrite(&Unit, sizeof(StuffUnit), 1, f);
}

void DeleteMenu()
{
	if (size != 0)
	{
		int pos;
		printf("\t1 - удалить по номеру, 2 - выполнить поиск и удаление\n\t");
		int  des;
		scanf("%d", &des);
		if (des == 1)
		{
			OutputAll();
			while (1)
			{
				printf("\tВведите номер который хотите удалить: ");
				scanf("%d", &pos);
				if (pos <= 0 || pos > size)
				{
					printf("\tОшибочный ввод.\n");
					continue;
				}
				else break;
			}
			DeleteByPos(pos);
			printf("\tУспешное удаление!\n");
		}
		else if (des == 2)
		{
			if (!Search())
			{
				while (1)
				{
					printf("\tВведите номер который хотите удалить: ");
					scanf("%d", &pos);
					if (pos <= 0 || pos > size)
					{
						printf("\tОшибочный ввод.\n");
						continue;
					}
					else break;
				}
				DeleteByPos(pos);
				printf("\tЗапись успешно удалена!\n\n");
			}
		}
		else printf("Incorrect input\n\n");
	}
	else printf("\tВ базе данных нет записей!\n\n");
	printf("\n\n");
	return 0;
}

void DeleteByPos(int pos)
{
	rewind(f);
	for (pos; pos < size; pos++)
	{
		fseek(f, pos * sizeof(StuffUnit), SEEK_SET);
		fread(&Unit, sizeof(StuffUnit), 1, f);
		fseek(f, (pos - 1) * sizeof(StuffUnit), SEEK_SET);
		fwrite(&Unit, sizeof(StuffUnit), 1, f);
	}
	_chsize(_fileno(f), (size - 1) * sizeof(StuffUnit));
}

//Функции поиска
void SearchMenu()
{
	if (size != 0)
	{
		Search();
	}
	else printf("\tВ базе данных нет записей!\n");
	printf("\n");
	return 0;
}

int Search()
{
	char name[20];
	float *a;
	int choice;
	printf("\tПоиск по имени - 1, по фамилии - 2, по отчеству - 3, по подразделению - 4, по зарплате - 5, по премии - 6, по налогам - 7.\n\t");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		while (1)
		{
			printf("\tИмя: ");
			scanf("%s", name);
			if (strlen(name) > 20)
			{
				printf("Max size of this field is 20 letters, enter again.\n");
			}
			else break;
		}
		if (SearchBy(SearchByFirstName, name, f)) return 1;
		break;
	case 2:
		while (1)
		{
			printf("\tФамилия: ");
			scanf("%s", name);
			if (strlen(name) > 20)
			{
				printf("Max size of this field is 20 letters, enter again.\n");
			}
			else break;
		}
		if (SearchBy(SearchByLastName, name, f)) return 1;
		break;
	case 3:
		while (1)
		{
			printf("\tОтчество: ");
			scanf("%s", name);
			if (strlen(name) > 20)
			{
				printf("Max size of this field is 20 letters, enter again.\n");
			}
			else break;
		}
		if (SearchBy(SearchBySecondName, name, f)) return 1;
		break;
	case 4:
		while (1)
		{
			printf("\tПодразделение: ");
			scanf("%s", name);
			if (strlen(name) > 20)
			{
				printf("Max size of this field is 20 letters, enter again.\n");
			}
			else break;
		}
		if (SearchBy(SearchByDepartment, name, f)) return 1;
		break;
	case 5:
		printf("\tЗарплата: ");
		scanf("%f", &a);
		if (SearchBy(SearchBySalary, &a, f)) return 1;
		break;
	case 6:
		printf("\tПремия: ");
		scanf("%f", &a);
		if (SearchBy(SearchByBonus, &a, f)) return 1;
		break;
	case 7:
		printf("\tРазмер налога: ");
		scanf("%f", &a);
		if (SearchBy(SearchByTaxes, &a, f)) return 1;
		break;
	default:
		printf("Incorrect input\n");
		return 1;
		break;
	}
}

int SearchBy(int(*Srch)(void *), void *field)
{
	rewind(f);
	int YoN = 0;
	for (int i = 0; i < size; i++)
	{
		fseek(f, i * sizeof(StuffUnit), SEEK_SET);
		fread(&Unit, sizeof(StuffUnit), 1, f);
		if (!Srch(field))
		{
			YoN++;
			if (YoN > 0)
			{
				printf("+-#-+------Фамилия------+--------Имя--------+------Отчество-----+----Подразделение----+---Зарплата---+---Премия---+---Налоги---+\n");
				OutputByPos(i, f);
				printf("\n");
			}
		}
	}
	if (YoN == 0)
	{
		printf("\tЗаписей не найдено.\n\n");
		return 1;
	}
	printf("\n\n");
	return 0;
}

int SearchByFirstName(void *name)
{
	if (strcmp(Unit.Person.FirstName, name) == 0)
		return 0;
	else return 1;
}

int SearchByLastName(void* name)
{
	if (strcmp(Unit.Person.LastName, name) == 0)
		return 0;
	else return 1;
}

int SearchBySecondName(void* name)
{
	if (strcmp(Unit.Person.SecondName, name) == 0)
		return 0;
	else return 1;
}

int SearchByDepartment(void* name)
{
	if (strcmp(Unit.DepName, name) == 0)
		return 0;
	else return 1;
}

int SearchBySalary(void* a)
{
	if (Unit.MoneyFlow.Salary == *(float*)a)
		return 0;
	else return 1;
}

int SearchByTaxes(void* a)
{
	if (Unit.MoneyFlow.Taxes == *(float*)a)
		return 0;
	else return 1;
}

int SearchByBonus(void* a)
{
	if (Unit.MoneyFlow.Bonus == *(float*)a)
		return 0;
	else return 1;
}

void Change()
{
	if (size != 0)
	{
		if (!Search())
		{
			int pos, choice;
			while (1)
			{
				printf("\tВведите номер записи которую хотите изменить: ");
				scanf("%d", &pos);
				if (pos <= 0 || pos > size)
				{
					printf("\tОшибочный ввод.\n");
					continue;
				}
				else break;
			}
			fseek(f, (pos - 1) * sizeof(StuffUnit), SEEK_SET);
			Add();
			printf("\tЗапись успешно изменена!\n\n");
		}
	}
	else printf("\tВ базе данных нет записей!\n");
	printf("\n\n");
	return 0;
}

//Функции сортировки
void Sort()
{
	if (size != 0)
	{
		int choice, i;
		StructPtr = malloc(sizeof(StuffUnit)*size);
		for (i = 0; i < size; i++)
		{
			fseek(f, i * sizeof(StuffUnit), SEEK_SET);
			fread(&StructPtr[i], sizeof(StuffUnit), 1, f);
		}
		printf("Сортировка по имени - 1, по фамилии - 2, по отчеству - 3, по подразделению - 4, по зарплате - 5, по премии - 6, по налогам - 7.\n\t");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1: bubble_sort(SortByFirstName);
			break;
		case 2: bubble_sort(SortByLastName);
			break;
		case 3: bubble_sort(SortBySecondName);
			break;
		case 4: bubble_sort(SortByDepatment);
			break;
		case 5: bubble_sort(SortBySalary);
			break;
		case 6: bubble_sort(SortByBonus);
			break;
		case 7: bubble_sort(SortByTaxes);
			break;
		default:
			printf("Incorrect input.\n\n");
			break;
		}

		printf("+-#-+------Фамилия------+--------Имя--------+------Отчество-----+----Подразделение----+---Зарплата---+---Премия---+---Налоги---+\n");
		for (i = 0; i < size; i++)
		{
			printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
			printf("|%3d|%19s|%19s|%19s|%21s|%14.1f|%12.1f|%12.1f|\n", i + 1, StructPtr[i].Person.LastName, StructPtr[i].Person.FirstName, StructPtr[i].Person.SecondName, StructPtr[i].DepName, StructPtr[i].MoneyFlow.Salary, StructPtr[i].MoneyFlow.Bonus, StructPtr[i].MoneyFlow.Taxes);
			printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
		}
		int des;
		printf("\n\tПрименить изменения? ДА - 1, НЕТ - 2.\n\t");
		scanf("%d", &des);
		if (des == 1)
		{
			for (i = 0; i < size; i++)
			{
				fseek(f, i * sizeof(StuffUnit), SEEK_SET);
				fwrite(&StructPtr[i], sizeof(StuffUnit), 1, f);
			}
			printf("Изменения применены...\n\n");
		}
		else printf("\tИзменения не применены\n\n");
		free(StructPtr);
	}
	else printf("\tВ базе данных нет записей!\n");
	printf("\n");
	return 0;
}

void bubble_sort(int(*Sort)(int))
{
	int i, j;
	StuffUnit A;

	for (i = 0; i < (size - 1); i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (Sort(j))
			{
				/* Swapping */
				A = StructPtr[j];
				StructPtr[j] = StructPtr[j + 1];
				StructPtr[j + 1] = A;
			}
		}
	}
}

int SortBySalary(int i)
{
	if (StructPtr[i].MoneyFlow.Salary > StructPtr[i + 1].MoneyFlow.Salary)
		return 1;
	else return 0;
}

int SortByBonus(int i)
{
	if (StructPtr[i].MoneyFlow.Bonus > StructPtr[i + 1].MoneyFlow.Bonus)
		return 1;
	else return 0;
}

int SortByTaxes(int i)
{
	if (StructPtr[i].MoneyFlow.Taxes > StructPtr[i + 1].MoneyFlow.Taxes)
		return 1;
	else return 0;
}

int SortByFirstName(int i)
{
	if (strcmp(StructPtr[i].Person.FirstName, StructPtr[i + 1].Person.FirstName) > 0)
		return 1;
	else return 0;
}

int SortByLastName(int i)
{
	if (strcmp(StructPtr[i].Person.LastName, StructPtr[i + 1].Person.LastName) > 0)
		return 1;
	else return 0;
}

int SortBySecondName(int i)
{
	if (strcmp(StructPtr[i].Person.SecondName, StructPtr[i + 1].Person.SecondName) > 0)
		return 1;
	else return 0;
}

int SortByDepatment(int i)
{
	if (strcmp(StructPtr[i].DepName, StructPtr[i + 1].DepName) > 0)
		return 1;
	else return 0;
}

int cmpString(const void* a, const void* b)
{
	return strcmp(*(const char**)a, *(const char**)b);
}

void OutputAll()
{
	if (size != 0)
	{
		int i;
		printf("+-#-+------Фамилия------+--------Имя--------+------Отчество-----+----Подразделение----+---Зарплата---+---Премия---+---Налоги---+\n");
		for (i = 0; i < size; i++)
			OutputByPos(i);
		printf("\n");
	}
	else printf("\tВ базе данных нет записей!\n\n");
}

void ClearAll()
{
	if (size != 0)
	{
		int choice;
		printf("Вы учерены, что хотите удалить все?\n\tДа - 1, нет - 2.\n\tВаш выбор: ");
		scanf("%d", &choice);
		if (choice == 1)
		{
			FILE *del = fopen("DB.bin", "wb");
			if (del != NULL)
			{
				printf("База данных успешно очищена.\n\n");
				fclose(del);
			}
			else printf("Не получилось открыть базу данных.\n");
		}
		else if (choice == 2)
			printf("Отмена удаления\n\n");
		else printf("Некорректный ввод.\n");
	}
	else printf("\tВ базе данных нет записей!\n\n");
}

void OutputByPos(int pos)
{
	rewind(f);
	fseek(f, pos * sizeof(StuffUnit), SEEK_SET);
	fread(&Unit, sizeof(StuffUnit), 1, f);
	printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
	printf("|%3d|%19s|%19s|%19s|%21s|%14.1f|%12.1f|%12.1f|\n", pos + 1, Unit.Person.LastName, Unit.Person.FirstName, Unit.Person.SecondName, Unit.DepName, Unit.MoneyFlow.Salary, Unit.MoneyFlow.Bonus, Unit.MoneyFlow.Taxes);
	printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
	return 0;
}

void ShowMenu()
{
	int choice;
	do
	{
		printf("+---+-----Меню-----+\n");
		printf("+---+--------------+\n");
		printf("|-1-|--Заполнение--|\n");
		printf("+---+--------------+\n");
		printf("|-2-|--Добавление--|\n");
		printf("+---+--------------+\n");
		printf("|-3-|---Удаление---|\n");
		printf("+---+--------------+\n");
		printf("|-4-|----Поиск-----|\n");
		printf("+---+--------------+\n");
		printf("|-5-|--Изменение---|\n");
		printf("+---+--------------+\n");
		printf("|-6-|--Сортировка--|\n");
		printf("+---+--------------+\n");
		printf("|-7-|----Вывод-----|\n");
		printf("+---+--------------+\n");
		printf("|-8-|Удаление всего|\n");
		printf("+---+--------------+\n");
		printf("|-9-|----Выход-----|\n");
		printf("+---+--------------+\n\n\t");
		printf("Ваш выбор: ");
		scanf("%d", &choice);
		printf("\n");
		size = DefineSize();
		switch (choice)
		{
		case 1: Fill();
			break;
		case 2: Add();
			printf("\tЗапись успешно добавлена!\n\n");
			break;
		case 3: DeleteMenu();
			break;
		case 4: SearchMenu();
			break;
		case 5: Change();
			break;
		case 6: Sort();
			printf("Сортировка успешно завершена!\n\n");
			break;
		case 7: OutputAll();
			break;
		case 8: ClearAll();
			break;
		case 9: printf("\tЗавершение...\n");
			exit(0);
			break;
		default: printf("\tНекорректный ввод!\n\n");
			break;
		}
	} while (choice != 9);
}
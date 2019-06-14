#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include <io.h>
#include "Header.h"
#define _CRT_SECURE_NO_WARNINGS

//���������� ����������
int size;
StuffUnit Unit;
StuffUnit* StructPtr;
FILE* f;

//��������� �������
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

//��������� ���������� ���������� �� �������
//<������������ ���> (* <���>)(<��� ����������>); 

int main()
{
	setlocale(LC_ALL, "RUS");
	f = fopen("DB.bin", "r+b");
	if (f != NULL)
	{
		ShowMenu();
		fclose(f);
	}
	else printf("�� ������� ������� ���� ������.\n");
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
		printf("\t������ ������� ���������!\n\n");
		printf("\t����������? �� - 1, ��� - 2.\n\t");
		printf("��� �����: ");
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
		printf("\t�������: ");
		scanf("%s", Unit.Person.LastName);
		if (strlen(Unit.Person.LastName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\t���: ");
		scanf("%s", Unit.Person.FirstName);
		if (strlen(Unit.Person.FirstName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\t��������: ");
		scanf("%s", Unit.Person.SecondName);
		if (strlen(Unit.Person.SecondName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n");
		}
		else break;
	}
	while (1)
	{
		printf("\t�������������: ");
		scanf("%s", Unit.DepName);
		if (strlen(Unit.DepName) > 20)
		{
			printf("Max size of this field is 20 letters, enter again.\n\t");
		}
		else break;
	}
	while (1)
	{
		printf("\t�������� ��������: ");
		scanf("%f", &Unit.MoneyFlow.Salary);
		if (Unit.MoneyFlow.Salary >= 0)
			break;
		else
		{
			printf("������������ ����, ���������.\n");
			continue;
		}
	}
	while (1)
	{
		printf("\t������: ");
		scanf("%f", &Unit.MoneyFlow.Bonus);
		if (Unit.MoneyFlow.Bonus >= 0)
			break;
		else
		{
			printf("������������ ����, ���������.\n");
			continue;
		}
	}
	while (1)
	{
		printf("\t������ �� ��������� ���: ");
		scanf("%f", &Unit.MoneyFlow.Taxes);
		if (Unit.MoneyFlow.Taxes >= 0)
			break;
		else
		{
			printf("������������ ����, ���������.\n");
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
		printf("\t1 - ������� �� ������, 2 - ��������� ����� � ��������\n\t");
		int  des;
		scanf("%d", &des);
		if (des == 1)
		{
			OutputAll();
			while (1)
			{
				printf("\t������� ����� ������� ������ �������: ");
				scanf("%d", &pos);
				if (pos <= 0 || pos > size)
				{
					printf("\t��������� ����.\n");
					continue;
				}
				else break;
			}
			DeleteByPos(pos);
			printf("\t�������� ��������!\n");
		}
		else if (des == 2)
		{
			if (!Search())
			{
				while (1)
				{
					printf("\t������� ����� ������� ������ �������: ");
					scanf("%d", &pos);
					if (pos <= 0 || pos > size)
					{
						printf("\t��������� ����.\n");
						continue;
					}
					else break;
				}
				DeleteByPos(pos);
				printf("\t������ ������� �������!\n\n");
			}
		}
		else printf("Incorrect input\n\n");
	}
	else printf("\t� ���� ������ ��� �������!\n\n");
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

//������� ������
void SearchMenu()
{
	if (size != 0)
	{
		Search();
	}
	else printf("\t� ���� ������ ��� �������!\n");
	printf("\n");
	return 0;
}

int Search()
{
	char name[20];
	float *a;
	int choice;
	printf("\t����� �� ����� - 1, �� ������� - 2, �� �������� - 3, �� ������������� - 4, �� �������� - 5, �� ������ - 6, �� ������� - 7.\n\t");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		while (1)
		{
			printf("\t���: ");
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
			printf("\t�������: ");
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
			printf("\t��������: ");
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
			printf("\t�������������: ");
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
		printf("\t��������: ");
		scanf("%f", &a);
		if (SearchBy(SearchBySalary, &a, f)) return 1;
		break;
	case 6:
		printf("\t������: ");
		scanf("%f", &a);
		if (SearchBy(SearchByBonus, &a, f)) return 1;
		break;
	case 7:
		printf("\t������ ������: ");
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
				printf("+-#-+------�������------+--------���--------+------��������-----+----�������������----+---��������---+---������---+---������---+\n");
				OutputByPos(i, f);
				printf("\n");
			}
		}
	}
	if (YoN == 0)
	{
		printf("\t������� �� �������.\n\n");
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
				printf("\t������� ����� ������ ������� ������ ��������: ");
				scanf("%d", &pos);
				if (pos <= 0 || pos > size)
				{
					printf("\t��������� ����.\n");
					continue;
				}
				else break;
			}
			fseek(f, (pos - 1) * sizeof(StuffUnit), SEEK_SET);
			Add();
			printf("\t������ ������� ��������!\n\n");
		}
	}
	else printf("\t� ���� ������ ��� �������!\n");
	printf("\n\n");
	return 0;
}

//������� ����������
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
		printf("���������� �� ����� - 1, �� ������� - 2, �� �������� - 3, �� ������������� - 4, �� �������� - 5, �� ������ - 6, �� ������� - 7.\n\t");
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

		printf("+-#-+------�������------+--------���--------+------��������-----+----�������������----+---��������---+---������---+---������---+\n");
		for (i = 0; i < size; i++)
		{
			printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
			printf("|%3d|%19s|%19s|%19s|%21s|%14.1f|%12.1f|%12.1f|\n", i + 1, StructPtr[i].Person.LastName, StructPtr[i].Person.FirstName, StructPtr[i].Person.SecondName, StructPtr[i].DepName, StructPtr[i].MoneyFlow.Salary, StructPtr[i].MoneyFlow.Bonus, StructPtr[i].MoneyFlow.Taxes);
			printf("+---+-------------------+-------------------+-------------------+---------------------+--------------+------------+------------+\n");
		}
		int des;
		printf("\n\t��������� ���������? �� - 1, ��� - 2.\n\t");
		scanf("%d", &des);
		if (des == 1)
		{
			for (i = 0; i < size; i++)
			{
				fseek(f, i * sizeof(StuffUnit), SEEK_SET);
				fwrite(&StructPtr[i], sizeof(StuffUnit), 1, f);
			}
			printf("��������� ���������...\n\n");
		}
		else printf("\t��������� �� ���������\n\n");
		free(StructPtr);
	}
	else printf("\t� ���� ������ ��� �������!\n");
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
		printf("+-#-+------�������------+--------���--------+------��������-----+----�������������----+---��������---+---������---+---������---+\n");
		for (i = 0; i < size; i++)
			OutputByPos(i);
		printf("\n");
	}
	else printf("\t� ���� ������ ��� �������!\n\n");
}

void ClearAll()
{
	if (size != 0)
	{
		int choice;
		printf("�� �������, ��� ������ ������� ���?\n\t�� - 1, ��� - 2.\n\t��� �����: ");
		scanf("%d", &choice);
		if (choice == 1)
		{
			FILE *del = fopen("DB.bin", "wb");
			if (del != NULL)
			{
				printf("���� ������ ������� �������.\n\n");
				fclose(del);
			}
			else printf("�� ���������� ������� ���� ������.\n");
		}
		else if (choice == 2)
			printf("������ ��������\n\n");
		else printf("������������ ����.\n");
	}
	else printf("\t� ���� ������ ��� �������!\n\n");
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
		printf("+---+-----����-----+\n");
		printf("+---+--------------+\n");
		printf("|-1-|--����������--|\n");
		printf("+---+--------------+\n");
		printf("|-2-|--����������--|\n");
		printf("+---+--------------+\n");
		printf("|-3-|---��������---|\n");
		printf("+---+--------------+\n");
		printf("|-4-|----�����-----|\n");
		printf("+---+--------------+\n");
		printf("|-5-|--���������---|\n");
		printf("+---+--------------+\n");
		printf("|-6-|--����������--|\n");
		printf("+---+--------------+\n");
		printf("|-7-|----�����-----|\n");
		printf("+---+--------------+\n");
		printf("|-8-|�������� �����|\n");
		printf("+---+--------------+\n");
		printf("|-9-|----�����-----|\n");
		printf("+---+--------------+\n\n\t");
		printf("��� �����: ");
		scanf("%d", &choice);
		printf("\n");
		size = DefineSize();
		switch (choice)
		{
		case 1: Fill();
			break;
		case 2: Add();
			printf("\t������ ������� ���������!\n\n");
			break;
		case 3: DeleteMenu();
			break;
		case 4: SearchMenu();
			break;
		case 5: Change();
			break;
		case 6: Sort();
			printf("���������� ������� ���������!\n\n");
			break;
		case 7: OutputAll();
			break;
		case 8: ClearAll();
			break;
		case 9: printf("\t����������...\n");
			exit(0);
			break;
		default: printf("\t������������ ����!\n\n");
			break;
		}
	} while (choice != 9);
}
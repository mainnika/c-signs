#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIGNS_COUNT 13
#define ZNAKS_MAX 12
#define STR_LEN 32

const char* SIGNS[SIGNS_COUNT] = {
	"",
	"aries",
	"taurus",
	"gemini",
	"cancer",
	"leo",
	"virgo",
	"libra",
	"scorpio",
	"sagittarius",
	"capricorn",
	"aquarius",
	"pisces"
};

enum Bday
{
	DAY,
	MONTH,
	YEAR,

	BDAY_MAX
};

struct Znak
{
	char first_name[STR_LEN];
	char last_name[STR_LEN];
	int bday[BDAY_MAX];
	int sign;
};

struct LinkedList
{
	void* data;
	struct LinkedList* next;
};

int str_to_sign(char* sign_str)
{
	int sign = 0;

	for (int i = 0; i < SIGNS_COUNT; i++)
	{
		int compare = strncmp(sign_str, SIGNS[i], STR_LEN);

		if (compare != 0)
		{
			continue;
		}

		sign = i;
		break;
	}

	return sign;
}

struct Znak* alloc_znak()
{
	struct Znak* sign = malloc(sizeof(struct Znak));
	memset(sign, 0, sizeof(struct Znak));

	return sign;
}

void get_fullname_from_znak(char* fullname, int fullname_len, struct Znak* znak)
{
	sprintf_s(fullname, fullname_len, "%s %s", znak->first_name, znak->last_name);
}

void get_sign_str_from_znak(char* fullname, int fullname_len, struct Znak* znak)
{
	int sign_s = znak->sign < SIGNS_COUNT ? znak->sign : 0;
	sprintf_s(fullname, fullname_len, "%s", SIGNS[sign_s]);
}

void get_bday_str_from_znak(char* fullname, int fullname_len, struct Znak* znak)
{
	sprintf_s(fullname, fullname_len, "%d.%d.%d", znak->bday[DAY], znak->bday[MONTH], znak->bday[YEAR]);
}

struct LinkedList* alloc_list()
{
	struct LinkedList* list = malloc(sizeof(struct LinkedList));
	memset(list, 0, sizeof(struct LinkedList));

	return list;
}

struct LinkedList* put_znak_to_list(struct Znak* znak, struct LinkedList* list)
{
	while (1)
	{
		if (list->next == NULL)
		{
			list->data = znak;
			list->next = alloc_list();

			return list;
		}

		struct Znak* iter = list->data;

		if (iter->sign > znak->sign)
		{
			struct LinkedList* next = alloc_list();

			*next = *list;

			list->data = znak;
			list->next = next;

			return list;
		}

		list = list->next;
	}
}

void print_znaks_from_list(struct LinkedList* list)
{
	int counter = 0;
	char full_name[STR_LEN * 2];
	char bday_str[STR_LEN];
	char sign_str[STR_LEN];

	printf("---\n");

	if (list->next == NULL)
	{
		printf("List is empty.\n");
		return;
	}

	while (list->next != NULL)
	{
		counter++;

		struct Znak* znak = list->data;

		get_fullname_from_znak(full_name, STR_LEN * 2, znak);
		get_bday_str_from_znak(bday_str, STR_LEN, znak);
		get_sign_str_from_znak(sign_str, STR_LEN, znak);

		printf("Element #%d, Name: %s\nBirthday: %s\nSign: %s\n", counter, full_name, bday_str, sign_str);

		list = list->next;
	}
}

void delete_znaks_from_list(struct LinkedList* list)
{
	struct LinkedList* iter = list->next;

	if (iter == NULL)
	{
		return;
	}

	while (iter->next != NULL)
	{
		struct LinkedList* current = iter;

		iter = iter->next;

		free(current->data);
		free(current);
	}

	free(iter->data);
	free(iter);
}

int find_znaks_by_bday_from_list(struct LinkedList* to, struct LinkedList* from, enum Bday filter, int criteria)
{
	int counter = 0;

	while (from->next != NULL)
	{
		struct Znak* current = from->data;

		if (current->bday[filter] == criteria)
		{
			to->data = alloc_znak();
			to->next = alloc_list();

			*(struct Znak*)to->data = *current;

			to = to->next;
			counter++;
		}

		from = from->next;
	}

	return counter;
}

void getd_s(int* result)
{
	scanf_s("%d", result);
	for (char c = 0; c != '\n' && c != EOF; c = getchar());
}

int main()
{
	int znaks_count = 0;
	struct LinkedList* znaks = alloc_list();

	for (; znaks_count < ZNAKS_MAX; znaks_count++)
	{
		struct Znak* znak = alloc_znak();

		printf("Enter first name (or leave it blank to continue): ");
		gets_s(znak->first_name, STR_LEN);

		int breakInput = strcmp(znak->first_name, "");
		if (breakInput == 0)
		{
			free(znak);
			break;
		}

		printf("Enter last name: ");
		gets_s(znak->last_name, STR_LEN);

		printf("Enter bday day: ");
		getd_s(&znak->bday[DAY]);

		printf("Enter bday month: ");
		getd_s(&znak->bday[MONTH]);

		printf("Enter bday year: ");
		getd_s(&znak->bday[YEAR]);

		char sign_str[STR_LEN];

		while (1)
		{
			printf("Enter sign: ");
			gets_s(sign_str, STR_LEN);

			znak->sign = str_to_sign(sign_str);

			if (znak->sign == 0)
			{
				printf("Invalid sign, please re-enter.\n");
				continue;
			}

			break;
		}

		struct LinkedList* inserted = put_znak_to_list(znak, znaks);
		struct LinkedList* before = inserted->next;

		printf("Filled %d of %d elements.\n", znaks_count + 1, ZNAKS_MAX);

		if (before == NULL || before->data == NULL)
		{
			printf("Currently stored to the end of list.\n");
			continue;
		}

		char full_name[STR_LEN * 2];
		get_fullname_from_znak(full_name, STR_LEN * 2, before->data);

		printf("Currently inserted before \"%s\".\n", full_name);
	}

	printf("Total:\n");
	print_znaks_from_list(znaks);

	int month = 0;
	printf("Enter month to find in a list: ");
	getd_s(&month);

	struct LinkedList* found = alloc_list();
	int found_count = find_znaks_by_bday_from_list(found, znaks, MONTH, month);

	printf("Found %d elements: \n", found_count);
	print_znaks_from_list(found);

	printf("Done.\n");

	delete_znaks_from_list(found);
	delete_znaks_from_list(znaks);
	free(found);
	free(znaks);

	return 0;
}

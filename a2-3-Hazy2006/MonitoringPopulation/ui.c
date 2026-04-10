#define _CRT_SECURE_NO_WARNINGS
#include "ui.h"
#include <stdio.h>
#include <string.h>

void run(Service* service)
{
    int choice;
    do {
        printf("\n");
        printf("1. Add country\n");
        printf("2. Delete country\n");
        printf("3. Update country\n");
        printf("4. Search by name\n");
        printf("5. List all countries\n");
        printf("6. Migrate population\n");
        printf("7. Filter by continent\n");
        printf("8. Undo\n");
        printf("9. Redo\n");
        printf("10. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char name[100], continent[20];
            int population;
            printf("Enter name: ");
            scanf(" %99s", name);
            while (name[0] >= '0' && name[0] <= '9')
            {
                printf("Invalid. Enter name: ");
                scanf(" %99s", name);
            }
            printf("Enter continent: ");
            scanf(" %19s", continent);
            while (continent[0] >= '0' && continent[0] <= '9')
            {
                printf("Invalid. Enter continent: ");
                scanf(" %19s", continent);
            }
            printf("Enter population: ");
            scanf("%d", &population);
            while (population <= 0)
            {
                while (getchar() != '\n');
                printf("Invalid. Enter population: ");
                scanf("%d", &population);
            }
            Country c = createCountry(name, continent, population);
            int result = addCountry(service, c);
            if (result == SUCCESS)
                printf("Country added successfully.\n");
            else if (result == ERROR_ARRAY_FULL)
                printf("Error: Cannot add more countries (limit reached).\n");
            else if (result == ERROR_DUPLICATE)
                printf("Error: Country already exists.\n");
            else if (result == ERROR_INVALID_POPULATION)
                printf("Error: Population must be greater than 0.\n");
            break;
        }
        case 2:
        {
            char name[100];
            printf("Enter country name to delete: ");
            scanf(" %99s", name);
            while (name[0] >= '0' && name[0] <= '9')
            {
                printf("Invalid. Enter name: ");
                scanf(" %99s", name);
            }
            int result = deleteCountry(service, name);
            if (result == SUCCESS)
                printf("Country deleted successfully.\n");
            else
                printf("Error: Country not found.\n");
            break;
        }
        case 3:
        {
            char name[100], newContinent[20];
            int newPopulation;
            printf("Enter country name to update: ");
            scanf(" %99s", name);
            while (name[0] >= '0' && name[0] <= '9')
            {
                printf("Invalid. Enter name: ");
                scanf(" %99s", name);
            }
            printf("Enter new continent: ");
            scanf(" %19s", newContinent);
            while (newContinent[0] >= '0' && newContinent[0] <= '9')
            {
                printf("Invalid. Enter continent: ");
                scanf(" %19s", newContinent);
            }
            printf("Enter new population: ");
            scanf("%d", &newPopulation);
            while (newPopulation <= 0)
            {
                while (getchar() != '\n');
                printf("Invalid. Enter population: ");
                scanf("%d", &newPopulation);
            }
            int result = updateCountry(service, name, newContinent, newPopulation);
            if (result == SUCCESS)
                printf("Country updated successfully.\n");
            else if (result == ERROR_NOT_FOUND)
                printf("Error: Country not found.\n");
            else if (result == ERROR_INVALID_POPULATION)
                printf("Error: Population must be greater than 0.\n");
            break;
        }
        case 4:
        {
            char name[100];
            int count = 0;
            printf("Enter search string: ");
            scanf(" %99s", name);
            Country* results = getByName(service, name, &count);
            if (count == 0)
                printf("No countries found.\n");
            else
            {
                printf("Results:\n");
                for (int i = 0; i < count; i++)
                    printf("%s - %s - %d million\n", results[i].name, results[i].continent, results[i].population);
            }
            if (results != NULL)
                free(results);
            break;
        }
        case 5:
        {
            int size = getSize(service);
            if (size == 0)
                printf("No countries in the database.\n");
            else
            {
                Country* countries = getAllCountries(service);
                printf("All countries:\n");
                for (int i = 0; i < size; i++)
                    printf("%s - %s - %d million\n", countries[i].name, countries[i].continent, countries[i].population);
            }
            break;
        }
        case 6:
        {
            char from[100], to[100];
            int amount;
            printf("Migrate FROM country: \n");
            scanf(" %99s", from);
            while (from[0] >= '0' && from[0] <= '9')
            {
                printf("Invalid. Enter country: ");
                scanf(" %99s", from);
            }
            printf("Migrate TO country: \n");
            scanf(" %99s", to);
            while (to[0] >= '0' && to[0] <= '9')
            {
                printf("Invalid. Enter country: ");
                scanf(" %99s", to);
            }
            printf("Amount to migrate (millions): \n");
            scanf("%d", &amount);
            while (amount <= 0)
            {
                while (getchar() != '\n');
                printf("Invalid. Enter amount: ");
                scanf("%d", &amount);
            }
            int result = migrate(service, from, to, amount);
            if (result == SUCCESS)
                printf("Migration successful.\n");
            else
                printf("Error: Migration failed (country not found or insufficient population).\n");
            break;
        }
        case 7:
        {
            char continent[20];
            int threshold, count = 0;
            printf("Enter continent (or 'all' for all continents): ");
            scanf(" %19s", continent);
            printf("Enter minimum population: ");
            scanf("%d", &threshold);
            Country* results = getByContinent(service, continent, &threshold, &count);
            if (count == 0)
                printf("No countries found.\n");
            else
            {
                printf("Results:\n");
                for (int i = 0; i < count; i++)
                    printf("%s - %s - %d million\n", results[i].name, results[i].continent, results[i].population);
            }
            break;
        }
        case 8:
           undoOperation(service);
            printf("Undo performed.\n");
			break;
        case 9 :
            redoOperation(service);
            printf("Redo performed.\n");
			break;
        case 10:
            printf("Exiting...\n");
			break;
        default:
            printf("Invalid option.\n");
            while (getchar() != '\n');
            break;
        }
    } while (choice != 10);
}

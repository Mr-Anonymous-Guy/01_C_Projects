#include "student.h"

void addStudent()
{
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp)
    {
        perror("Error opening file");
        return;
    }

    struct Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("✅ Student added successfully!\n");
}

void displayStudents()
{
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp)
    {
        printf("No records found.\n");
        return;
    }

    struct Student s;
    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(struct Student), 1, fp))
    {
        printf("Roll: %d\nName: %s\nAge: %d\nMarks: %.2f\n------------------\n",
               s.roll, s.name, s.age, s.marks);
    }
    fclose(fp);
}

void searchStudent()
{
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp)
    {
        printf("No records found.\n");
        return;
    }

    int roll, found = 0;
    struct Student s;
    printf("Enter Roll Number to search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(struct Student), 1, fp))
    {
        if (s.roll == roll)
        {
            printf("Record Found:\nRoll: %d\nName: %s\nAge: %d\nMarks: %.2f\n",
                   s.roll, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("❌ No record found with Roll Number %d\n", roll);

    fclose(fp);
}

void modifyStudent()
{
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp)
    {
        printf("No records found.\n");
        return;
    }

    int roll, found = 0;
    struct Student s;
    printf("Enter Roll Number to modify: ");
    scanf("%d", &roll);
    getchar();

    while (fread(&s, sizeof(struct Student), 1, fp))
    {
        if (s.roll == roll)
        {
            printf("Enter New Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter New Age: ");
            scanf("%d", &s.age);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -(long)sizeof(struct Student), SEEK_CUR); // ✅ fix
            fwrite(&s, sizeof(struct Student), 1, fp);

            found = 1;
            printf("✅ Record updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("❌ No record found.\n");

    fclose(fp);
}

void deleteStudent()
{
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp)
    {
        printf("Error opening file.\n");
        return;
    }

    int roll, found = 0;
    struct Student s;
    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(struct Student), 1, fp))
    {
        if (s.roll != roll)
            fwrite(&s, sizeof(struct Student), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("✅ Record deleted successfully!\n");
    else
        printf("❌ No record found with that Roll Number.\n");
}

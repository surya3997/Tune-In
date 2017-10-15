//Programmer     :     S.Surya Prasath
//Title            :    Tune-In Music Library Organiser

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./include/tune_in.h"
#include "./include/print_funs.c"

#if defined(_WIN32) || defined(_WIN64)
const char *OS = "Windows";
#else
#ifdef __linux
const char *OS = "Linux";
#else
const char *OS = "Unknown";
#endif
#endif

int cnt = 0, strucnt = 0, dircnt = 0, cnt1 = 0, assign, flag = 0;
char garbage[50];

void player()
{
    int play, i, l;
    char playpath[500];
    FILE *fpstructbase = fopen("./files/structbase.txt", "r");

    printf("\n\n\tEnter the importer id tag : ");
    scanf("%d", &play);
    if (play <= cnt)
    {
        for (i = 1; i <= play; i++)
        {
            fgets(garbage, 200, fpstructbase);
            l = strlen(garbage);
            garbage[l - 1] = '\0';

            fgets(id3.dir, 200, fpstructbase);
            l = strlen(id3.dir);
            id3.dir[l - 1] = '\0';

            fgets(id3.title, 200, fpstructbase);
            l = strlen(id3.title);
            id3.title[l - 1] = '\0';

            fgets(id3.artist, 200, fpstructbase);
            l = strlen(id3.artist);
            id3.artist[l - 1] = '\0';

            fgets(id3.album, 200, fpstructbase);
            l = strlen(id3.album);
            id3.album[l - 1] = '\0';

            fgets(id3.year, 200, fpstructbase);
            l = strlen(id3.year);
            id3.year[l - 1] = '\0';
        }

        if (!strcmp(OS, "Windows"))
        {
            sprintf(playpath, "\"%s\"", id3.dir);
            system(playpath);
            system("cls");
        }
        else if (!strcmp(OS, "Linux"))
        {
            sprintf(playpath, "xdg-open \"%s\"", id3.dir);
            system(playpath);
            system("clear");
        }
        return;
    }

    else
    {
        printf("\n\tINVALID INPUT TAG.");
    }
}

void initialise()
{
    FILE *fpvarbase;

    if (fpvarbase = fopen("./files/varbase.txt", "r"))
    {
        fscanf(fpvarbase, "%d", &cnt);
        fscanf(fpvarbase, "%d", &strucnt);
        fscanf(fpvarbase, "%d", &dircnt);
    }
    else
        fpvarbase = fopen("./files/varbase.txt", "w");
    fclose(fpvarbase);
}

/*IMPORTING MP3 TO A FILE*/
int importer(char path[])
{
    FILE *fpfilebase;
    fpfilebase = fopen("./files/filebase.txt", "a+");
    char temppath[500], *null, tester[500];
    int l;
    DIR *dp;
    struct dirent *spdir;
    strcpy(temppath, path);
    flag++;

    null = &temppath[strlen(temppath)];
    dp = opendir(temppath);

    if (dp != NULL)
    {
        if (flag == 1)
            printf("\n\tPlease wait as the files may take some time to import.....");
        while ((spdir = readdir(dp)) != NULL)
        {
            if ((strcmp(spdir->d_name, ".") != 0) && (strcmp(spdir->d_name, "..") != 0))
            {
                strcpy(tester, spdir->d_name);
                l = strlen(tester);
                if (tester[l - 1] == '3')
                {
                    if ((tester[l - 2] == 'p' || tester[l - 2] == 'P'))
                    {
                        if ((tester[l - 3] == 'm' || tester[l - 3] == 'M'))
                        {
                            fprintf(fpfilebase, "%s/%s\n", temppath, spdir->d_name);
                            cnt++;
                            cnt1++;
                        }
                    }
                }

                sprintf(null, "/%s", spdir->d_name);

                if (importer(temppath))
                {
                    closedir(dp);
                    return 1;
                }

                *null = '\0';
            }
        }
    }

    else if (flag == 1)
        return 5;

    closedir(dp);
    fclose(fpfilebase);
    return 0;
}

//IMPORTING FROM FILE TO A STRUCT
void importer1()
{
    int i;
    FILE *fpfilebase;
    char trans[500];
    fpfilebase = fopen("./files/filebase.txt", "r");
    printf("\n\n");

    for (i = 0; i < assign; i++)
    {
        fgets(trans, 500, fpfilebase);
    }

    for (i = assign; i < cnt; i++)
    {
        fgets(trans, 500, fpfilebase);
        trans[strlen(trans) - 1] = '\0';
        strcpy(id3.dir, trans);
        getid3(trans);
        strucnt++;
    }
    fclose(fpfilebase);
}

int getid3(char path[500])
{
    FILE *fpfile;
    fpfile = fopen(path, "r");
    FILE *fpstructbase;
    fpstructbase = fopen("./files/structbase.txt", "a+");
    char buffer[128];
    int x;

    fseek(fpfile, -128, SEEK_END);
    fread(buffer, sizeof(char), sizeof(buffer), fpfile);

    if (!(buffer[0] == 'T' && buffer[1] == 'A' && buffer[2] == 'G'))
    {
        return 0;
    }

    for (x = 0; x < 30; x++)
        id3.title[x] = buffer[x + 3];
    id3.title[30] = '\0';

    for (x = 0; x < 30; x++)
        id3.artist[x] = buffer[x + 33];
    id3.artist[30] = '\0';

    for (x = 0; x < 30; x++)
        id3.album[x] = buffer[x + 63];
    id3.album[30] = '\0';

    for (x = 0; x < 4; x++)
        id3.year[x] = buffer[x + 93];
    id3.year[4] = '\0';

    fprintf(fpstructbase, "%d\n", strucnt + 1);
    fputs(id3.dir, fpstructbase);
    fprintf(fpstructbase, "\n");
    fputs(id3.title, fpstructbase);
    fprintf(fpstructbase, "\n");
    fputs(id3.artist, fpstructbase);
    fprintf(fpstructbase, "\n");
    fputs(id3.album, fpstructbase);
    fprintf(fpstructbase, "\n");
    fputs(id3.year, fpstructbase);
    fprintf(fpstructbase, "\n");

    fclose(fpfile);
    fclose(fpstructbase);
    return 1;
}

void search(int op1)
{
    FILE *fpstructbase = fopen("./files/structbase.txt", "r");
    char str[31], txt[31];
    int m, n, i, i1, j, k, l, f, f1;

    switch (op1)
    {
    case 1:
        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        printf("\nEnter the Song name\t:\t");
        fgets(garbage, sizeof(garbage), stdin);

        fgets(txt, sizeof(txt), stdin);
        n = strlen(txt);

        txt[n--] = '\0';
        f1 = 1;
        printf("\n");

        for (i = 0; i < n; i++)
        {
            if ((int)txt[i] >= 65 && (int)txt[i] <= 90)
                txt[i] = txt[i] + 32;
        }

        for (i1 = 0; i1 < cnt; i1++)
        {

            id3.id = i1 + 1;

            fgets(garbage, 200, fpstructbase);
            l = strlen(garbage);
            garbage[l - 1] = '\0';

            fgets(id3.dir, 200, fpstructbase);
            l = strlen(id3.dir);
            id3.dir[l - 1] = '\0';

            fgets(id3.title, 200, fpstructbase);
            l = strlen(id3.title);
            id3.title[l - 1] = '\0';

            fgets(id3.artist, 200, fpstructbase);
            l = strlen(id3.artist);
            id3.artist[l - 1] = '\0';

            fgets(id3.album, 200, fpstructbase);
            l = strlen(id3.album);
            id3.album[l - 1] = '\0';

            fgets(id3.year, 200, fpstructbase);
            l = strlen(id3.year);
            id3.year[l - 1] = '\0';

            strcpy(str, id3.title);
            m = strlen(str);
            f = 0;

            for (i = 0; i < m; i++)
            {
                if (((int)str[i]) >= 65 && ((int)str[i]) <= 90)
                    str[i] = str[i] + 32;
            }

            for (i = n - 1; i < m; i++)
            {
                k = i;
                if (str[i] == txt[n - 1])
                {
                    if (n == 1)
                    {
                        f = 1;
                    }

                    for (j = n - 2; j >= 0; j--)
                    {
                        k--;
                        if (str[k] == txt[j])
                        {
                            f = 1;
                        }
                        else
                        {
                            f = 0;
                            break;
                        }
                    }
                }

                if (f == 1)
                {
                    break;
                }
            }

            if (f == 1)
            {
                puts(id3.dir);
                printf("Impoter id\t:\t%d\n", id3.id);
                printf("Song\t\t:\t");
                puts(id3.title);
                printf("Album\t\t:\t");
                puts(id3.album);
                printf("Artist\t\t:\t");
                puts(id3.artist);
                printf("Year\t\t:\t");
                puts(id3.year);
                printf("\n");
                f1 = 0;
            }
        }

        if (f1 == 1)
        {
            printf("\n\tNO RESULTS FOUND.");
        }

        fclose(fpstructbase);
        break;

    case 2:
        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        printf("\nEnter the Artist name\t:\t");
        fgets(garbage, sizeof(garbage), stdin);
        fgets(txt, sizeof(txt), stdin);
        n = strlen(txt);

        txt[n--] = '\0';

        f1 = 1;
        printf("\n");

        for (i = 0; i < n; i++)
        {
            if (((int)txt[i]) >= 65 && ((int)txt[i]) <= 90)
                txt[i] = txt[i] + 32;
        }

        for (i1 = 0; i1 < cnt; i1++)
        {

            id3.id = i1 + 1;

            fgets(garbage, 200, fpstructbase);
            l = strlen(garbage);
            garbage[l - 1] = '\0';

            fgets(id3.dir, 200, fpstructbase);
            l = strlen(id3.dir);
            id3.dir[l - 1] = '\0';

            fgets(id3.title, 200, fpstructbase);
            l = strlen(id3.title);
            id3.title[l - 1] = '\0';

            fgets(id3.artist, 200, fpstructbase);
            l = strlen(id3.artist);
            id3.artist[l - 1] = '\0';

            fgets(id3.album, 200, fpstructbase);
            l = strlen(id3.album);
            id3.album[l - 1] = '\0';

            fgets(id3.year, 200, fpstructbase);
            l = strlen(id3.year);
            id3.year[l - 1] = '\0';

            strcpy(str, id3.artist);
            m = strlen(str);
            f = 0;

            for (i = 0; i < m; i++)
            {
                if (((int)str[i]) >= 65 && ((int)str[i]) <= 90)
                    str[i] = str[i] + 32;
            }

            for (i = n - 1; i < m; i++)
            {
                k = i;
                if (str[i] == txt[n - 1])
                {
                    if (n == 1)
                    {
                        f = 1;
                    }

                    for (j = n - 2; j >= 0; j--)
                    {
                        k--;
                        if (str[k] == txt[j])
                        {
                            f = 1;
                        }
                        else
                        {
                            f = 0;
                            break;
                        }
                    }
                }

                if (f == 1)
                {
                    break;
                }
            }

            if (f == 1)
            {
                puts(id3.dir);
                printf("Impoter id\t:\t%d\n", id3.id);
                printf("Song\t\t:\t");
                puts(id3.title);
                printf("Album\t\t:\t");
                puts(id3.album);
                printf("Artist\t\t:\t");
                puts(id3.artist);
                printf("Year\t\t:\t");
                puts(id3.year);
                printf("\n");
                f1 = 0;
            }
        }

        if (f1 == 1)
        {
            printf("\n\tNO RESULTS FOUND.");
        }

        fclose(fpstructbase);
        break;

    case 3:
        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        printf("\nEnter the Album name\t:\t");
        fgets(garbage, sizeof(garbage), stdin);
        fgets(txt, sizeof(txt), stdin);
        n = strlen(txt);

        txt[n--] = '\0';

        f1 = 1;
        printf("\n");

        for (i = 0; i < n; i++)
        {
            if ((((int)txt[i]) >= 65) && (((int)txt[i]) <= 90))
                txt[i] = txt[i] + 32;
        }

        for (i1 = 0; i1 < cnt; i1++)
        {
            id3.id = i1 + 1;

            fgets(garbage, 200, fpstructbase);
            l = strlen(garbage);
            garbage[l - 1] = '\0';

            fgets(id3.dir, 200, fpstructbase);
            l = strlen(id3.dir);
            id3.dir[l - 1] = '\0';

            fgets(id3.title, 200, fpstructbase);
            l = strlen(id3.title);
            id3.title[l - 1] = '\0';

            fgets(id3.artist, 200, fpstructbase);
            l = strlen(id3.artist);
            id3.artist[l - 1] = '\0';

            fgets(id3.album, 200, fpstructbase);
            l = strlen(id3.album);
            id3.album[l - 1] = '\0';

            fgets(id3.year, 200, fpstructbase);
            l = strlen(id3.year);
            id3.year[l - 1] = '\0';

            strcpy(str, id3.album);
            m = strlen(str);
            f = 0;

            for (i = 0; i < m; i++)
            {
                if ((((int)str[i]) >= 65) && (((int)str[i]) <= 90))
                    str[i] = str[i] + 32;
            }

            for (i = n - 1; i < m; i++)
            {
                k = i;
                if (str[i] == txt[n - 1])
                {
                    if (n == 1)
                    {
                        f = 1;
                    }

                    for (j = n - 2; j >= 0; j--)
                    {
                        k--;
                        if (str[k] == txt[j])
                        {
                            f = 1;
                        }
                        else
                        {
                            f = 0;
                            break;
                        }
                    }
                }

                if (f == 1)
                {
                    break;
                }
            }

            if (f == 1)
            {
                puts(id3.dir);
                printf("Impoter id\t:\t%d\n", id3.id);
                printf("Song\t\t:\t");
                puts(id3.title);
                printf("Album\t\t:\t");
                puts(id3.album);
                printf("Artist\t\t:\t");
                puts(id3.artist);
                printf("Year\t\t:\t");
                puts(id3.year);
                printf("\n");
                f1 = 0;
            }
        }

        if (f1 == 1)
        {
            printf("\n\tNO RESULTS FOUND.");
        }

        fclose(fpstructbase);
        break;

    case 4:
        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        printf("\nEnter the Year\t\t:\t");
        fgets(garbage, sizeof(garbage), stdin);
        fgets(txt, sizeof(txt), stdin);
        n = strlen(txt);

        txt[n--] = '\0';

        f1 = 1;
        printf("\n");

        for (i = 0; i < n; i++)
        {
            if ((((int)txt[i]) >= 65) && (((int)txt[i]) <= 90))
                txt[i] = txt[i] + 32;
        }

        for (i1 = 0; i1 < cnt; i1++)
        {
            id3.id = i1 + 1;

            fgets(garbage, 200, fpstructbase);
            l = strlen(garbage);
            garbage[l - 1] = '\0';

            fgets(id3.dir, 200, fpstructbase);
            l = strlen(id3.dir);
            id3.dir[l - 1] = '\0';

            fgets(id3.title, 200, fpstructbase);
            l = strlen(id3.title);
            id3.title[l - 1] = '\0';

            fgets(id3.artist, 200, fpstructbase);
            l = strlen(id3.artist);
            id3.artist[l - 1] = '\0';

            fgets(id3.album, 200, fpstructbase);
            l = strlen(id3.album);
            id3.album[l - 1] = '\0';

            fgets(id3.year, 200, fpstructbase);
            l = strlen(id3.year);
            id3.year[l - 1] = '\0';

            strcpy(str, id3.year);
            m = strlen(str);
            f = 0;

            for (i = 0; i < m; i++)
            {
                if ((((int)str[i]) >= 65) && (((int)str[i]) <= 90))
                    str[i] = str[i] + 32;
            }

            for (i = n - 1; i < m; i++)
            {
                k = i;
                if (str[i] == txt[n - 1])
                {
                    if (n == 1)
                    {
                        f = 1;
                    }

                    for (j = n - 2; j >= 0; j--)
                    {
                        k--;
                        if (str[k] == txt[j])
                        {
                            f = 1;
                        }
                        else
                        {
                            f = 0;
                            break;
                        }
                    }
                }

                if (f == 1)
                {
                    break;
                }
            }

            if (f == 1)
            {
                puts(id3.dir);
                printf("Impoter id\t:\t%d\n", id3.id);
                printf("Song\t\t:\t");
                puts(id3.title);
                printf("Album\t\t:\t");
                puts(id3.album);
                printf("Artist\t\t:\t");
                puts(id3.artist);
                printf("Year\t\t:\t");
                puts(id3.year);
                printf("\n");
                f1 = 0;
            }
        }

        if (f1 == 1)
        {
            printf("\n\tNO RESULTS FOUND.");
        }

        fclose(fpstructbase);
        break;

    default:
        printf("\tINVALID INPUT");
        return;
    }
}

void destroyer()
{
    int dest, i, counter = 0;
    char tempcpy[500];
    if (!strcmp(OS, "Windows"))
        system("cls");
    else if (!strcmp(OS, "Linux"))
        system("clear");
    printf("\n\tEnter the Importer ID of the song to remove from the database  :  ");
    scanf("%d", &dest);

    if (dest > cnt)
    {
        printf("\n\tInvalid Input Tag....");
    }
    else
    {
        FILE *fpstructbase;
        FILE *fptemp;
        FILE *fpvarbase;
        FILE *fpfilebase;

        fpstructbase = fopen("./files/structbase.txt", "r");
        fptemp = fopen("./files/temp.txt", "w");
        for (i = 1; i <= cnt; i++)
        {
            if (dest != i)
            {
                counter++;
            }

            fgets(tempcpy, 200, fpstructbase);
            if (dest != i)
            {
                fprintf(fptemp, "%d\n", counter);
            }

            fgets(tempcpy, 500, fpstructbase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }

            fgets(tempcpy, 500, fpstructbase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }

            fgets(tempcpy, 500, fpstructbase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }

            fgets(tempcpy, 500, fpstructbase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }

            fgets(tempcpy, 500, fpstructbase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }
        }

        fclose(fptemp);
        fclose(fpstructbase);

        fpstructbase = fopen("./files/structbase.txt", "w");
        fptemp = fopen("./files/temp.txt", "r");
        for (i = 1; i <= cnt - 1; i++)
        {
            fgets(tempcpy, 200, fptemp);
            fputs(tempcpy, fpstructbase);

            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpstructbase);

            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpstructbase);

            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpstructbase);

            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpstructbase);

            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpstructbase);
        }

        fclose(fptemp);
        fclose(fpstructbase);

        fpfilebase = fopen("./files/filebase.txt", "r");
        fptemp = fopen("./files/temp.txt", "w");
        for (i = 1; i <= cnt; i++)
        {
            fgets(tempcpy, 500, fpfilebase);
            if (dest != i)
            {
                fputs(tempcpy, fptemp);
            }
        }
        fclose(fptemp);
        fclose(fpfilebase);

        fpfilebase = fopen("./files/filebase.txt", "w");
        fptemp = fopen("./files/temp.txt", "r");
        for (i = 1; i <= cnt - 1; i++)
        {
            fgets(tempcpy, 500, fptemp);
            fputs(tempcpy, fpfilebase);
        }
        fclose(fptemp);
        fclose(fpfilebase);

        cnt--;
        strucnt--;

        fpvarbase = fopen("./files/varbase.txt", "w");
        fprintf(fpvarbase, "%d\n%d\n%d", cnt, strucnt, dircnt);
        fclose(fpvarbase);

        printf("\n\tThe song is removed from the database...");
    }
}

void reset()
{

    FILE *fpdirbase;
    FILE *fpfilebase;
    FILE *fpstructbase;
    FILE *fpvarbase;

    fpfilebase = fopen("./files/filebase.txt", "w");
    fclose(fpfilebase);
    fpstructbase = fopen("./files/structbase.txt", "w");
    fclose(fpstructbase);
    fpdirbase = fopen("./files/dirbase.txt", "w");
    fclose(fpdirbase);
    fpvarbase = fopen("./files/varbase.txt", "w");
    fclose(fpvarbase);

    id3.id = 0;
    strcpy(id3.dir, "\0");
    strcpy(id3.title, "\0");
    strcpy(id3.album, "\0");
    strcpy(id3.artist, "\0");
    strcpy(id3.year, "\0");

    cnt = strucnt = dircnt = assign = 0;
    printf("\n\tDirectory Log cleared.... Import new Directory...");
}

void displayMenu()
{
    printf("\n\n\n\t1. Import Directories");
    printf("\n\t2. Search Categories");
    printf("\n\t3. Display Imported Files");
    printf("\n\t4. Play a Song with importer ID");
    printf("\n\t5. Delete an Entry with importer ID");
    printf("\n\t6. Clear Directory Log");
    printf("\n\t7. About");
    printf("\n\t8. Exit");

    printf("\n\n\n\tEnter your option\t:\t");
}

int main()
{
    FILE *fpdirbase;
    FILE *fpvarbase;
    FILE *fpstructbase;

    char direct[500], ss;
    int op, op1, i, l;

    initialise();

    while (1)
    {
        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        header();
        displayMenu();

        fflush(stdin);
        scanf("%d", &op);

        printf("\a");

        if (!strcmp(OS, "Windows"))
            system("cls");
        else if (!strcmp(OS, "Linux"))
            system("clear");

        switch (op)
        {
        case 1:
            printf("\n\n\tEnter the Directory of the music files\t:\t");
            fgets(garbage, sizeof(garbage), stdin);
            fgets(direct, sizeof(direct), stdin);
            l = strlen(direct);

            fpdirbase = fopen("./files/dirbase.txt", "a+");

            assign = cnt;

            if (direct[l - 1] == '\\')
                direct[l - 1] = '\0';

            if (importer(direct) == 5)
                printf("\n\n\tDirectory not found...\n");
            else
            {
                printf("\n\n\tAll .mp3 files in the directory and ");
                printf("sub-directories were imported successfully.");
                printf("\n\tNumber of files imported : %d", cnt1);
                printf("\n\tTotal files imported in database : %d", cnt);
                flag = 0;
                cnt1 = 0;

                fpvarbase = fopen("./files/varbase.txt", "w");
                fprintf(fpvarbase, "%d\n%d\n%d", cnt, strucnt, dircnt);
                fclose(fpvarbase);

                fputs(direct, fpdirbase);
                fprintf(fpdirbase, "\n");
                dircnt++;
            }

            importer1();
            fclose(fpdirbase);
            scanf(" %c", &ss);
            break;

        case 2:

            if (cnt != 0)
            {
                printf("\n\n\tSearch criteria :");
                printf("\n\t1.Song");
                printf("\n\t2.Artist");
                printf("\n\t3.Album");
                printf("\n\t4.Year");
                printf("\n\n\tEnter your option\t:\t");
                fflush(stdin);
                scanf("%d", &op1);
                search(op1);
                printf("\a");
            }
            else
            {
                printf("\t\t\tNO FILES IMPORTED!");
            }
            scanf(" %c", &ss);
            break;

        case 3:
            if (cnt == 0)
                printf("\t\t\tNO FILES IMPORTED!");
            else
            {
                fpstructbase = fopen("./files/structbase.txt", "r");

                printf("\n");
                for (i = 0; i < cnt; i++)
                {
                    fgets(garbage, 200, fpstructbase);
                    l = strlen(garbage);
                    garbage[l - 1] = '\0';

                    fgets(id3.dir, 200, fpstructbase);
                    l = strlen(id3.dir);
                    id3.dir[l - 1] = '\0';

                    fgets(id3.title, 200, fpstructbase);
                    l = strlen(id3.title);
                    id3.title[l - 1] = '\0';

                    fgets(id3.artist, 200, fpstructbase);
                    l = strlen(id3.artist);
                    id3.artist[l - 1] = '\0';

                    fgets(id3.album, 200, fpstructbase);
                    l = strlen(id3.album);
                    id3.album[l - 1] = '\0';

                    fgets(id3.year, 200, fpstructbase);
                    l = strlen(id3.year);
                    id3.year[l - 1] = '\0';

                    printf("%d\n", i + 1);
                    printf("%s\n", id3.dir);
                    printf("%s\n", id3.title);
                    printf("%s\n", id3.album);
                    printf("%s\n", id3.artist);
                    printf("%s\n", id3.year);
                    printf("\n");
                }
                fclose(fpstructbase);
            }
            printf("\n\n\t%d Files\n\n", cnt);
            scanf(" %c", &ss);
            break;

        case 4:
            player();
            scanf(" %c", &ss);
            break;

        case 5:
            destroyer();
            scanf(" %c", &ss);
            break;

        case 6:
            reset();
            scanf(" %c", &ss);
            break;

        case 7:
            header();
            about();
            scanf(" %c", &ss);
            break;

        case 8:
            printf("\n\n\t\t\tPROGRAM TERMINATES");
            fpvarbase = fopen("./files/varbase.txt", "w");
            fprintf(fpvarbase, "%d\n%d\n%d", cnt, strucnt, dircnt);
            return 0;

        default:
            printf("\n\tINVALID INPUT");
            continue;
        }
    }

    return 0;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ORDERSIZE 200
#define ARGSIZE 200
#define LINE_LENGTH 120
#define FILESIZE 5000

struct orders
{
    char *full_input;
    char *order_head;
    char *order_body;
    char *arg;
    int num;
    struct orders *next;
    struct orders *prev;
};

char trash[1000];
char trash2[1000];
char trash3[1000];
char trash4[1000];

void prep_new_command(struct orders *address);
int read_command(struct orders *address);
void read_arg(struct orders *address);
void edit_arg(struct orders *address);
FILE *make_directory(char *file_stats);
int isDirectoryExists(const char *path);
void insert_file(char *message, int linez, int n, char *file_name);
void parse_text(char *text);
void cat(char *file_name);
int read_before(int linez, int n, char *file_name, char *makan, char *holdprime);
void read_after(int linez, int n, char *file_name, char *makan, char *holdprime, FILE *file_ad);
void remove_str(char *file_name, int linez, int n, int size, char dir);
void copy_clipboard(char *file_name, int linez, int n, int size, char dir);
void cut_to_clipboard(char *file_name, int linez, int n, int size, char dir);
void paste_for_real(char *target);
void copy_for_real(char *target);
int find(char *file_name, char *pattern, int mode);
void put_content_in_string(char *file_name, char *hold);
int check_match(int start, char *source, char *target, int n, int check);
void check_wildcard(char *text);
int read_wildcard(char *source, char end, int start);
// edite insert

int main()
{

    struct orders *firstOrd = (struct orders *)malloc(sizeof(struct orders));
    firstOrd->arg = (char *)malloc(sizeof(char) * ARGSIZE);
    firstOrd->full_input = (char *)malloc(sizeof(char) * (ARGSIZE + 2 * ORDERSIZE));
    firstOrd->order_body = (char *)malloc(sizeof(char) * ORDERSIZE);
    firstOrd->order_head = (char *)malloc(sizeof(char) * ORDERSIZE);
    firstOrd->prev = 0;
    firstOrd->num = 0;

    char *dastan = (char *)malloc(sizeof(char) * 100);
    gets(dastan);
    printf("%c",dastan[1]);
    printf("%d", find("root/file1.txt", dastan, 1));

    // cut_to_clipboard("root/file1.txt", 2, 10, 10, 'b');
    // FILE* add=make_directory("/root/filezz.txt");

    return 0;
}

void prep_new_command(struct orders *address)
{
    address->next = (struct orders *)malloc(sizeof(struct orders));
    address->next->arg = (char *)malloc(sizeof(char) * ARGSIZE);
    address->next->full_input = (char *)malloc(sizeof(char) * (ARGSIZE + 2 * ORDERSIZE));
    address->next->order_head = (char *)malloc(sizeof(char) * ORDERSIZE);
    address->next->order_body = (char *)malloc(sizeof(char) * ORDERSIZE);
    address->next->prev = address;
    address->next->next = 0;
    address->next->num = address->num + 1;
    address = address->next;
}

int read_command(struct orders *address)
{
    printf("kos\n");
    int check;
    gets(address->full_input);
    check = sscanf(address->full_input, "%s %s %s", address->order_head, trash);
    gets(address->arg);
    return check;
}

void read_arg(struct orders *address)
{
    char c;
    int count = 0, j = 0, i = 0;
    while (1)
    {
        c = address->full_input[j];
        if (c == '\n')
            break;
        if (c >= 2)
        {
            address->full_input[j] = address->arg[i];
            i++;
        }
        if (c == ' ')
            count++;
        j++;
    }
    if (count > 2)
        edit_arg(address);
}

void edit_arg(struct orders *address)
{
    char *hold = (char *)malloc(sizeof(char) * ARGSIZE);
    for (int i = 1, j = 0; i < strlen(address->arg) - 1; i++, j++)
    {
        hold[j] = address->arg[i];
    }
    for (int i = 0; i < strlen(hold); i++)
    {
        address->arg[i] = hold[i];
    }
}

FILE *make_directory(char *file_stats)
{
    FILE *address;
    int count = 0;
    char c;
    char hold[ARGSIZE] = {'\0'};
    int i = 1;
    while (1)
    {
        while (1)
        {
            c = file_stats[i];
            if (c == 47)
            {
                count++;
                break;
            }
            hold[i - 1] = c;
            i++;
            if (i == strlen(file_stats))
            {
                count++;
                break;
            }
        }
        if (count == 1)
        {
            if (strcmp("root", hold) || file_stats[0] != 47)
            {
                printf("Error: not started from root");
                return 0;
            }
        }
        if (!isDirectoryExists(hold) && i != strlen(file_stats))
        {
            CreateDirectory(hold, NULL);
            printf("%s\n", hold);
        }
        if (i == strlen(file_stats))
        {
            address = (FILE *)fopen(hold, "w");
            if (!address)
                perror("fopen");
            return address;
        }
        hold[i - 1] = c;
        i++;
    }
}

int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}

void parse_text(char *text)
{
    char hold[ARGSIZE] = {'\0'};
    strcpy(hold, text);
    int i0 = strlen(text);
    int j = 0;
    int i = 0;
    while (i <= i0)
    {
        if (hold[i] != 92)
            text[j] = hold[i];
        else
        {

            if (hold[i + 1] == 92 && hold[i + 2] == 'n')
            {
                i++;
                text[j] = hold[i];
                j++;
                i++;
                text[j] = hold[i];
            }
            else if (hold[i + 1] == 'n')
            {
                text[j] = '\n';
                i++;
            }
            else if (hold[i + 1] == 34 || hold[i + 1] == '*')
                j--;
            else
                text[j] = hold[i];
        }
        i++;
        j++;
    }
}

int read_before(int linez, int n, char *file_name, char *makan, char *holdprime)
{
    int val = 1;
    FILE *file_ad = fopen(file_name, "r");
    char c;
    char hold[ARGSIZE] = {'\0'};
    for (int i = 0; i < linez - 1; i++)
    {
        fgets(hold, LINE_LENGTH, file_ad);
        strcat(makan, hold);
        memset(hold, 0, strlen(hold));
    }
    memset(hold, 0, strlen(hold));
    int i0 = strlen(makan);
    int check = 1;
    fgets(holdprime, LINE_LENGTH, file_ad);
    for (int i = 0; i < n; i++)
    {
        c = holdprime[i];
        if (holdprime[i] == '\0' || holdprime[i] == '\n')
        {
            val = 0;
            for (int j = i; j < n; j++)
            {
                hold[j] = ' ';
            }
            check = 0;
        }
        if (!check)
            break;
        hold[i] = holdprime[i];
    }
    strcat(makan, hold);
    return val;
}

void read_after(int linez, int n, char *file_name, char *makan, char *holdprime, FILE *file_ad)
{
    int j = 0;
    int check = 1;
    char hold[ARGSIZE] = {'\0'};
    memset(hold, 0, strlen(hold));
    if (holdprime[n] == '\n' || holdprime[n] == '\0')
        check = 0;

    for (int i = n; i < LINE_LENGTH; i++)
    {
        if (check == 0)
        {
            hold[0] = '\n';
            break;
        }
        hold[i - n] = holdprime[i];
        if (holdprime[i] == '\n')
            break;
    }
    strcat(makan, hold);
    memset(hold, 0, strlen(hold));
    for (int i = 0; i < linez - 1; i++)
    {
        fgets(hold, LINE_LENGTH, file_ad);
        strcat(makan, hold);
        memset(hold, 0, strlen(hold));
    }
    fclose(file_ad);
}

void insert_file(char *message, int linez, int n, char *file_name)
{
    char c;
    char holdprime[ARGSIZE] = {'\0'};
    char *before = (char *)malloc(sizeof(char) * LINE_LENGTH);
    char *after = (char *)malloc(sizeof(char) * LINE_LENGTH);
    strcpy(before, "\0");
    strcpy(after, "\0");
    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);

    if (!read_before(linez, n, file_name, before, holdprime))
    {
        printf("error: line %d does not have character %d", linez, n);
        return;
    }
    parse_text(message);
    printf("kit");
    read_after(linez, n, file_name, after, holdprime, file_ad);

    FILE *ff = fopen(file_name, "w");
    printf("%s%s%s", before, message, after);
    fputs(before, ff);
    fputs(message, ff);
    fputs(after, ff);
    fclose(ff);
}

void cat(char *file_name)
{
    char *hold = (char *)malloc(sizeof(char) * FILESIZE);
    memset(hold, 0, strlen(hold));
    put_content_in_string(file_name, hold);
    printf("%s", hold);
}

void remove_str(char *file_name, int linez, int n, int size, char dir)
{
    char c;
    char holdprime[ARGSIZE] = {'\0'};

    char *before = (char *)malloc(sizeof(char) * LINE_LENGTH);
    char *ultimate_before = (char *)malloc(sizeof(char) * LINE_LENGTH);

    char *after = (char *)malloc(sizeof(char) * LINE_LENGTH);
    char *ultimate_after = (char *)malloc(sizeof(char) * LINE_LENGTH);

    memset(before, 0, strlen(before));
    memset(after, 0, strlen(after));

    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);
    if (!read_before(linez, n, file_name, before, holdprime))
    {
        printf("error: line %d does not have character %d", linez, n);
        return;
    }
    read_after(linez, n, file_name, after, holdprime, file_ad);

    memset(ultimate_after, 0, strlen(after));
    memset(ultimate_before, 0, strlen(before));

    if (dir == 'f')
    {
        int finish = strlen(after);

        for (int i = size; i < finish; i++)
        {
            ultimate_after[i - size] = after[i];
            if (!after[i + 1] || after[i + 1] == EOF)
            {
                ultimate_after[i + 1] = EOF;
                break;
            }
        }
        strcpy(ultimate_before, before);
    }

    else if (dir == 'b')
    {
        int finish = strlen(before);
        for (int i = 0; i < finish - size; i++)
        {
            ultimate_before[i] = before[i];
        }
        strcpy(ultimate_after, after);
    }
    else
    {
        printf("invalid direction");
        return;
    }
    // printf("%s%s", ultimate_before, ultimate_after);
    fclose(file_ad);
    fopen(file_name, "w");
    fputs(ultimate_before, file_ad);
    fputs(ultimate_after, file_ad);
    fclose(file_ad);
}

void copy_for_real(char *target)
{
    OpenClipboard(0);
    EmptyClipboard();
    size_t lol = 1 + strlen(target);
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, lol);
    memcpy(GlobalLock(h), target, lol);
    GlobalUnlock(h);
    SetClipboardData(CF_TEXT, h);
    CloseClipboard();
}

void paste_for_real(char *target)
{
    // Try opening the clipboard
    if (!OpenClipboard(NULL))
        printf("error\n");

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == NULL)
        printf("error\n");

    // Lock the handle to get the actual text pointer
    char *pszText = (char *)(GlobalLock(hData));
    if (pszText == NULL)
        printf("error\n");

    // Save text in a string class instance
    strcpy(target, pszText);

    // Release the lock
    GlobalUnlock(hData);

    // Release the clipboard
    CloseClipboard();
}

void copy_clipboard(char *file_name, int linez, int n, int size, char dir)
{
    char c;
    char holdprime[ARGSIZE] = {'\0'};
    char clipz[ARGSIZE] = {'\0'};

    char *before = (char *)malloc(sizeof(char) * LINE_LENGTH);
    char *after = (char *)malloc(sizeof(char) * LINE_LENGTH);

    memset(before, 0, strlen(before));
    memset(after, 0, strlen(after));

    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);
    if (!read_before(linez, n, file_name, before, holdprime))
    {
        printf("error: line %d does not have character %d", linez, n);
        return;
    }
    read_after(linez, n, file_name, after, holdprime, file_ad);

    if (dir == 'f')
    {
        for (int i = 0; i < size; i++)
        {
            clipz[i] = after[i];
            if (!after[i + 1] || after[i + 1] == EOF)
            {
                clipz[i + 1] = EOF;
                break;
            }
        }
    }
    else if (dir == 'b')
    {
        int finish = strlen(before);
        for (int i = finish; i > finish - size; i--)
        {
            clipz[finish - i] = before[i - 1];
        }

        int i0 = strlen(clipz);
        char hold[ARGSIZE] = {'\0'};
        strcpy(hold, clipz);
        memset(clipz, 0, strlen(clipz));
        for (int i = 0; i < i0; i++)
        {
            clipz[i] = hold[i0 - i - 1];
        }
    }
    else
    {
        printf("invalid direction");
        return;
    }
    copy_for_real(clipz);
}

void cut_to_clipboard(char *file_name, int linez, int n, int size, char dir)
{
    copy_clipboard(file_name, linez, n, size, dir);
    remove_str(file_name, linez, n, size, dir);
}

void paste_from_clipboard(char *file_name, int linez, int n)
{
    char clipz[ARGSIZE] = {'\0'};
    paste_for_real(clipz);
    insert_file(clipz, linez, n, file_name);
}

void put_content_in_string(char *file_name, char *hold)
{
    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    int i = 0;
    char c;
    while (1)
    {
        c = fgetc(file_ad);
        if (c == EOF)
            break;
        hold[i] = c;
        i++;
    }
    fclose(file_ad);
}

// int read_word(char *file_name, char *target)
// {
//     FILE *file_ad = fopen(file_name, "r");
//     if (!file_ad)
//     {
//         perror("fopen");
//         return;
//     }
//     int i = 0;
//     while (1)
//     {
//         target[i] = getc(file_ad);
//         if (target[i] == '\0' || target[i] == EOF || target[i] == ' ')
//         {
//             fclose(file_ad);
//             break;
//         }
//         i++;
//     }
//     return i - 1;
// }

void check_wildcard(char *text)
{
    int length = strlen(text);
    int check = 0;
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '*')
        {
            if (i == 0 || text[i - 1] != 92)
            {
                text[i] = '.';
            }
        }
    }
}

int check_match(int start, char *source, char *target, int n, int check)
{
    if ((int) target[check] == '.')
    {

        char end;
        if (strlen(target) - 1 == check)
            end = '\0';
        else
            end = target[check + 1];
        int shift = read_wildcard(source,end, start + n);
        // printf("%d",shift);
        if (!shift)
            return 0;
        else
        {
            // printf("kirrrr");
            n += shift;
        }
    }
    else if (target[check] != source[start + n])
        return 0;
    if (check == strlen(target) - 1)
        return n;
    else
        return check_match(start, source, target, n + 1, check + 1);
}
// b*
int read_wildcard(char *source, char end, int start)
{
    int i = 0;
    if (!end)
    {
        while (source[start + i] != EOF && source[start + i] != ' ' && source[start + i] != '\0'){
            i++;
        }
            return i;
    }
    // printf("kir");
    while (1)
    {
        if (source[start + i] == end)
            break;
        if (source[start + i] == EOF && source[start + i] == ' ' && source[start + i] == '\0')
        {
            return 0;
        }
        i++;
    }
    return i;
}


int find(char *file_name, char *pattern, int mode)
{

    char *hold = (char *)malloc(sizeof(char) * FILESIZE);
    memset(hold, 0, strlen(hold));
    int length = strlen(pattern), i = 0;

    check_wildcard(pattern);

    parse_text(pattern);
    put_content_in_string(file_name, hold);

    int i0 = strlen(hold);
    int ans;

    // printf("%c\n",pattern[0]);
    for (int i = 0; i < i0; i++)
    {
        ans = check_match(i, hold, pattern, 0, 0);
        if (ans)
        {
            return i;
        }
    }
    return -1;
}
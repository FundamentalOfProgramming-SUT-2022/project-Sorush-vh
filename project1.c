#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

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
char *greps[20];
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
int find(char *file_name, char *pattern, int all, int count, int at, int byword);
void put_content_in_string(char *file_name, char *hold);
int check_match(int start, char *source, char *target, int n, int check);
void check_wildcard(char *text);
int read_wildcard(char *source, char end, int start);
int word_count(int index, char *source);
void replace(char *file_name, char *pattern, char *alt, int at, int all);
int find_line(char *data, int char_num);
int find_char(char *data, int char_num);
void grep(char *pattern, int c, int l, int num);
void put_line_in_string(int n, char *store, char *file_name);
void set_greps();
void indent(char *file_name);
int find_whitespace_size(int char_num, char *temp);
int find_starting_whitespaces(char* temp);
void compare_files(char* file1, char* file2);
int checklines(char* hold1,char* hold2);
void telltree(char* dir_address,int tree);
void insert_str(char *message, int start, char *makan);
void remove_empty_stuff(char* hold);
void add_indentation(char* hold);
int closing_line(int char_num, char* hold);
// edite insert

int main()
{
    set_greps();
    struct orders *firstOrd = (struct orders *)malloc(sizeof(struct orders));
    firstOrd->arg = (char *)malloc(sizeof(char) * ARGSIZE);
    firstOrd->full_input = (char *)malloc(sizeof(char) * (ARGSIZE + 2 * ORDERSIZE));
    firstOrd->order_body = (char *)malloc(sizeof(char) * ORDERSIZE);
    firstOrd->order_head = (char *)malloc(sizeof(char) * ORDERSIZE);
    firstOrd->prev = 0;
    firstOrd->num = 0;

    char *dastan = (char *)malloc(sizeof(char) * 100);
    char *lol = (char *)malloc(sizeof(char) * 100);
    gets(dastan);
    strcpy(lol, "kir");
    strcpy(greps[0], "root/file1.txt");
    strcpy(greps[1], "root/file2.txt");
    // remove_str("root/file1.txt",2,0,3,'f');
    // rewind(file_ad);
    //  insert_file(lol,2, 0, "root/file1.txt");

telltree("root",1);

    // cut_to_clipboard("root/file1.txt", 2, 10, 10, 'b');
    // FILE* add=make_directory("/root/filezz.txt");

    return 0;
}
// debuge find:    printf("%d",find("root/file1.txt",dastan,0,0,2,0)); bayad tuye at bealave un dastane beshe

void reset_greps()
{
    for (int i = 0; i < 20; i++)
    {
        greps[i] = 0;
    }
}

void set_greps()
{
    for (int i = 0; i < 20; i++)
    {
        greps[i] = (char *)malloc(sizeof(char) * ARGSIZE);
    }
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
    while (1)
    {
        fgets(hold, LINE_LENGTH, file_ad);
        strcat(makan, hold);
        if (feof(file_ad))
            break;
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
    read_after(linez, n, file_name, after, holdprime, file_ad);

    FILE *ff = fopen(file_name, "w");
    fputs(before, ff);
    fputs(message, ff);
    fputs(after, ff);
    fclose(ff);
}

void insert_str(char *message, int start, char *makan){
        char c;
    char before[FILESIZE]={0};
    char after[FILESIZE]={0};
    memset(before,0,strlen(before));
    memset(after,0,strlen(after));

        for (int i = 0; i < start; i++)
    {
        before[i]=makan[i];
    }

    int i0=strlen(makan);

        for (int i = start; i <i0; i++)
    {
        after[i-start]=makan[i];
    }
    memset(makan,0,strlen(makan));
    strcat(makan,before);
    strcat(makan,message);
    strcat(makan,after);
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
    memset(hold,0,strlen(hold));
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
        if (c == EOF || c=='\0')
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
                text[i] = 5;
            }
        }
    }
}

int check_match(int start, char *source, char *target, int n, int check)
{
    if ((int)target[check] == 5)
    {
        char end;
        if (strlen(target) - 1 == check)
            end = '\0';
        else
            end = target[check + 1];
        int shift = read_wildcard(source, end, start + n);

        if (!shift)
            return 0;
        else
        {
            n += shift;
        }
    }
    else if (target[check] != source[start + n])
        return 0;
    if (check == strlen(target) - 1)
        return n + 1;
    else
        return check_match(start, source, target, n + 1, check + 1);
}

// ino dorsost konii
int read_wildcard(char *source, char end, int start)
{
    int i = 0;
    if (!end)
    {
        // ino dorsost konii
        while (source[start + i] != EOF && source[start + i] != ' ' && source[start + i] != '\0')
        {
            i++;
        }
        return i;
    }
    while (1)
    {
        // dorost kardane space be sharte space nabudan
        if (source[start + i] == EOF || source[start + i] == ' ' || source[start + i] == '\0' || source[start + i] == '\n')
        {
            return 0;
        }
        if (source[start + i + 1] == end)
        {
            return i;
        }
        i++;
    }
}

int word_count(int index, char *source)
{
    int count = 0;
    int skip = 0;
    for (int i = 0; i < index; i++)
    {
        if (source[i] == ' ')
        {
            count++;
            while (1)
            {
                if (source[i + 1 + skip] == ' ')
                    skip++;
                else
                    break;
            }
            i += skip;
            skip = 0;
        }
    }
    return count;
}

// age naresid -1
int find(char *file_name, char *pattern, int all, int count, int at, int byword)
{
    // modez: count at byword all
    // count : tedad
    // all: hameash print she(ya tu 0 ya tu byword)
    // at: mige ru chandomi
    // byword: jaye char, kalame mige

    // se ta ba ham nist
    // all va at nist
    //
    char *hold = (char *)malloc(sizeof(char) * FILESIZE);
    memset(hold, 0, strlen(hold));
    int length = strlen(pattern), i = 0;

    check_wildcard(pattern);

    parse_text(pattern);
    put_content_in_string(file_name, hold);

    int i0 = strlen(hold);
    int ans;
    int countt = 0;
    // printf("%c\n",pattern[0]);
    // al va at
    // count ba hame
    for (int i = 0; i < i0; i++)
    {
        ans = check_match(i, hold, pattern, 0, 0);
        if (ans)
        {
            if (!all && count && !at && !byword)
            {
                countt++;
            }
            else if (!count)
            {
                if (!all && !at && !byword)
                    return i;
                else if (!all && !at && byword)
                    return word_count(i, hold);
                else if (all && !at && !byword)
                    printf("%d, ", i);
                else if (all && !at && byword)
                    printf("%d, ", word_count(i, hold));
                else if (!all && byword && at)
                {
                    countt++;
                    if (countt == at)
                        return word_count(i, hold);
                }
                else if (!all && !byword && at)
                {
                    countt++;
                    if (countt == at)
                        return i;
                }
                else
                {
                    printf("invalid mode\n");
                    return -1;
                }
            }
        }
    }
    if (!all && count && !at && !byword)
        return countt;
    return -1;
}

int find_line(char *data, int char_num)
{
    int count = 1;
    for (int i = 0; i < char_num; i++)
    {
        if (data[i] == '\n')
            count++;
    }
    return count;
}

int find_char(char *data, int char_num)
{
    int i = 0;
    int j = char_num;
    // 12345lol
    while (1)
    {
        if (data[j - 1] == '\n' || j == 0)
            return i;
        i++;
        j--;
    }
}

void replace(char *file_name, char *pattern, char *alt, int at, int all)
{
    if (at && all)
    {
        printf("invalid arguement format\n");
        return;
    }
    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);
    char *hold = (char *)malloc(sizeof(char) * FILESIZE);
    memset(hold, 0, strlen(hold));
    int length = strlen(pattern), i = 0;

    check_wildcard(pattern);

    parse_text(pattern);
    put_content_in_string(file_name, hold);

    int i0 = strlen(hold);
    int ans, line, char_num;
    int countt = 0;
    // printf("%c\n",pattern[0]);
    // al va at
    // count ba hame
    for (int i = 0; i < i0; i++)
    {
        ans = check_match(i, hold, pattern, 0, 0);
        if (ans)
        {
            if (!at && !all)
            {
                line = find_line(hold, i);
                char_num = find_char(hold, i);
                printf("%d", char_num);
                rewind(file_ad);
                remove_str(file_name, line, char_num, ans, 'f');
                rewind(file_ad);
                insert_file(alt, line, char_num, file_name);
                return;
            }
            else if (all)
            {
                line = find_line(hold, i);
                char_num = find_char(hold, i);
                rewind(file_ad);
                remove_str(file_name, line, char_num, ans, 'f');
                rewind(file_ad);
                insert_file(alt, line, char_num, file_name);
            }
            else
            {
                countt++;
                if (countt == at)
                {
                    line = find_line(hold, i);
                    char_num = find_char(hold, i);
                    printf("%d\n%d", char_num, ans);
                    rewind(file_ad);
                    remove_str(file_name, line, char_num, ans, 'f');
                    rewind(file_ad);
                    insert_file(alt, line, char_num, file_name);
                    return;
                }
            }
        }
    }
    if (at && countt < at)
        printf("error: invalid arguement\n");
}

void put_line_in_string(int n, char *store, char *file_name)
{
    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);
    rewind(file_ad);
    fopen(file_name, "r");
    for (int i = 1; i < n + 1; i++)
    {
        memset(store, 0, strlen(store));
        fgets(store, LINE_LENGTH, file_ad);
    }
    fclose(file_ad);
}

// star ro dorost kone
void grep(char *pattern, int c, int l, int num)
{
    int lol = 1;
    char *file_name = (char *)malloc(sizeof(char) * ARGSIZE);
    char *hold = (char *)malloc(sizeof(char) * FILESIZE);
    char *temp = (char *)malloc(sizeof(char) * LINE_LENGTH);
    int countt = 0;
    for (int i = 0; i < num; i++)
    {
        memset(file_name, 0, strlen(file_name));
        file_name = greps[i];

        memset(hold, 0, strlen(hold));
        int length = strlen(pattern), i = 0;

        check_wildcard(pattern);

        parse_text(pattern);
        put_content_in_string(file_name, hold);

        int i0 = strlen(hold);
        int ans, line, char_num;

        for (int i = 0; i < i0; i++)
        {

            ans = check_match(i, hold, pattern, 0, 0);
            if (ans)
            {

                if (!c && !l)
                {
                    line = find_line(hold, i);
                    put_line_in_string(line, temp, file_name);
                    printf("%s:%s", file_name, temp);
                    char_num = find_char(hold, i);
                    i += (strlen(temp) - char_num);
                    memset(temp, 0, strlen(temp));
                }
                if (c)
                {
                    countt++;
                    line = find_line(hold, i);
                    put_line_in_string(line, temp, file_name);
                    char_num = find_char(hold, i);
                    i += (strlen(temp) - char_num);
                    memset(temp, 0, strlen(temp));
                }
                if (l && lol)
                {
                    printf("%s\n", file_name);
                    lol = 0;
                }
            }
        }
        lol = 1;
    }
    if (c)
        printf("%d\n", countt);
    reset_greps();
}

int find_whitespace_size(int char_num, char *temp)
{
    int j = 0;
    int i = char_num - 1;
    while (1)
    {
        if (i < 0 || temp[i] != ' ')
            break;
        i--;
        j++;
    }
    return j;
}

int find_starting_whitespaces(char* temp){
    int i=0;
    while (1)
    {
        if(temp[i] !=' ') return i;
        i++;
    }
}

void remove_empty_stuff(char* hold){
    char temp[FILESIZE]={0};
    int i0=strlen(hold);
    int check=0;
    int j=0;
    for (int i = 0; i < i0; i++)
    {
        if(hold[i] !='\n'&& hold[i] !=' ') {
            check=1; 
            temp[j]=hold[i];
            j++;
        }
        else if(hold[i]==' '&& check){
            temp[j]=hold[i];
            j++;
        }
        else if(hold[i]=='\n'&& check){
                temp[j]=hold[i];
                j++;
                check=0;
        }
    }
    memset(hold,0,strlen(hold));
    strcpy(hold,temp);
    
}

void add_indentation(char* hold){
    char temp[FILESIZE]={0};
    char Stab[5] = "    ";
    int openz=0,j=0,check=0;
    int i0=strlen(hold);
    int real_shift;

    for (int i = 0; i < i0; i++,j++)
    {
        temp[j]=hold[i];
        if(hold[i]=='{') openz++;
        if(hold[i]=='}') openz--;
        if(hold[i]=='\n') {
            if(closing_line(i+1,hold)) real_shift=openz-1;
            else real_shift=openz;
            if(openz>0) for (int i = 0; i < real_shift; i++)
            {
                insert_str(Stab,j+1,temp);
                j+=4;
            }
            
        }
        
    }
    memset(hold,0,i0);
    strcpy(hold,temp);
}

int closing_line(int char_num, char* hold){
    int i=0;
    while (1)
    {
        if(hold[char_num+i]=='}') return 1;
        if(hold[char_num+i]=='\n' || hold[char_num+i]=='\0' || hold[char_num+i]==EOF) return 0;
        i++;
    }
}

void indent(char *file_name)
{
    FILE *file_ad = fopen(file_name, "r");
    if (!file_ad)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad);

    char hold[FILESIZE]={0};
    put_content_in_string(file_name, hold);

    char Sinter[5] = "\n";
    char Sspace[5] = " ";

    int i=0;
    while (1)
    {
        if(hold[i]=='\0'|| hold[i]==EOF) break;
        if( hold[i]=='}') {
            insert_str(Sinter,i+1,hold);
            insert_str(Sinter,i,hold);
            i++;
        }
        if(hold[i]=='{') {
            insert_str(Sinter,i+1,hold);
            insert_str(Sspace,i,hold);
            i++;
        }
        i++;
    }
        remove_empty_stuff(hold);
        add_indentation(hold);
        rewind(file_ad);
fopen(file_name,"w");
fputs(hold,file_ad);
fclose(file_ad);
}

int checklines(char* hold1,char* hold2){
if(strlen(hold1) != strlen(hold2) ) return 0;
int i0=strlen(hold2);
for (int i = 0; i < i0; i++)
{
    if(hold1[i]!=hold2[i]) return 0;
}
return 1;
}

void compare_files(char* file1, char* file2){
    char *hold1 = (char *)malloc(sizeof(char) * FILESIZE);
    char *hold2 = (char *)malloc(sizeof(char) * FILESIZE);

        FILE *file_ad1 = fopen(file1, "r");
    if (!file_ad1)
    {
        perror("fopen");
        return;
    }
        FILE *file_ad2 = fopen(file2, "r");
    if (!file_ad2)
    {
        perror("fopen");
        return;
    }
    fclose(file_ad1);
    fclose(file_ad2);
    put_content_in_string(file1,hold1);
    put_content_in_string(file2,hold2);
    rewind(file_ad1);
    rewind(file_ad2);

    int line1=find_line(hold1,strlen(hold1)-1);
    int line2=find_line(hold2,strlen(hold2)-1);

    int is1less= line1<line2;
    int limit;
    if(is1less) limit=line1;
    else limit=line2;

        memset(hold1,0,strlen(hold1));
        memset(hold2,0,strlen(hold2));

    for (int i = 1; i < limit+1; i++)
    {
            rewind(file_ad1);
            rewind(file_ad2);
        put_line_in_string(i,hold1,file1);
        put_line_in_string(i,hold2,file2);

            if(hold2[strlen(hold2)-1] !='\n') strcat(hold2,"\n");
            if(hold1[strlen(hold1)-1] !='\n') strcat(hold1,"\n");
        if(!checklines(hold1,hold2)){
            printf("lol");
            printf("==================line %d:==================\n%s:%s%s:%s\n",i,file1,hold1,file2,hold2);
        }
        memset(hold1,0,strlen(hold1));
        memset(hold2,0,strlen(hold2));
    }
    int rest;
    if(is1less) rest=line2-limit;
    else rest=line1-limit;

printf("================== #%d to #%d: ==================\n",limit+1,( (line2>line1)*(line2)+(line1>line2)*line1 ) );
    for (int i = limit+1; i < (line2>line1)*(line2)+(line1>line2)*line1+1 ; i++)
    {
        rewind(file_ad1);
        rewind(file_ad2);
            if(is1less) {
                        put_line_in_string(i,hold2,file2);
                        printf("%s",hold2);
                        memset(hold2,0,strlen(hold2));
            }
            else {
                        put_line_in_string(i,hold1,file1);
                        printf("%s",hold1);
                        memset(hold1,0,strlen(hold1));                
            }
    }
}

void telltree(char* dir_address, int tree){
DIR* directory;
struct dirent *entry;
directory=opendir("./root");
if(directory==NULL){
    return 0;
}
while( (entry=readdir(directory))!=NULL){
    printf("%s\n",entry->d_name);
}
closedir(directory);
}
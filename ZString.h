/*
    #define ZSTRING_IMPLEMENTATION
*/

#ifndef ZSTRING_H
#define ZSTRING_H

#include <stdio.h>      // vsprintf()
#include <ctype.h>      // toupper(), tolower()
#include <stdlib.h>     // malloc(), free()
#include <string.h>     // strlen(), strstr(), strncmp(), memcpy()
#include <stdarg.h>     // va_list(), va_start(), va_end()
#include <stdbool.h>    // true, false

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// ZString Function Declarations
//----------------------------------------------------------------------------

// --- Location Index --- //
int string_find(char *str, char *substr);
int string_find_nth(char *str, char *substr, unsigned int nth);

// --- Counting --- //
unsigned int string_count(char *str, char *substr);
unsigned int string_count_overlap(char *str, char *substr);

unsigned int string_streak(char *str, char *substr);

// --- Booleans --- //
bool string_contains(char *str, char *substr);
bool string_starts_with(char *str, char *substr);
bool string_ends_with(char *str, char *substr);

//----------------------------------------------------------------------------
// Functions that require "free()"
//----------------------------------------------------------------------------

// --- Formatting --- //
char *string_format(char *str, ...);

// --- Slicing --- //
char *string_slice(char *str, unsigned int start, unsigned int end);

// --- Cutting --- //
char *string_cut_left(char *str, unsigned int amount);
char *string_cut_right(char *str, unsigned int amount);

// --- Splitting ---//
char **string_split(char *str, char *delimiter);

// --- Trimming --- //
char *string_trim_left(char *str, char *substr);
char *string_trim_right(char *str, char *substr);

// --- Removing --- //
char *string_remove(char *str, char *substr);
char *string_remove_all(char *str, char *substr);

// --- Shifting --- //
char *string_shift_left(char *str, unsigned int amount);
char *string_shift_right(char *str, unsigned int amount);

// --- Capitalizing --- //
char *string_upper(char *str);
char *string_lower(char *str);

// --- Replacing --- //
char *string_replace(char *str, char *substr, char *replacement);
char *string_replace_all(char *str, char *substr, char *replacement);

// --- Inserting --- //
char *string_insert(char *str, char *substr, unsigned int index);

// --- Reversing --- //
char *string_reverse(char *str);

// --- Getting --- //
char *string_before(char *str, char *substr);
char *string_after(char *str, char *substr);
char *string_between(char *str, char *a, char *b);

#endif // ZSTRING_H

//----------------------------------------------------------------------------
// ZString Function Definitions
//----------------------------------------------------------------------------

#ifdef ZSTRING_IMPLEMENTATION

//----------------|
// Location Index |
//----------------|

/*
int string_find(char *str, char *substr)

returns:
    > position of the first occurence of <substr> in <str>
    > -1 if <substr> wasn't found

example:
    > string_find("Foo Bar Foo Bar", "Foo") -> 0
                   ^
    > string_find("Bar Foo Bar Foo", "Foo") -> 4
                       ^
*/
int string_find(char *str, char *substr) 
{
    if (!str || !substr) {return -1;}

    size_t length_str = strlen(str);
    int pos = -1;

    if (length_str > 0)
    {
        const char *ptr = str;

        ptr = strstr(ptr, substr);
    
        if (ptr != NULL)
        {
            pos = ptr - str;
        }
    }

    return pos;
}

/*
int string_find_nth(char *str, char *substr, unsigned int count)

returns:
    > position of nth <count> occurence of <substr> in <str>
    > -1 if <substr> wasn't found
    > -1 if <str>, <substr> or <count> are invalid  

example:
    > string_find_nth("Foo Bar Foo Bar", "Foo", 2) -> 8
                               ^
*/
int string_find_nth(char *str, char *substr, unsigned int count)
{
    if (!str || !substr || count == 0) {return -1;}
    
    size_t length_str = strlen(str);
    int pos = -1;

    if (length_str > 0)
    {
        const char *ptr = str;

        for (int i = 0; i < count; ++i) 
        {
            ptr = strstr(ptr, substr);
    
            if (ptr == NULL)
            {
                break;
            } 

            pos = (ptr - str);
            ++ptr;
        }
    }

    return pos;
}

//----------|
// Counting |
//----------|

/*
unsigned int string_count(char *str, char *substr)

returns:
    > the amount of times <substr> occurs in <str>

example:
    > string_count("Fooo Foo", "oo") -> 2
                     ^    ^
*/
unsigned int string_count(char *str, char *substr)
{
    if (!str || !substr) {return 0;}

    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    if (length_str < length_sub || length_str == 0 || length_sub == 0) {return 0;}

    char *ptr = str;
    unsigned int count = 0;

    while (ptr = strstr(ptr, substr)) 
    {
        ptr += length_sub;
        ++count;
    }

    return count;
}

/*
unsigned int string_count_overlap(char *str, char *substr) 

returns:
    > the amount of times <substr> occurs in <str> with overlap

example:
    > string_count("Fooo Foo", "oo") -> 3
                     ^^   ^
*/
unsigned int string_count_overlap(char *str, char *substr)
{
    if (!str || !substr) {return 0;}

    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    if (length_str < length_sub || length_str == 0 || length_sub == 0) {return 0;}

    char *ptr = str;
    unsigned int count = 0;

    while (ptr = strstr(ptr, substr)) 
    {
        ++ptr;
        ++count;
    }

    return count;
}

/*
unsigned int string_streak(char *str, char *substr)

returns:
    > the amount of times the first occurence of 
      <substr> is repeated in a row in <str>

example:
    > string_streak_("Bar Foo Foo Bar Foo", "Foo ") -> 2
                          ^   ^
*/
unsigned int string_streak(char *str, char *substr)
{
    if (!str || !substr) {return 0;}

    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    if (length_str < length_sub || length_str == 0 || length_sub == 0) {return 0;}

    int pos = -1;
    unsigned int count = 0;

    const char *ptr = str;

    ptr = strstr(ptr, substr);

    if (ptr != NULL)
    {
        pos = ptr - str;
    }

    while (strncmp(str + pos, substr, length_sub) == 0)
    {
        ++count;
        pos += length_sub;
    }

    return count;
}

//----------|
// Booleans |
//----------|

/*
bool string_contains(char *str, char *substr)

returns:
    > true if <str> contains <substr>

example:
    > string_contains("Hello World", "Hello") -> true
*/
bool string_contains(char *str, char *substr)
{
    return (strstr(str, substr) != 0 ? true : false);
}

/*
bool string_starts_with(char *str, char *substr)

returns:
    > true if <str> starts with <substr>

example:
    > string_starts_with("Hello World", "Hello") -> true
    > string_starts_with("Hello World", "World") -> false
*/
bool string_starts_with(char *str, char *substr)
{
    return (strncmp(str, substr, strlen(substr)) == 0 ? true : false);
}

/*
bool string_ends_with(char *str, char *substr)

returns:
    > true if <str> ends with <substr>

example:
    > string_ends_with("Hello World", "Hello") -> false
    > string_ends_with("Hello World", "World") -> true
*/
bool string_ends_with(char *str, char *substr)
{
    return (strncmp(str + (strlen(str) - strlen(substr)), substr, strlen(substr)) == 0 ? true : false);
}

//------------|
// Formatting |
//------------|

/*
int string_format(char *str, ...)

returns:
    > <str> formatted with extra arguments
    > needs to be freed!
    > NULL if invalid <str>

example:
    > string_format("Hello %s", "World") -> "Hello World"
*/
char *string_format(char *str, ...) 
{
    if (!str) {return NULL;}

    va_list args;
    
    va_start(args, str);
    int length_buf = vsnprintf(NULL, 0, str, args);
    va_end(args);

    if (length_buf > 0)
    {
        ++length_buf;
        char *output = malloc(length_buf);

        va_start(args, str);
        vsnprintf(output, length_buf, str, args);
        va_end(args);

        output[length_buf] = '\0';
        return output;
    }

    return str;

}

//---------|
// Slicing |
//---------|

/*
char *string_slice(char *str, unsigned int start, unsigned int end)

returns:
    > <str> sliced from <start> to <end> (inclusive)
    > needs to be freed!
    > NULL if invalid <str>

example:
    > string_slice("Hello World", 0, 4) -> "Hello"
                    ^---^
*/
char *string_slice(char *str, unsigned int start, unsigned int end)
{
    size_t length_str = strlen(str);

    if (!str || start > end || length_str == 0 || length_str < start || length_str < end) {return NULL;}

    char *output = malloc((end - start) + 1);

    memcpy(output, str + start, (end - start) + 1);
    
    output[length_str] = '\0';
    return output;
}

//---------|
// Cutting |
//---------|

/*
char *string_cut_left(char *str, unsigned int amount)

returns:
    > <str> with <amount> sliced off from the left
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_cut_left("Hello World", 5) -> " World"
                       ----^
*/
char *string_cut_left(char *str, unsigned int amount)
{
    size_t length_str = strlen(str);

    if (!str || length_str < amount) {return NULL;}

    char *output = malloc((length_str + 1) - amount);

    memcpy(output, str + amount, length_str - amount);

    output[length_str] = '\0';

    return output;
}

/*
char *string_cut_right(char *str, unsigned int amount)

returns:
    > <str> with <amount> sliced off from the right
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_cut_right("Hello World", 5) -> "Hello "
                              ^----
*/
char *string_cut_right(char *str, unsigned int amount)
{
    size_t length_str = strlen(str);

    if (!str || length_str < amount) {return NULL;}

    char *output = malloc((length_str + 1) - amount);

    memcpy(output, str, length_str - amount);

    output[length_str] = '\0';

    return output;
}

/*
char *string_split(char *str, char *delimiter)

returns:
    > an array containing contents of <str> split by <delimiter>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_split("Hello World", " ") -> {"Hello", "World"}
*/

char **string_split(char *str, char *delimiter)
{
    if (!str || !delimiter) {return NULL;}

    size_t length_str = strlen(str);
    unsigned int count = 0;
    
    {
        size_t length_sub = strlen(delimiter);

        if (length_str < length_sub || length_str == 0 || length_sub == 0) {return NULL;}

        char *ptr = str;

        while (ptr = strstr(ptr, delimiter)) 
        {
            ptr += length_sub;
            ++count;
        }
    }

    char *copy = malloc(length_str + 1);
    memcpy(copy, str, length_str + 1);

    char **output = malloc(sizeof(char*) * (count + 1));

    if (count == 0) {output[0] = copy; return output;}

    int i = 0;
    char *token = strtok(copy, delimiter);

    while (token != NULL)
    {
        output[i++] = token;
        token = strtok(NULL, delimiter);
    }

    return output;
}

//----------|
// Trimming |
//----------|

/*
char *string_trim_left(char *str, char *substr)

returns:
    > <str> with <substr> trimmed from the left
    > NULL if invalid <str>

example:
    > string_trim_left("Hello World", "Hello ") -> "World"
                        ^-----
*/
char *string_trim_left(char *str, char *substr)
{
    if (!str)       {return NULL;}
    if (!substr)    {return str;}

    return strncmp(str, substr, strlen(substr)) == 0 ? str + strlen(substr) : str;

}

/* 
char *string_trim_right(char *str, char *substr)

returns:
    > <str> with <substr> trimmed from the right
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_trim_right("Hello World", " World") -> "Hello"
                              ^-----
*/
char *string_trim_right(char *str, char *substr)
{
    if (!str)       {return NULL;}
    if (!substr)    {return str;}
    
    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    if (strncmp(str + (length_str - length_sub), substr, length_sub) == 0)
    {
        size_t length_buf = length_str - length_sub;

        char *output = malloc(length_buf + 1);

        memcpy(output, str, length_buf);
        output[length_buf] = '\0';

        return output;
    }
    else
    {
        return str;
    }
}

//----------|
// Removing |
//----------|

/*
char *string_remove(char *str, char *substr)

returns:
    > <str> with first occurence of <substr> removed
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_remove("Hello There World", "There") -> "Hello  World"
                           ^---^
*/
char *string_remove(char *str, char *substr)
{
    if (!str)       {return NULL;}
    if (!substr)    {return str;}

    size_t length_str = strlen(str);

    // Find position of substr  
    int pos = -1;

    if (length_str > 0)
    {
        const char *ptr = str;

        ptr = strstr(ptr, substr);
    
        if (ptr != NULL)
        {
            pos = ptr - str;
        }
    }

    if (pos == -1) {return str;}

    // Remove substring
    size_t length_sub = strlen(substr);
    size_t length_buf = length_str - length_sub;

    char *output = malloc(length_buf + 1);

    memcpy(output, str, pos);
    memcpy(output + pos, str + pos + length_sub, length_buf);

    output[length_buf] = '\0';

    return output;
}

char *string_remove_all(char *str, char *substr)
{
    if (!str)       {return NULL;}
    if (!substr)    {return str;}

    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    unsigned int count = 0;
    
    {
        char *ptr = str;

        while (ptr = strstr(ptr, substr))
        {
            ptr += length_sub;
            ++count;
        }
    }

    if (count == 0) {return str;}

    size_t length_buf = length_str - (length_sub * count);

    int pos_str = 0;
    int pos_out = 0;
    char *output = malloc(length_buf);

    for (int i = 0; i < count; ++i)
    {
        int pos = -1;

        const char *ptr = str;

        for (int j = 0; j < i + 1; ++j) 
        {
            ptr = strstr(ptr, substr);
    
            if (ptr == NULL)
            {
                break;
            } 

            pos = (ptr - str);
            ++ptr;
        }

        int length_copy = pos - pos_str;
        
        memcpy(output + pos_out, str + pos_str, length_copy);
        pos_out += length_copy;
        pos_str += length_copy + length_sub;

        //++pos_out;
    }

    memcpy(output + pos_out, str + pos_str, length_str - pos_str);

    output[length_buf] = '\0';
    return output;
}

//----------|
// Shifting |
//----------|

/*
char *string_shift_left(char *str, unsigned int amount)

returns:
    > <str> shifted <amount> of letters to the left
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_shift_left("abcdefg", 3) -> "defgabc"
                         <<^
*/
char *string_shift_left(char *str, unsigned int amount)
{
    size_t length_str = strlen(str);

    amount = amount % length_str;

    if (!str)        {return NULL;}
    if (amount == 0) {return str;}

    char *output = malloc(length_str + 1);

    memcpy(output, str + amount, length_str - amount);
    memcpy(output + (length_str - amount), str, amount);

    output[length_str] = '\0';

    return output;
}

/*
char *string_shift_right(char *str, unsigned int amount)

returns:
    > <str> shifted <amount> of letters to the right
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_shift_right("abcdefg", 3) -> "efgabcd"
                              ^>>
*/
char *string_shift_right(char *str, unsigned int amount)
{
    size_t length_str = strlen(str);

    amount = amount % length_str;

    if (!str)        {return NULL;}
    if (amount == 0) {return str;}

    char *output = malloc(length_str + 1);

    memcpy(output, str + (length_str - amount), amount);
    memcpy(output + amount, str, length_str - amount);

    output[length_str] = '\0';

    return output;
}

//----------------|
// Capitalization |
//----------------|

/*
char *string_upper(char *str) 

returns:
    > <str> with upper case letters
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_upper("Hello World") -> "HELLO WORLD"
*/
char *string_upper(char *str)
{
    if (!str) {return NULL;}

    size_t length_str = strlen(str);
    char *output = malloc(length_str + 1);

    for (int i = 0; i < length_str; ++i)
    {
        if (str[i] != '\0') 
        {
            output[i] = (char)toupper(str[i]);
        }
    }

    output[length_str] = '\0';

    return output;
}

/*
char *string_lower(char *str)

returns:
    > <str> with lower case letters
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_lower("HELLO WORLD") -> "hello world"
*/
char *string_lower(char *str)
{
    if (!str) {return NULL;}

    size_t length_str = strlen(str);
    char *output = malloc(length_str + 1);

    for (int i = 0; i < length_str; ++i)
    {
        if (str[i] != '\0') 
        {
            output[i] = (char)tolower(str[i]);
        }
    }

    output[length_str] = '\0';

    return output;
}

//-----------|
// Replacing |
//-----------|

/*
char *string_replace(char *str, char *substr, char *replacement)

returns:
    > <str> with first occurence of <substr> replaced with <replacement>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_replace("Hello Hello World", "Hello", "Bye") -> "Bye Hello World"
                      ^---^
*/
char *string_replace(char *str, char *substr, char *replacement)
{
    if (!str || !substr)    {return NULL;}
    if (!replacement)       {return str;}

    size_t length_buf;
    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);
    size_t length_rep = strlen(replacement);

    if (length_str < length_sub || length_str == 0 || length_sub == 0) {return 0;}

    char *ptr = strstr(str, substr);

    if (ptr == NULL) 
    {
        return str;
    }

    int pos = (ptr - str);

    if (length_sub < length_rep)
    {
        length_buf = length_str + (length_rep - length_sub);
    }
    else
    {
        length_buf = length_str - (length_sub - length_rep);
    }

    char *output = malloc(length_buf);

    memcpy(output, str, pos);
    memcpy(output + pos, replacement, length_rep);
    memcpy(output + pos + length_rep, str + pos + length_sub, length_str - length_sub);

    output[length_buf] = '\0';

    return output;
}

/*
char *string_replace_all(char *str, char *substr, char *replacement)

returns:
    > <str> with every occurence of <substr> replaced with <replacement>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_replace_all("Hello Hello World", "Hello", "Bye") -> "Bye Bye World"
                          ^---^ ^---^
*/
char *string_replace_all(char *str, char *substr, char *replacement)
{
    if (!str || !substr)    {return NULL;}
    if (!replacement)       {return str;}

    size_t length_buf;
    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);
    size_t length_rep = strlen(replacement);

    if (length_str < length_sub || length_str == 0 || length_sub == 0) {return 0;}

    unsigned int count = 0;
    
    {
        char *ptr = str;

        while (ptr = strstr(ptr, substr)) 
        {
            ptr += length_sub;
            ++count;
        }
    }

    if (count == 0) {return str;}

    if (length_sub < length_rep)
    {
        length_buf = length_str + ((length_rep - length_sub) * count);
    }
    else
    {
        length_buf = length_str - ((length_sub - length_rep) * count); // Change (Don't make buffer unnecessarily large)
    }

    int pos_str = 0;
    int pos_out = 0;
    char *output = malloc(length_buf);

    for (int i = 0; i < count; ++i)
    {
        int pos = -1;

        const char *ptr = str;

        for (int j = 0; j < i + 1; ++j) 
        {
            ptr = strstr(ptr, substr);
    
            if (ptr == NULL)
            {
                break;
            } 

            pos = (ptr - str);
            ++ptr;
        }

        int length_copy = pos - pos_str;
        
        memcpy(output + pos_out, str + pos_str, length_copy);
        pos_out += length_copy;
        pos_str += length_copy;

        memcpy(output + pos_out, replacement, length_rep);
        pos_out += length_rep;
        pos_str += length_sub;
    }

    memcpy(output + pos_out, str + pos_str, length_str - pos_str);

    output[length_buf] = '\0';
    return output;
}

//-----------|
// Inserting |
//-----------|

/*
char *string_insert(char *str, char *substr, unsigned int index)

returns:
    > <str> with <substr> inserted at <index>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_insert("Hello World", "There ", 6) -> "Hello There World"
                           ^
*/
char *string_insert(char *str, char *substr, unsigned int index)
{
    if (!str)       {return NULL;}
    if (!substr)    {return str;}

    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);
    size_t length_buf = length_str + length_sub;

    char *output = malloc(length_buf + 1);

    memcpy(output, str, index);
    memcpy(output + index, substr, length_sub);
    memcpy(output + index + length_sub, str + index, length_str);

    output[length_buf] = '\0';

    return output;
}

//-----------|
// Reversing |
//-----------|

/*
char *string_reverse(char *str)

returns:
    > <str> reversed
    > NULL if invalid <str>
    > needs to be freed!

example:
    > string_reverse("Hello World") -> "dlroW olleH"
*/
char *string_reverse(char *str)
{
    if (!str) {return NULL;}

    size_t length_str = strlen(str);
    char *output = malloc(length_str + 1);

    {
        int j = length_str - 1;
        for (int i = 0; i < length_str; ++i)
        {
            output[i] = str[j];
            --j;
        }
    }

    output[length_str] = '\0';

    return output;
}

//---------|
// Getting |
//---------|

/*
char *string_before(char *str, char *substr)

returns:
    > returns the string before <substr> in <str>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_before("Hello There World", "There") -> "Hello "
*/
char *string_before(char *str, char *substr)
{
    if (!str || !substr) {return NULL;}
    
    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    int pos;

    if (length_str > 0 && length_sub > 0)
    {
        const char *ptr;

        ptr = str;
        ptr = strstr(ptr, substr);
    
        if (ptr != NULL)
        {
            pos = ptr - str;
        } 
        else {return NULL;}
    }

    char *output = malloc(pos + 1);

    memcpy(output, str, pos);
    
    output[pos] = '\0';

    return output;
}

/*
char *string_after(char *str, char *substr)

returns:
    > returns the string after <substr> in <str>
    > NULL if invalid <str> or <substr>
    > needs to be freed!

example:
    > string_after("Hello There World", "There") -> " World"
*/
char *string_after(char *str, char *substr)
{
    if (!str || !substr) {return NULL;}
    
    size_t length_str = strlen(str);
    size_t length_sub = strlen(substr);

    int pos;

    if (length_str > 0 && length_sub > 0)
    {
        const char *ptr;

        ptr = str;
        ptr = strstr(ptr, substr);
    
        if (ptr != NULL)
        {
            pos = ptr - str;
        } 
        else {return NULL;}
    }

    size_t length_buf = (length_str - length_sub) - pos;

    char *output = malloc(length_buf + 1);

    memcpy(output, str + pos + length_sub, length_buf);
    
    output[length_buf] = '\0';

    return output;
}

/*
char *string_between(char *str, char *a, char *b)

returns:
    > returns the string between <a> and <b> in <str>
    > NULL if invalid <str>, <a> or <b>
    > needs to be freed!

example:
    > string_between("Hello There World", "Hello", "World") -> " There "
*/
char *string_between(char *str, char *a, char *b)
{
    if (!str || !a || !b) {return NULL;}
    
    size_t length_str = strlen(str);
    size_t length_a = strlen(a);
    size_t length_b = strlen(b);

    int pos_a;
    int pos_b;

    if (length_str > 0 && length_a > 0 && length_b > 0)
    {
        const char *ptr;

        ptr = str;
        ptr = strstr(ptr, a);
    
        if (ptr != NULL)
        {
            pos_a = ptr - str;
        } 
        else {return NULL;}

        ptr = str;
        ptr = strstr(ptr, b);
    
        if (ptr != NULL)
        {
            pos_b = ptr - str;
        }
        else {return NULL;}
    }

    size_t length_buf = (pos_b - (pos_a + length_a));

    char *output = malloc(length_buf + 1);

    memcpy(output, str + (pos_a + length_a), length_buf);
    
    output[length_buf] = '\0';

    return output;
}

#ifdef __cplusplus
}
#endif

#endif // ZSTRING_IMPLEMENTATION

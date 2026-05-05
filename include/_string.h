#ifndef __STRING_H
#define __STRING_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_LENGTH 512

typedef struct _String {
    char* value;
    int length;
    int byteLength;
} String;

typedef enum {
    STR2NBR_OK,
    STR2NBR_OVERFLOW,
    STR2NBR_UNDERFLOW,
    STR2NBR_INCONVERTIBLE
} STR2NBR_STATUS;

/**
 * Allocate a string on the heap.
 * If the memory allocation failed for the String struct or the char*, returns NULL
 * Calculate also the UTF8 string length - Not reliable
 */
String* string_create(char* value);

/**
 * Get back the input from the user in a temporary buffer set by DEFAULT_BUFFER_LENGTH
 * Clear the stdin buffer also
 */
String* string_readLine();

/**
 * Free the String, will set to NULL after DESTRUCTION
 */
void string_free(String* string);

/**
 * It does what it says - if somehow the string is NULL will return -1.
 */
int string_getUTF8Length(const String* string);

/**
 * Inserts line breaks into a String by replacing whitespace characters with
 * newlines whenever a line exceeds a given maximum length.
 *
 * Iterates through the string and, once the character count reaches or exceeds
 * `maxLength`, replaces the next encountered whitespace character with '\n'.
 * Only characters for which `char_isMaybeUTF8()` returns true are counted,
 * so ASCII control bytes and continuation bytes are ignored in the length
 * calculation.
 *
 * @param string               The String to modify in-place. If NULL, the
 *                             function returns immediately.
 * 
 * @param maxLength            The maximum number of counted characters allowed
 *                             per line before a break is inserted. Values below
 *                             0 are clamped to 0.
 * 
 * @param out_longestLineLength  Output parameter set to the length of the
 *                             longest line produced (excluding the replaced
 *                             whitespace character). Can be NULL if you don't care.
 * 
 * @param out_lineBreaksCount  Output parameter set to the total number of line
 *                             breaks inserted. Can be NULL if you don't care.
 *
 *
 * @note The string is modified in-place; the original content is overwritten.
 * @note Line breaks are only inserted at whitespace boundaries, so a word
 *       longer than `maxLength` will not be broken and will exceed the limit.
 * @note The replaced whitespace character itself is not counted toward the
 *       reported longest line length.
 */
void string_addLineBreaks(String* string, int maxLength, int* out_longestLineLength, int* out_lineBreaksCount);

bool char_isMaybeUTF8(char c);
bool char_isWhitespace(char c);

STR2NBR_STATUS string_toInt(String* string, int* out);
STR2NBR_STATUS string_toLong(String* string, long* out);

char* string_getValue(const String* string);
int string_getLength(const String* string);
int string_getByteLength(const String* string);
void string_manageFailedMalloc(const String* string);

#endif
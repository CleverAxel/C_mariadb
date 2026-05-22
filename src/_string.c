#include "_string.h"
#include "conf.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
String* string_create(char* value) {
    if (value == NULL)
        return NULL;

    String* string = malloc(sizeof(String));
    if (string == NULL)
        return NULL;

    int byteSize = strlen(value);

    // +1 for the NULL character
    string->value = malloc((sizeof(char) * byteSize) + 1);

    if (string->value == NULL) {
        free(string);
        return NULL;
    }

    strcpy(string->value, value);
    string->length = string_getUTF8Length(string);
    string->byteLength = byteSize;

    return string;
}

String* string_readLine() {
    char buffer[DEFAULT_BUFFER_LENGTH] = { 0 };

    if (!fgets(buffer, DEFAULT_BUFFER_LENGTH, stdin)) {
        buffer[0] = '\0';
    }

    int length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }

    String* string = string_create(buffer);
    return string;
}

void string_free(String* string) {
    if (string == NULL)
        return;

    string->length = 0;
    string->byteLength = 0;

    if (string->value == NULL)
        return;

    free(string->value);
    string->value = NULL;

    free(string);
    string = NULL;
}

int string_getUTF8Length(const String* string) {
    if (string == NULL)
        return -1;

    if (string->value == NULL)
        return -1;

    char* s = string->value;
    int count = 0;
    while (*s) {
        count += char_isMaybeUTF8(*s);
        *s++;
    }

    return count;
}

void string_addLineBreaks(String* string, int maxLength, int* out_longestLineLength, int* out_lineBreaksCount) {
    if (string == NULL)
        return;

    if (maxLength < 0) {
        maxLength = 0;
    }

    char* s = string->value;

    int longestLineLength = 0;
    int lineBreaksCount = 0;
    int i = 0;
    int charCount = 0;
    while (*s) {
        if (char_isMaybeUTF8(*s)) {
            charCount++;
        }

        if (charCount >= maxLength) {
            if (char_isWhitespace(*s)) {
                string->value[i] = '\n';
                lineBreaksCount++;

                // -1 - We do not count the whitespace, it's being replaced by a line break.
                if (charCount - 1 > longestLineLength) {
                    longestLineLength = charCount - 1;
                }
                charCount = 0;
            }
        }

        i++;
        *s++;
    }

    // if the last sentence/word didn't got a break line and that it wasn't accounted for
    if (charCount > longestLineLength) {
        longestLineLength = charCount;
    }

    if (out_lineBreaksCount != NULL)
        *out_lineBreaksCount = lineBreaksCount;

    if (out_longestLineLength != NULL)
        *out_longestLineLength = longestLineLength;
}

bool char_isMaybeUTF8(char c) {
    return (c & 0xC0) != 0x80;
}

bool char_isWhitespace(char c) {
    return isspace(c);
}

STR2NBR_STATUS string_toInt(String* string, int* out) {
    if (string == NULL || string->length == 0) {
        return STR2NBR_INCONVERTIBLE;
    }

    char* end;
    errno = 0;
    long casted = strtol(string->value, &end, 10);

    if (casted > INT_MAX || (errno == ERANGE && casted == LONG_MAX))
        return STR2NBR_OVERFLOW;

    if (casted < INT_MIN || (errno == ERANGE) && casted == LONG_MIN)
        return STR2NBR_UNDERFLOW;

    if (*end != '\0')
        return STR2NBR_INCONVERTIBLE;

    *out = casted;
    return STR2NBR_OK;
}

STR2NBR_STATUS string_toLong(String* string, long* out) {
    if (string == NULL || string->length == 0) {
        return STR2NBR_INCONVERTIBLE;
    }

    char* end;
    errno = 0;
    long casted = strtol(string->value, &end, 10);

    if (casted > LLONG_MAX || (errno == ERANGE && casted == LLONG_MAX))
        return STR2NBR_OVERFLOW;

    if (casted < LLONG_MIN || (errno == ERANGE) && casted == LLONG_MIN)
        return STR2NBR_UNDERFLOW;

    if (*end != '\0')
        return STR2NBR_INCONVERTIBLE;

    *out = casted;
    return STR2NBR_OK;
}

char* string_getValue(const String* string) {
    return string->value;
}

int string_getLength(const String* string) {
    if (string == NULL)
        return -1;

    return string->length;
}

int string_getByteLength(const String* string) {
    if (string == NULL)
        return -1;

    return string->byteLength;
}

void string_manageFailedMalloc(const String* string) {
    if (string == NULL) {
#if DEBUG
        fprintf(stderr, "OKI, we couldn't allocate a string :(");
#endif
        exit(EXIT_FAILURE);
    }
}

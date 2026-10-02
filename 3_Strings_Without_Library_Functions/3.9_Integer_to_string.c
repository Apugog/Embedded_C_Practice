#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    ITOA_OK = 0,
    ITOA_ERR_NULL_PTR,
    ITOA_ERR_BUFFER_TOO_SMALL
} itoa_status_t;

/**
 * @brief Converts a signed 32-bit integer to a null-terminated string.
 * 
 * @param value     The 32-bit signed integer to convert.
 * @param buf       Destination buffer to store the resulting string.
 * @param buf_size  Total capacity of the destination buffer in bytes.
 * @return itoa_status_t:
 *         - ITOA_OK on success
 *         - ITOA_ERR_NULL_PTR if buf is NULL
 *         - ITOA_ERR_BUFFER_TOO_SMALL if the buffer is too small to fit the string + null terminator
 */
itoa_status_t int32_to_str(int32_t value, char *buf, size_t buf_size)
{
    if(buf == NULL){
        return ITOA_ERR_NULL_PTR;
    }

    if(buf_size < 2){
        return ITOA_ERR_BUFFER_TOO_SMALL;
    }

    if(value == 0){
        buf[0] = '0';
        buf[1] = '\0';

        return ITOA_OK;
    }

    uint32_t uval;

    bool is_negetive = false;

    if(value < 0){
        is_negetive = true;
        uval = 0U - (uint32_t)value;
    }
    else{
        uval = (uint32_t)value;
    }

    // Step 4: Extract digits in reverse order
    // TODO: Loop using modulo (% 10) and division (/ 10)
    // Be sure to check buffer boundary on each iteration!
    size_t idx = 0;

    while(uval){
        if(idx >= buf_size -1){
            buf[0] = '\0';
            return ITOA_ERR_BUFFER_TOO_SMALL;
        }
        buf[idx++] = '0' + (uval%10);
        uval /= 10;
    }

    if(is_negetive){
        if (idx >= buf_size - 1) {
            buf[0] = '\0';
            return ITOA_ERR_BUFFER_TOO_SMALL;
        }
        buf[idx++]='-';
    }
    
    buf[idx]='\0';

    size_t start = 0;
    size_t end = idx - 1;

    while(start<end){
        char temp = buf[start];
        buf[start] = buf[end];
        buf[end] = temp;
        start++;
        end--;
    }

    return ITOA_OK;
}

/**
 * @brief Standard Library Method (C99) using snprintf.
 * 
 * Recommended for interviews when string conversion is just a helper/utility step.
 * Demonstrates both speed of implementation AND awareness of buffer safety.
 */
itoa_status_t int32_to_str_stdlib(int32_t value, char *buf, size_t buf_size)
{
    if (buf == NULL) {
        return ITOA_ERR_NULL_PTR;
    }
    if (buf_size == 0) {
        return ITOA_ERR_BUFFER_TOO_SMALL;
    }

    /* snprintf safely writes at most buf_size bytes (including '\0').
     * Return value: number of characters (excluding '\0') that *would*
     * have been written if buffer was large enough.
     */
    int written = snprintf(buf, buf_size, "%d", value);

    /* Check for encoding error (< 0) or truncation (>= buf_size) */
    if (written < 0 || (size_t)written >= buf_size) {
        buf[0] = '\0';
        return ITOA_ERR_BUFFER_TOO_SMALL;
    }

    return ITOA_OK;
}

#ifndef TESTING
int main(void)
{
    char buf[16];

    /* 1. Custom in-place conversion (Embedded / from-scratch style) */
    if (int32_to_str(-1234, buf, sizeof(buf)) == ITOA_OK) {
        printf("[Custom itoa] Result: %s\n", buf);
    }

    /* 2. Standard library conversion (Interview utility style) */
    if (int32_to_str_stdlib(-1234, buf, sizeof(buf)) == ITOA_OK) {
        printf("[Stdlib snprintf] Result: %s\n", buf);
    }

    /* Quick 1-liner often used directly inside interview functions: */
    char quick_buf[16];
    snprintf(quick_buf, sizeof(quick_buf), "%d", 42);
    printf("[Quick 1-liner] Result: %s\n", quick_buf);

    return 0;
}
#endif

#ifndef C_INTEGERPARSE_H
#define	C_INTEGERPARSE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint64_t c_parse_unsigned_int64(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

uint32_t c_parse_unsigned_int32(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

uint16_t c_parse_unsigned_int16(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

uint8_t c_parse_unsigned_int8(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

unsigned long long c_parse_unsigned_long_long(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

unsigned long c_parse_unsigned_long(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

unsigned int c_parse_unsigned_int(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

unsigned short c_parse_unsigned_short(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

unsigned char c_parse_unsigned_char(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

int64_t c_parse_signed_int64(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

int32_t c_parse_signed_int32(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

int16_t c_parse_signed_int16(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

int8_t c_parse_signed_int8(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

signed long long c_parse_signed_long_long(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

signed long c_parse_signed_long(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

signed int c_parse_signed_int(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

signed short c_parse_signed_short(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);

signed char c_parse_signed_char(
    const char  *data_buffer,
    size_t      data_length,
    bool        *parse_error
);


#endif	/* C_INTEGERPARSE_H */

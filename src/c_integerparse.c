#include <c_integerparse.h>
#include <limits.h>

static const char *DIGITS_BINARY        = "01";
static const char *DIGITS_OCTAL         = "01234567";
static const char *DIGITS_DECIMAL       = "0123456789";
static const char *CHARS_HEXADECIMAL    = "AaBbCcDdEeFf";
static const char *SIGN_CHARS           = "-+";

static const uint8_t BASE_BINARY        = 2;
static const uint8_t BASE_OCTAL         = 8;
static const uint8_t BASE_DECIMAL       = 10;
static const uint8_t BASE_HEXADECIMAL   = 16;

static const uint8_t ALPHA_OFFSET_HEXADECIMAL   = 10;

static uint64_t c_parse_unsigned_int64_numeric_impl(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error,
    const uint64_t      max_value,
    const uint8_t       base,
    const char *const   digit_set
)
{
    uint64_t result = 0;
    bool error_flag = false;
    if (data_length >= 1)
    {
        const uint64_t max_value_base = max_value / base;
        size_t index = 0;
        while (index < data_length && result <= max_value_base)
        {
            result *= base;

            size_t digit_index = 0;
            while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
            {
                ++digit_index;
            }
            if (digit_set[digit_index] == '\0' || digit_index > max_value - result)
            {
                break;
            }
            result += digit_index;

            ++index;
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }

    return error_flag ? 0 : result;
}

static unsigned long long c_parse_unsigned_long_long_numeric_impl(
    const char *const           data_buffer,
    const size_t                data_length,
    bool *const                 parse_error,
    const unsigned long long    max_value,
    const uint8_t               base,
    const char *const           digit_set
)
{
    unsigned long long result = 0;
    bool error_flag = false;
    if (data_length >= 1)
    {
        const unsigned long long max_value_base = max_value / base;
        size_t index = 0;
        while (index < data_length && result <= max_value_base)
        {
            result *= base;

            size_t digit_index = 0;
            while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
            {
                ++digit_index;
            }
            if (digit_set[digit_index] == '\0' || digit_index > max_value - result)
            {
                break;
            }
            result += digit_index;

            ++index;
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }

    return error_flag ? 0 : result;
}

static int64_t c_parse_signed_int64_numeric_impl(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error,
    const int64_t       min_value,
    const int64_t       max_value,
    const uint8_t       base,
    const char *const   digit_set
)
{
    int64_t result = 0;
    bool error_flag = false;
    const int64_t min_negative_base = min_value / base;
    const int64_t max_positive_base = max_value / base;

    if (data_length >= 1)
    {
        size_t sign_index = 0;
        while (SIGN_CHARS[sign_index] != '\0' && SIGN_CHARS[sign_index] != data_buffer[0])
        {
            ++sign_index;
        }
        const bool positive_signed = sign_index != 0;
        const size_t numbers_offset = (SIGN_CHARS[sign_index] == '\0' ? 0 : 1);

        size_t index = numbers_offset;
        if (positive_signed)
        {
            while (index < data_length && result <= max_positive_base)
            {
                result *= base;

                int8_t digit_index = 0;
                while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (digit_set[digit_index] == '\0' || digit_index > max_value - result)
                {
                    break;
                }
                result += digit_index;

                ++index;
            }
        }
        else
        {
            while (index < data_length && result >= min_negative_base)
            {
                result *= base;

                // digit_index must be signed to enable multiplication with -1
                int8_t digit_index = 0;
                while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (digit_set[digit_index] == '\0' || (digit_index * -1) < min_value - result)
                {
                    break;
                }
                result -= digit_index;

                ++index;
            }
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }
    return error_flag ? 0 : result;
}

static signed long long c_parse_signed_long_long_numeric_impl(
    const char *const       data_buffer,
    const size_t            data_length,
    bool *const             parse_error,
    const signed long long  min_value,
    const signed long long  max_value,
    const uint8_t           base,
    const char *const       digit_set
)
{
    signed long long result = 0;
    bool error_flag = false;
    const signed long long min_negative_base = min_value / base;
    const signed long long max_positive_base = max_value / base;

    if (data_length >= 1)
    {
        size_t sign_index = 0;
        while (SIGN_CHARS[sign_index] != '\0' && SIGN_CHARS[sign_index] != data_buffer[0])
        {
            ++sign_index;
        }
        const bool positive_signed = sign_index != 0;
        const size_t numbers_offset = (SIGN_CHARS[sign_index] == '\0' ? 0 : 1);

        size_t index = numbers_offset;
        if (positive_signed)
        {
            while (index < data_length && result <= max_positive_base)
            {
                result *= base;

                int8_t digit_index = 0;
                while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (digit_set[digit_index] == '\0' || digit_index > max_value - result)
                {
                    break;
                }
                result += digit_index;

                ++index;
            }
        }
        else
        {
            while (index < data_length && result >= min_negative_base)
            {
                result *= base;

                // digit_index must be signed to enable multiplication with -1
                int8_t digit_index = 0;
                while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (digit_set[digit_index] == '\0' || (digit_index * -1) < min_value - result)
                {
                    break;
                }
                result -= digit_index;

                ++index;
            }
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }
    return error_flag ? 0 : result;
}

static uint64_t c_parse_unsigned_int64_alphanumeric_impl(
    const char* const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error,
    const uint64_t      max_value,
    const uint8_t       base,
    const char *const   digit_set,
    const char *const   alpha_set,
    const size_t        alpha_offset
)
{
    uint64_t result = 0;
    bool error_flag = false;
    if (data_length >= 1)
    {
        const uint64_t max_value_base = max_value / base;
        size_t index = 0;
        while (index < data_length && result <= max_value_base)
        {
            result *= base;

            // Scan for a matching numeric character
            size_t digit_index = 0;
            while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
            {
                ++digit_index;
            }
            if (digit_set[digit_index] == '\0')
            {
                // No numeric character matches, scan for a matching alpha character
                digit_index = 0;
                while (alpha_set[digit_index] != '\0' && alpha_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (alpha_set[digit_index] == '\0')
                {
                    break;
                }
                digit_index = (digit_index >> 1) + alpha_offset;
            }
            if (digit_index > max_value - result)
            {
                break;
            }
            result += digit_index;

            ++index;
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }

    return error_flag ? 0 : result;
}

static unsigned long long c_parse_unsigned_long_long_alphanumeric_impl(
    const char* const           data_buffer,
    const size_t                data_length,
    bool *const                 parse_error,
    const unsigned long long    max_value,
    const uint8_t               base,
    const char *const           digit_set,
    const char *const           alpha_set,
    const size_t                alpha_offset
)
{
    unsigned long long result = 0;
    bool error_flag = false;
    if (data_length >= 1)
    {
        const unsigned long long max_value_base = max_value / base;
        size_t index = 0;
        while (index < data_length && result <= max_value_base)
        {
            result *= base;

            // Scan for a matching numeric character
            size_t digit_index = 0;
            while (digit_set[digit_index] != '\0' && digit_set[digit_index] != data_buffer[index])
            {
                ++digit_index;
            }
            if (digit_set[digit_index] == '\0')
            {
                // No numeric character matches, scan for a matching alpha character
                digit_index = 0;
                while (alpha_set[digit_index] != '\0' && alpha_set[digit_index] != data_buffer[index])
                {
                    ++digit_index;
                }
                if (alpha_set[digit_index] == '\0')
                {
                    break;
                }
                digit_index = (digit_index >> 1) + alpha_offset;
            }
            if (digit_index > max_value - result)
            {
                break;
            }
            result += digit_index;

            ++index;
        }
        if (index < data_length)
        {
            error_flag = true;
        }
    }
    else
    {
        error_flag = true;
    }

    if (parse_error != NULL)
    {
        *parse_error = error_flag;
    }

    return error_flag ? 0 : result;
}



// Decimal number parsers
//

uint64_t c_parse_unsigned_int64(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT64_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

uint32_t c_parse_unsigned_int32(
    const char* const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint32_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT32_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

uint16_t c_parse_unsigned_int16(
    const char* const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint16_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT16_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

uint8_t c_parse_unsigned_int8(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint8_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT8_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

unsigned long long c_parse_unsigned_long_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULLONG_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

unsigned long c_parse_unsigned_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned long) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULONG_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

unsigned int c_parse_unsigned_int(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned int) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

unsigned short c_parse_unsigned_short(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned short) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        USHRT_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

unsigned char c_parse_unsigned_char(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned char) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UCHAR_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

int64_t c_parse_signed_int64(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_signed_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        INT64_MIN,
        INT64_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

int32_t c_parse_signed_int32(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (int32_t) c_parse_signed_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        INT32_MIN,
        INT32_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

int16_t c_parse_signed_int16(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (int16_t) c_parse_signed_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        INT16_MIN,
        INT16_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

int8_t c_parse_signed_int8(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (int8_t) c_parse_signed_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        INT8_MIN,
        INT8_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

signed long long c_parse_signed_long_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_signed_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        LLONG_MIN,
        LLONG_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

signed long c_parse_signed_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (signed long) c_parse_signed_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        LONG_MIN,
        LONG_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

signed int c_parse_signed_int(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (signed int) c_parse_signed_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        INT_MIN,
        INT_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

signed short c_parse_signed_short(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (signed short) c_parse_signed_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        SHRT_MIN,
        SHRT_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}

signed char c_parse_signed_char(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (signed char) c_parse_signed_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        CHAR_MIN,
        CHAR_MAX,
        BASE_DECIMAL,
        DIGITS_DECIMAL
    );
}



// Binary number parsers
//

uint64_t c_parse_binary_64(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint64_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT64_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

uint32_t c_parse_binary_32(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint32_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT32_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

uint16_t c_parse_binary_16(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint16_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT16_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

uint8_t c_parse_binary_8(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint8_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT8_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

unsigned long long c_parse_binary_unsigned_long_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned long long) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULLONG_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

unsigned long c_parse_binary_unsigned_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned long) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULONG_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

unsigned int c_parse_binary_unsigned_int(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned int) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

unsigned short c_parse_binary_unsigned_short(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned short) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        USHRT_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}

unsigned char c_parse_binary_unsigned_char(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned char) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UCHAR_MAX,
        BASE_BINARY,
        DIGITS_BINARY
    );
}



// Octal number parsers
//

uint64_t c_parse_octal_64(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT64_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

uint32_t c_parse_octal_32(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint32_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT32_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

uint16_t c_parse_octal_16(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint16_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT16_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

uint8_t c_parse_octal_8(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint8_t) c_parse_unsigned_int64_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT8_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

unsigned long long c_parse_octal_unsigned_long_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULLONG_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

unsigned long c_parse_octal_unsigned_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned long) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULONG_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

unsigned int c_parse_octal_unsigned_int(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned int) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

unsigned short c_parse_octal_unsigned_short(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned short) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        USHRT_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}

unsigned char c_parse_octal_unsigned_char(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned char) c_parse_unsigned_long_long_numeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UCHAR_MAX,
        BASE_OCTAL,
        DIGITS_OCTAL
    );
}



// Hexadecimal number parsers
//

uint64_t c_parse_hexadecimal_64(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_int64_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT64_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

uint32_t c_parse_hexadecimal_32(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint32_t) c_parse_unsigned_int64_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT32_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

uint16_t c_parse_hexadecimal_16(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint16_t) c_parse_unsigned_int64_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT16_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

uint8_t c_parse_hexadecimal_8(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (uint8_t) c_parse_unsigned_int64_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT8_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

unsigned long long c_parse_hexadecimal_unsigned_long_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return c_parse_unsigned_long_long_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULLONG_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

unsigned long c_parse_hexadecimal_unsigned_long(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned long) c_parse_unsigned_long_long_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        ULONG_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

unsigned int c_parse_hexadecimal_unsigned_int(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned int) c_parse_unsigned_long_long_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UINT_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

unsigned short c_parse_hexadecimal_unsigned_short(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned short) c_parse_unsigned_long_long_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        USHRT_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

unsigned char c_parse_hexadecimal_unsigned_char(
    const char *const   data_buffer,
    const size_t        data_length,
    bool *const         parse_error
)
{
    return (unsigned char) c_parse_unsigned_long_long_alphanumeric_impl(
        data_buffer,
        data_length,
        parse_error,
        UCHAR_MAX,
        BASE_HEXADECIMAL,
        DIGITS_DECIMAL,
        CHARS_HEXADECIMAL,
        ALPHA_OFFSET_HEXADECIMAL
    );
}

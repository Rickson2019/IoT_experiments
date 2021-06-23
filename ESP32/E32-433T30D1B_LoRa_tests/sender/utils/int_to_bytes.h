
void int_to_bytes(int integer, char *integer_info_bytes)
{
    // char integer_info_bytes[2] = {0, 0};

    integer_info_bytes[0] = (integer) / 256;

    integer_info_bytes[1] = (integer) % 256;
}

int bytes_to_int(char *integer_info_bytes)
{
    int retval;
    // char integer_info_bytes[2] = {0, 0};

    // integer_info_bytes[0] = (integer) / 256;

    // integer_info_bytes[1] = (integer) % 256;
    retval = integer_info_bytes[0] * 256 + integer_info_bytes[1];
    return retval;
}

#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <stddef.h>

/* dummy implementation of strtol */

long strtol(const char *nPtr, char **endPtr, int base)
{
	if (nPtr == NULL)
	{
		errno = EINVAL;
		return 0;
	}

	const char *current_pointer = nPtr;

	if (*current_pointer == '\0')
	{
		if (endPtr)
		{
			*endPtr = (char *)current_pointer;
		}
		return 0;
	}

	if (base < 2 || base > 36)
	{
		if (base != 0)
		{
			if (endPtr)
			{
				*endPtr = (char *)current_pointer;
			}
			errno = EINVAL;
			return 0;
		}
	}

	while (isspace(*current_pointer))
	{
		current_pointer++;
	}

	int is_positive = 1;

	if (*current_pointer == '-')
	{
		is_positive = 0;
		current_pointer++;
	}
	else if (*current_pointer == '+')
	{
		current_pointer++;
	}

	if (base == 16)
	{
		if (*current_pointer == '0' && (*(current_pointer + 1) == 'X' || *(current_pointer + 1) == 'x'))
		{
			current_pointer += 2;
		}
	}
	else if (base == 8)
	{
		if (*current_pointer == '0')
		{
			current_pointer++;
		}
	}
	else if (base == 0)
	{
		if (*current_pointer == '0')
		{
			current_pointer++;
			if (*current_pointer == 'X' || *current_pointer == 'x')
			{
				base = 16;
				current_pointer++;
			}
			else
			{
				base = 8;
			}
		}
		else
		{
			base = 10;
		}
	}

	int current_number;
	long number = 0;
	int is_empty_string = 1;
	int is_overflow = 0;

	while (*current_pointer != '\0')
	{
		if (isdigit(*current_pointer) && (*current_pointer - '0') < base)
		{
			current_number = *current_pointer - '0';
		}
		else if (isalpha(*current_pointer) && (toupper(*current_pointer) - 'A' + 10) < base)
		{
			current_number = toupper(*current_pointer) - 'A' + 10;
		}
		else
		{
			break;
		}


		if (number > (LONG_MAX - current_number) / base || number < (LONG_MIN + current_number) / base)
		{
			is_overflow = 1;
		}

		if (is_overflow)
		{
			while (*current_pointer != '\0')
			{
				current_pointer++;
				if (!isdigit(*current_pointer) && !isalpha(*current_pointer))
				{
					break;
				}
			}

			if (endPtr)
			{
				*endPtr = (char *)current_pointer;
			}
			errno = ERANGE;

			if (is_positive)
			{
				return LONG_MAX;
			}
			else
			{
				return LONG_MIN;
			}
		}

		if (is_positive)
		{
			number = number * base + current_number;
		}
		else
		{
			number = number * base - current_number;
		}
		current_pointer++;
		is_empty_string = 0;
	}

	if (is_empty_string && base != 8 && base != 16)
	{
		return 0;
	}

	if (is_empty_string && base == 16)
	{
		current_pointer--;
		if (endPtr)
		{
			*endPtr = (char *)current_pointer;
		}
		return 0;
	}

	if (endPtr)
	{
		*endPtr = (char *)current_pointer;
	}

	return number;
}
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h> /* for write function */

#define BUFF_SIZE 1024

void print_buffer(char buffer[], int *buff_ind);
int handle_print(const char **format, va_list list, char buffer[], int *buff_ind);

/**
 * _printf - Printf function
 * @format: Format string.
 * Return: Number of characters printed.
 */
int _printf(const char *format, ...)
{
	if (format == NULL)
		return (-1);

	int printed_chars = 0;
	int buff_ind = 0;
	char buffer[BUFF_SIZE];
	va_list list;

	va_start(list, format);

	for (const char *current_char = format; *current_char != '\0'; current_char++)
	{
		if (*current_char != '%')
		{
			buffer[buff_ind++] = *current_char;
			if (buff_ind == BUFF_SIZE)
			{
				print_buffer(buffer, &buff_ind);
				printed_chars += buff_ind;
			}
		}
		else
		{
			print_buffer(buffer, &buff_ind);
			int printed = handle_print(&current_char, list, buffer, &buff_ind);
			if (printed < 0)
			{
				va_end(list);
				return (-1);
			}
			printed_chars += printed;
		}
	}

	print_buffer(buffer, &buff_ind);
	printed_chars += buff_ind;

	va_end(list);

	return (printed_chars);
}

/**
 * print_buffer - Prints the contents of the buffer if it exists
 * @buffer: Array of chars.
 * @buff_ind: Index at which to add the next char, represents the length.
 */
void print_buffer(char buffer[], int *buff_ind)
{
	if (*buff_ind > 0)
	{
		write(1, buffer, *buff_ind);
		*buff_ind = 0;
	}
}

/**
 * handle_print - Handles the conversion specifier and prints the corresponding value.
 * @format: Pointer to the current character in the format string.
 * @list: The va_list of arguments.
 * @buffer: The buffer to store the printed characters.
 * @buff_ind: Index at which to add the next char, represents the length.
 * Return: Number of characters printed for this conversion specifier.
 */
int handle_print(const char **format, va_list list, char buffer[], int *buff_ind)
{
	int printed = 0;
	(*format)++; /* Move past '%' */

	/* Handle conversion specifiers */
	switch (**format)
	{
	case 'c':
		/* Fetch the next character argument from va_list */
		buffer[(*buff_ind)++] = va_arg(list, int);
		printed = 1;
		break;
	case 's':
		/* Fetch the next string argument from va_list */
		for (const char *str = va_arg(list, const char *); *str != '\0'; str++)
			buffer[(*buff_ind)++] = *str;
		printed = (*buff_ind);
		break;
	case '%':
		buffer[(*buff_ind)++] = '%';
		printed = 1;
		break;
	default:
		buffer[(*buff_ind)++] = '%'; /* Print the '%' character if the conversion specifier is unknown */
		buffer[(*buff_ind)++] = **format;
		printed = 2;
	}

	return (printed);
}

/**
 * main - Entry point of the program
 * This function demonstrates the usage of the _printf function.
 * It prints a formatted string using various conversion specifiers.
 * Return: Always 0.
 */
int main(void)
{
	int printed_chars = _printf("Hello, %s! The character is %c. This is a percent sign: %%.\n", "John", 'A');
	printf("Total printed characters: %d\n", printed_chars);
	return (0);
}

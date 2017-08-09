#define LSW(value) \
	*(unsigned long int *)&(value)
#define MSW(value) \
	*((unsigned long int *)&(value) + 1)

int __cdecl _isnan(double x)
{
	return (MSW(x) & 0x7FF00000) == 0x7FF00000 && ((MSW(x) & 0x000FFFFF) || LSW(x));
}

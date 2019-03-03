#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv [])
{
	system("raspistill -t 10000 -o filename.png");

	return 0;
}

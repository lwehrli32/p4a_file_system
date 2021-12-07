#include <stdio.h>
#include "mfs.h"

int main(int argc, char *argv[]){
	int rc = MFS_Init("localhost", 10000);
	return rc;
}

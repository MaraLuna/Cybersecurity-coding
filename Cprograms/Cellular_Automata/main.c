#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "picture.h"
#include "rules.h"
#include "arrays.h"

int main( int argc, char **argv){
    if (argc<3){
        printf("Usage: %s program width height\n",argv[0]);
        return 1;
    }
long width = strtol(argv[1],NULL,10);
long height = strtol(argv[2],NULL,10);
printf("Drawing %ld X %ld\n", width, height);

rule_18_picture(width, height);
test_rule_18();

rule_57_picture(width, height);
test_rule_57();

rule_60_picture(width, height);
test_rule_60();

rule_73_picture(width, height);
test_rule_73();

}


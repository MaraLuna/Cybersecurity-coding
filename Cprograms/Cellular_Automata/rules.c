#include <stdio.h>
#include <stdbool.h>

#include "rules.h"

bool rule_18(bool left, bool center, bool right){
    if (left && !center && !right) {
        return true;
    }
    if (!left && !center && right){
        return true;
    }
    else{
        return false;
    }
}
bool rule_57(bool left, bool center, bool right){
    if (left && !center && right){
        return true;
    }
    if (left && !center && !right){
        return true;
    }
    if (!left && center && right){
        return true;
    }
    if (!left && !center && !right){
        return true;
    }
    else{
        return false;
    }
}

bool rule_60(bool left, bool center, bool right){
    if (left && !center && right){
        return true;
    }
    if (left && !center && !right){
        return true;
    }
    if (!left && center && right){
        return true;
    }
    if (!left && center && !right){
        return true;
    }
    else{
        return false;
    }
}


bool rule_73(bool left, bool center, bool right){
    if (left && center && !right){
        return true;
    }
    if (!left && center && right){
        return true;
    }
    if (!left && !center && !right){
        return true;
    }
    else{
        return false;
    }
}

void test_rule_18(void){
    if ((rule_18(true,true,true)) != false) {
        printf("Error with Rule 18 t-t-t");
    }
    if ((rule_18(true,true,false)) != false) {
        printf("Error with Rule 18 t-t-f");
    }
    if ((rule_18(true,false,true)) != false) {
        printf("Error with Rule 18 t-f-t");
    }
    if ((rule_18(true,false,false)) != true) {
        printf("Error with Rule 18 t-f-f");
    }
    if ((rule_18(false,true,true)) != false) {
        printf("Error with Rule 18 f-t-t");
    }
    if ((rule_18(false,true,false)) != false) {
        printf("Error with Rule 18 f-t-f");
    }
    if ((rule_18(false,false,true)) != true) {
        printf("Error with Rule 18 f-f-t");
    }
    if ((rule_18(false,false,false)) != false) {
        printf("Error with Rule 18 f-f-f");
    }
}
void test_rule_57(void){
    if ((rule_57(true,true,true)) != false) {
        printf("Error with Rule 57 t-t-t");
    }
    if ((rule_57(true,true,false)) != false) {
        printf("Error with Rule 57 t-t-f");
    }
    if ((rule_57(true,false,true)) != true) {
        printf("Error with Rule 57 t-f-t");
    }
    if ((rule_57(true,false,false)) != true) {
        printf("Error with Rule 57 t-f-f");
    }
    if ((rule_57(false,true,true)) != true) {
        printf("Error with Rule 57 f-t-t");
    }
    if ((rule_57(false,true,false)) != false) {
        printf("Error with Rule 57 f-t-f");
    }
    if ((rule_57(false,false,true)) != false) {
        printf("Error with Rule 57 f-f-t");
    }
    if ((rule_57(false,false,false)) != true) {
        printf("Error with Rule 57 f-f-f");
    }
}
void test_rule_60(void){
    if ((rule_60(true,true,true)) != false) {
        printf("Error with Rule 60 t-t-t");
    }
    if ((rule_60(true,true,false)) != false) {
        printf("Error with Rule 60 t-t-f");
    }
    if ((rule_60(true,false,true)) != true) {
        printf("Error with Rule 60 t-f-t");
    }
    if ((rule_60(true,false,false)) != true) {
        printf("Error with Rule 60 t-f-f");
    }
    if ((rule_60(false,true,true)) != true) {
        printf("Error with Rule 60 f-t-t");
    }
    if ((rule_60(false,true,false)) != true) {
        printf("Error with Rule 60 f-t-f");
    }
    if ((rule_60(false,false,true)) != false) {
        printf("Error with Rule 60 f-f-t");
    }
    if ((rule_60(false,false,false)) != false) {
        printf("Error with Rule 60 f-f-f");
    }
}

void test_rule_73(void){
    if ((rule_73(true,true,true)) != false) {
        printf("Error with Rule 73 t-t-t");
    }
    if ((rule_73(true,true,false)) != true) {
        printf("Error with Rule 73 t-t-f");
    }
    if ((rule_73(true,false,true)) != false) {
        printf("Error with Rule 73 t-f-t");
    }
    if ((rule_73(true,false,false)) != false) {
        printf("Error with Rule 73 t-f-f");
    }
    if ((rule_73(false,true,true)) != true) {
        printf("Error with Rule 73 f-t-t");
    }
    if ((rule_73(false,true,false)) != false) {
        printf("Error with Rule 73 f-t-f");
    }
    if ((rule_73(false,false,true)) != false) {
        printf("Error with Rule 73 f-f-t");
    }
    if ((rule_73(false,false,false)) != true) {
        printf("Error with Rule 73 f-f-f");
    }
}






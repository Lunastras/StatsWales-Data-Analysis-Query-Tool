//
// Created by radu_ on 3/14/2021.
//

#include "Helper.h"

std::string lowerString(std::string s) {
    for(char &c: s) {
        if(c >= 'A' && c <= 'Z') {
            c = std::tolower(c);
        }
    }

    return s;
}
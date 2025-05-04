#ifndef MAIN_H
#define MAIN_H

#include "utils.h"
#include "caesar.h"
#include "monoalphabetic.h"
#include "homophonic.h"
#include "polygram.h"
#include "polyalphabetic.h"
#include "playfair.h"
#include "hill.h"
#include "encodings.h"

void display_main_menu(void);
void handle_cipher_selection(int choice);

#endif 
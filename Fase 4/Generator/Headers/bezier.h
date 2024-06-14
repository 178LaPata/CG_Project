#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "../../Comum/Headers/forma.h"
#include "../../Comum/Headers/ponto.h"
#include "../../Comum/Matrix.h"
#include <sys/stat.h>

void lerFicheiroPatch(char *ficheiro, int nivel_tess, char *ficheiroF);
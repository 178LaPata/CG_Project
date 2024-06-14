#include<vector>
#include<iostream>
#include <fstream>
#include<string>
#include "ponto.h"

class Forma
{
private:
	std::vector<Ponto> pontos;

public:
	Forma() {};
	~Forma() {};

	void adicionarPonto(Ponto p);
	void escreverParaFicheiro(char *file);
};
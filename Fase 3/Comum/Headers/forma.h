#ifndef FORMA_H
#define FORMA_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "ponto.h"

class Forma
{
private:
	std::string ficheiro;
	std::vector<Ponto> pontos;
	uint32_t vboId; // uint32_t e o mesmo que GLuint, que e usado para identificar o VBO

public:
	Forma(){};
	Forma(std::string ficheiro) : ficheiro(ficheiro){}; // inicializar a variavel
	~Forma(){};

	void adicionarPonto(Ponto p);
	void escreverParaFicheiro(char *file);
	std::vector<Ponto> getPontos();
	void desenhar();
	void setVBOID(uint32_t id);
	uint32_t getVBOID();
	std::string getFicheiro();
};
#endif
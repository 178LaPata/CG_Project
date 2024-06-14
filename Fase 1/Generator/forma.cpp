#include "Headers/forma.h"

//estrutura que guarda os pontos que constituem as formas

void Forma::adicionarPonto(Ponto p)
{
    this->pontos.push_back(p);
}

void Forma::escreverParaFicheiro(char *file)
{
    std::string caminho = "", strFicheiro = std::string(file);
	if (strFicheiro.find("../files/") == -1)
		caminho.append("../files/");
	caminho.append(strFicheiro);

	std::ofstream f(caminho);

	for (Ponto ponto : this->pontos)
		f << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";

	f.close();
}
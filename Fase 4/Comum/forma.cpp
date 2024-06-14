#include "Headers/forma.h"

// estrutura que guarda os pontos que constituem as formas

void Forma::adicionarPonto(Ponto p)
{
    this->pontos.push_back(p);
}

void Forma::adicionarNormal(Ponto p)
{
    this->normais.push_back(p);
}

void Forma::adicionarCoordTextura(Ponto2D p)
{
    this->coordsText.push_back(p);
}

void Forma::escreverParaFicheiro(char *file)
{
    std::string caminho = "", strFicheiro = std::string(file);
    if (strFicheiro.find("../files/") == -1)
        caminho.append("../files/");
    caminho.append(strFicheiro);

    std::ofstream f(caminho);

    f << pontos.size() << "\n";
    for (Ponto ponto : this->pontos)
        f << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";

    f << normais.size() << "\n";
    for (Ponto ponto : this->normais)
        f << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";

    f << coordsText.size() << "\n";
    for (Ponto2D ponto : this->coordsText)
        f << ponto.getX() << " " << ponto.getY() << "\n";

    f.close();
}

bool Forma::operator==(const Forma &forma)
{
    return this->ficheiro==forma.ficheiroTextura;
}


std::vector<Ponto> Forma::getPontos()
{
    return this->pontos;
}

std::vector<Ponto> Forma::getNormais()
{
    return this->normais;
}

std::vector<Ponto2D> Forma::getCoordTextura()
{
    return this->coordsText;
}

Material Forma::getMaterial() 
{
    return this->m;
}


void Forma::normalizarNormal()
{
    for(Ponto& normal : normais)
        normal.normalizar();
}

void Forma::multiplicarNormais(float num)
{
    for(int i=0; i<normais.size(); i++) normais[i] = normais[i]*num;
}

void Forma::setVBOID(uint32_t id)
{
    this->vboId = id;
}

void Forma::setTEXTUREID(uint32_t id)
{
    this->textureId = id;
}

void Forma::setMaterial(Material m)
{
    this->m = m;
}

uint32_t Forma::getVBOID()
{
    return this->vboId;
}

std::string Forma::getFicheiro()
{
    return this->ficheiro;
}

std::string Forma::getFicheiroTextura()
{
    return this->ficheiroTextura;
}

void Forma::setFicheiroTextura(std::string ficheiroTextura)
{
    this->ficheiroTextura = ficheiroTextura;
}
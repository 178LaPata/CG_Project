#ifndef FORMA_H
#define FORMA_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "ponto.h"
#include "ponto2d.h"

class Material
{
    private:
        Ponto diffuse, ambient, specular, emissive;
        float shininess;

    public:
        Material(): diffuse(200.0f/255.0f,200.0f/255.0f,200.0f/255.0f), ambient(50.0f/255.0f,50.0f/255.0f,50.0f/255.0f), specular(0,0,0), emissive(0,0,0), shininess(0.0f){};
        Material(Ponto d, Ponto a, Ponto s, Ponto e, float sh): diffuse(d), ambient(a), specular(s), emissive(e), shininess((sh<0) ?0 :(sh>128) ?128 :sh) {};
        Material(Material& material): diffuse(material.diffuse), ambient(material.ambient), specular(material.specular), emissive(material.emissive), shininess(material.shininess) {};

        void apply();
        
        inline void setDiffuse(Ponto diffuse) {
            this->diffuse = diffuse;
        }
        inline void setAmbient(Ponto ambient) {
            this->diffuse = diffuse;
        }
        inline void setSpecular(Ponto specular) {
            this->specular = specular;
        }
        inline void setEmissive(Ponto emissive) {
            this->emissive = emissive;
        }
        inline void setShininess(float shininess) {
            this->shininess = shininess;
        }
		inline Ponto getDiffuse() {
			return diffuse;
		}
		inline Ponto getAmbient() {
			return ambient;
		}
		inline Ponto getSpecular() {
			return specular;
		}
		inline Ponto getEmissive() {
			return emissive;
		}
		inline float getShininess() {
			return shininess;
		}
};


class Forma
{
private:
	std::string ficheiro;
	std::string ficheiroTextura;
	std::vector<Ponto> pontos, normais;
	std::vector<Ponto2D> coordsText;
	uint32_t vboId; // uint32_t e o mesmo que GLuint, que e usado para identificar o VBO
	uint32_t textureId;
	Material m; // material

public:
	Forma(){};
	Forma(std::string ficheiro) : ficheiro(ficheiro), m() {}; // inicializar a variavel
	~Forma(){};

	void adicionarPonto(Ponto p);
	void adicionarNormal(Ponto p);
	void adicionarCoordTextura(Ponto2D p);
	void escreverParaFicheiro(char *file);
	std::string getFicheiro();
	std::string getFicheiroTextura();
	std::vector<Ponto> getPontos();
	std::vector<Ponto> getNormais();
	std::vector<Ponto2D> getCoordTextura();
	Material getMaterial();
	bool operator==(const Forma& forma);
	void normalizarNormal();
	void multiplicarNormais(float num);
	void desenhar();
	void setVBOID(uint32_t id);
	void setTEXTUREID(uint32_t id);
	void setFicheiroTextura(std::string ficheiroTextura);
	void setMaterial(Material m);
	uint32_t getVBOID();
};
#endif
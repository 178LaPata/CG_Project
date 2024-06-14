#ifndef LUZ_H
#define LUZ_H

#include "../../Comum/Headers/ponto.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif


enum TipoLuz {
    LUZ_PONTO = 0,
    LUZ_DIRECIONAL = 1,
    LUZ_FOCO = 2
};

class Luz {
public:
    Luz(TipoLuz tipo, Ponto posDir);
    void definirTipo(TipoLuz tipo);
    void definirPosicao(Ponto pos);
    void definirDirecao(Ponto dir);
    void definirCortarFoco(float cutoff);
    void definirIdentificadorLuz(GLenum identificador);
    void inicializar();
    void aplicar();

    TipoLuz obterTipo();
    Ponto obterPosicao();
    Ponto obterDirecao();
    float obterCortarFoco();
    GLenum obterIdentificadorLuz();

private:
    TipoLuz tipo;
    Ponto posicao;
    Ponto direcao;
    float cutoff;
    GLenum identificador;
};

#endif

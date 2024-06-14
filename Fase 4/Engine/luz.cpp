#include "Headers/luz.h"

void Luz::inicializar() {
    glEnable(GL_LIGHTING);
    glEnable(identificador);

    float branco[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float especular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float difuso[4] = {1.0f, 1.0f, 1.0f, 0.0f}; // Changed alpha to 1.0f
    float ambiente[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(identificador, GL_SPECULAR, especular);
    glLightfv(identificador, GL_AMBIENT, ambiente);
    glLightfv(identificador, GL_DIFFUSE, difuso);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, branco);
    glEnable(GL_RESCALE_NORMAL);
}

void Luz::aplicar() {
    glEnable(GL_LIGHTING);
    glEnable(identificador);

    if (tipo == 0) { // LUZ_PONTO
        GLfloat pos[4] = {posicao.getX(), posicao.getY(), posicao.getZ(), 1.0f};
        glLightfv(identificador, GL_POSITION, pos);
    } else if (tipo == 1) { // LUZ_DIRECIONAL
        GLfloat dir[4] = {direcao.getX(), direcao.getY(), direcao.getZ(), 0.0f};
        glLightfv(identificador, GL_POSITION, dir);
    } else if (tipo == 2) { // LUZ_FOCO
        GLfloat pos[4] = {posicao.getX(), posicao.getY(), posicao.getZ(), 1.0f};
        GLfloat dir[4] = {direcao.getX(), direcao.getY(), direcao.getZ(), 0.0f};
        glLightfv(identificador, GL_POSITION, pos);
        glLightfv(identificador, GL_SPOT_DIRECTION, dir);
        glLightf(identificador, GL_SPOT_CUTOFF, cutoff);
    }
}


Luz::Luz(TipoLuz tipo, Ponto posDir) : tipo(tipo) {
    if (tipo == LUZ_PONTO) {
        posicao = posDir;
    } else {
        direcao = posDir;
    }
    inicializar();
}

void Luz::definirTipo(TipoLuz tipo) {
    this->tipo = tipo;
}

void Luz::definirPosicao(Ponto pos) {
    this->posicao = pos;
}

void Luz::definirDirecao(Ponto dir) {
    this->direcao = dir;
}

void Luz::definirCortarFoco(float cutoff) {
    this->cutoff = cutoff;
}

void Luz::definirIdentificadorLuz(GLenum identificador) {
    this->identificador = identificador;
}

TipoLuz Luz::obterTipo() {
    return tipo;
}

Ponto Luz::obterPosicao() {
    return posicao;
}

Ponto Luz::obterDirecao() {
    return direcao;
}

float Luz::obterCortarFoco() {
    return cutoff;
}

GLenum Luz::obterIdentificadorLuz() {
    return identificador;
}

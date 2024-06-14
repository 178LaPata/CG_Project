#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include "../Comum/Headers/forma.h"
#include "./Headers/bezier.h"

void geraPlano(float tam_lado, float num_divs, char *ficheiro)
{
    float ref = tam_lado / 2;
    float x = -ref, y = 0, z = -ref;
    float ladoCadaQua = tam_lado / num_divs;

    Forma forma = Forma();

    for (int i = 0; i < num_divs; i++)
    {
        x = -ref;
        for (int j = 0; j < num_divs; j++)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(1,1));
            forma.adicionarPonto(Ponto(x, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(1,0));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(0,0));
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(1,1));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(0,0));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(0,1));
            x += ladoCadaQua;
        }
        z += ladoCadaQua;
    }
    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

void geraCubo(float tam_lado, float num_divs, char *ficheiro)
{
    float ref = tam_lado / 2;
    float ladoCadaQua = tam_lado / num_divs;
    float x = -ref, y = -ref, z = -ref;
    float s, t, textInc = 1.0f/num_divs;

    Forma forma = Forma();

    // baixo
    s = 0.0f;
    for (int i = 0; i < num_divs; i++,s+=textInc)
    {
        x = -ref;
        t=0.0f;
        for (int j = 0; j < num_divs; j++,t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));

            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));

            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));

            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,-1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));

            x += ladoCadaQua;
        }
        z += ladoCadaQua;
    }
    x = -ref, y = ref, z = -ref, s=0.0f;

    // cima
    for (int i = 0; i < num_divs; i++, s+=textInc)
    {
        x = -ref;
        t=0.0f;
        for (int j = 0; j < num_divs; j++, t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));

            forma.adicionarPonto(Ponto(x, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));

            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));

            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z));
            forma.adicionarNormal(Ponto(0,1,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));

            x += ladoCadaQua;
        }
        z += ladoCadaQua;
    }
    x = -ref, y = -ref, z = -ref, s=0.0f;

    // lado 1 - tras
    for (int i = 0; i < num_divs; i++, s+=textInc)
    {
        y = -ref, t=0.0f;
        for (int j = 0; j < num_divs; j++, t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z));
            forma.adicionarNormal(Ponto(0,0,-1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));
            y += ladoCadaQua;
        }
        x += ladoCadaQua;
    }

    x = -ref, y = -ref, z = ref, s=0.0f;

    // lado 2 - frente
    for (int i = 0; i < num_divs; i++, s+=textInc)
    {
        y = -ref, t=0.0f;
        for (int j = 0; j < num_divs; j++, t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));
            forma.adicionarPonto(Ponto(x + ladoCadaQua, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(0,0,1));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            y += ladoCadaQua;
        }
        x += ladoCadaQua;
    }

    x = -ref, y = -ref, z = -ref, s=0.0f;

    // lado 3 - esquerda
    for (int i = 0; i < num_divs; i++, s+=textInc)
    {
        y = -ref, t=0.0f;
        for (int j = 0; j < num_divs; j++, t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(-1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));
            y += ladoCadaQua;
        }
        z += ladoCadaQua;
    }

    x = ref, y = -ref, z = -ref;

    // lado 4 - direita
    for (int i = 0; i < num_divs; i++, s+=textInc)
    {
        y = -ref, t=0.0f;
        for (int j = 0; j < num_divs; j++, t+=textInc)
        {
            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            forma.adicionarPonto(Ponto(x, y, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t));

            forma.adicionarPonto(Ponto(x, y, z));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s,t+textInc));
            forma.adicionarPonto(Ponto(x, y + ladoCadaQua, z + ladoCadaQua));
            forma.adicionarNormal(Ponto(1,0,0));
            forma.adicionarCoordTextura(Ponto2D(s+textInc,t+textInc));
            y += ladoCadaQua;
        }
        z += ladoCadaQua;
    }
    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

void geraEsfera(float raio, int nr_slices, int nr_stacks, char *ficheiro)
{
    Forma forma = Forma();

    float anguloSlices = (2 * M_PI) / nr_slices;
    float anguloStack = M_PI / nr_stacks;

    float alpha = 0;
    float beta = -M_PI_2;

    // baixo
    for (int j = 0; j < nr_slices; j++, alpha += anguloSlices)
    {   

        Ponto p1 = Ponto(raio * cos(beta + anguloStack) * sin(alpha + anguloSlices), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha + anguloSlices)),
              p2 = Ponto(raio * cos(beta + anguloStack) * sin(alpha), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha)),
              p3 = Ponto(raio * cos(beta) * sin(alpha + anguloSlices), raio * sin(beta), raio * cos(beta) * cos(alpha + anguloSlices));
  
        Ponto2D t1 = Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))),
                t2 = Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))),
                t3 = Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0)));


        forma.adicionarPonto(p1);
        forma.adicionarNormal(p1);
        forma.adicionarCoordTextura(t1);
        forma.adicionarPonto(p2);
        forma.adicionarNormal(p2);
        forma.adicionarCoordTextura(t2);
        forma.adicionarPonto(p3);
        forma.adicionarNormal(p3);
        forma.adicionarCoordTextura(t3);
        
    }
    beta += anguloStack;

    // meio
    for (int i = 1; i < nr_stacks - 1; i++, beta += anguloStack)
    {
        alpha = 0;

        for (int j = 0; j < nr_slices; j++, alpha += anguloSlices)
        {

            forma.adicionarPonto(Ponto(raio * cos(beta) * sin(alpha), raio * sin(beta), raio * cos(beta) * cos(alpha)));
            forma.adicionarNormal(Ponto(cos(beta) * sin(alpha), sin(beta), cos(beta) * cos(alpha)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0))));
            forma.adicionarPonto(Ponto(raio * cos(beta) * sin(alpha + anguloSlices), raio * sin(beta), raio * cos(beta) * cos(alpha + anguloSlices)));
            forma.adicionarNormal(Ponto(cos(beta) * sin(alpha + anguloSlices), sin(beta), cos(beta) * cos(alpha + anguloSlices)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0))));
            forma.adicionarPonto(Ponto(raio * cos(beta + anguloStack) * sin(alpha), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha)));
            forma.adicionarNormal(Ponto(cos(beta + anguloStack) * sin(alpha), sin(beta + anguloStack), cos(beta + anguloStack) * cos(alpha)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))));

            forma.adicionarPonto(Ponto(raio * cos(beta + anguloStack) * sin(alpha + anguloSlices), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha + anguloSlices)));
            forma.adicionarNormal(Ponto(cos(beta + anguloStack) * sin(alpha + anguloSlices), sin(beta + anguloStack), cos(beta + anguloStack) * cos(alpha + anguloSlices)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))));
            forma.adicionarPonto(Ponto(raio * cos(beta + anguloStack) * sin(alpha), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha)));
            forma.adicionarNormal(Ponto(cos(beta + anguloStack) * sin(alpha), sin(beta + anguloStack), cos(beta + anguloStack) * cos(alpha)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))));
            forma.adicionarPonto(Ponto(raio * cos(beta) * sin(alpha + anguloSlices), raio * sin(beta), raio * cos(beta) * cos(alpha + anguloSlices)));
            forma.adicionarNormal(Ponto(cos(beta) * sin(alpha + anguloSlices), sin(beta), cos(beta) * cos(alpha + anguloSlices)));
            forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0))));

        }
    }
    alpha = 0;
    // cima
    for (int j = 0; j < nr_slices; j++, alpha += anguloSlices)
    {
        forma.adicionarPonto(Ponto(raio * cos(beta) * sin(alpha), raio * sin(beta), raio * cos(beta) * cos(alpha)));
        forma.adicionarNormal(Ponto(cos(beta) * sin(alpha), sin(beta), cos(beta) * cos(alpha)));
        forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0))));
        forma.adicionarPonto(Ponto(raio * cos(beta) * sin(alpha + anguloSlices), raio * sin(beta), raio * cos(beta) * cos(alpha + anguloSlices)));
        forma.adicionarNormal(Ponto(cos(beta) * sin(alpha + anguloSlices), sin(beta), cos(beta) * cos(alpha + anguloSlices)));
        forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha-anguloSlices) + 1.0, (1.0 / (M_PI)) * (beta + (M_PI / 2.0))));
        forma.adicionarPonto(Ponto(raio * cos(beta + anguloStack) * sin(alpha), raio * sin(beta + anguloStack), raio * cos(beta + anguloStack) * cos(alpha)));
        forma.adicionarNormal(Ponto(cos(beta + anguloStack) * sin(alpha), sin(beta + anguloStack), cos(beta + anguloStack) * cos(alpha)));
        forma.adicionarCoordTextura(Ponto2D((1.0 / (2.0*M_PI)) * (-alpha) + 1.0, (1.0 / (M_PI)) * (beta + anguloStack + (M_PI / 2.0))));
    }

    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

void geraCone(float raio, float altura, int nr_slices, int nr_stacks, char *ficheiro)
{
    Forma forma = Forma();

    float alpha = (2 * M_PI) / nr_slices;
    float anguloSlices = alpha;

    float altura_niveis = altura / nr_stacks; // altura a cada vez que ando um nivel para cima
    float altura_aumento = altura_niveis;     // quando aumenta a altura

    float raio_2 = raio / nr_stacks;
    float raio_niveis = raio - raio_2; // raio a cada nivel que subo

    float tInc = 1.0f/nr_stacks, sInc = 1.0f/nr_slices; // incremento de t em cada camada da textura, incremento de s para cada angulo (lateral)


    for (int i = 0; i < nr_slices; i++)
    {
        // Base
        forma.adicionarPonto(Ponto(0, 0, 0));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5, 0.5));
        forma.adicionarPonto(Ponto(raio * sin(anguloSlices), 0, raio * cos(anguloSlices)));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(anguloSlices), 0.5 + 0.5*sin(anguloSlices)));
        forma.adicionarPonto(Ponto(raio * sin(anguloSlices - alpha), 0, raio * cos(anguloSlices - alpha)));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(anguloSlices - alpha), 0.5 + 0.5*sin(anguloSlices - alpha)));
        
        // Lados
        for (int j = 0; j < nr_stacks ; j++)
        {
            Ponto2D t1 = Ponto2D(sInc*(i+1),j*tInc),
                    t2 = Ponto2D(sInc*(i+1),(j+1)*tInc),
                    t3 = Ponto2D(sInc*i,(j+1)*tInc),
                    t4 = Ponto2D(sInc*i,j*tInc);


            forma.adicionarPonto(Ponto((raio_niveis + raio_2) * sin(anguloSlices), altura_aumento - altura_niveis, (raio_niveis + raio_2) * cos(anguloSlices)));
            forma.adicionarNormal(Ponto(sin(anguloSlices), raio_2, cos(anguloSlices)));
            forma.adicionarCoordTextura(t1);
            forma.adicionarPonto(Ponto(raio_niveis * sin(anguloSlices), altura_aumento, raio_niveis * cos(anguloSlices)));
            forma.adicionarNormal(Ponto(sin(anguloSlices), raio_2, cos(anguloSlices)));
            forma.adicionarCoordTextura(t2);
            forma.adicionarPonto(Ponto(raio_niveis * sin(anguloSlices - alpha), altura_aumento, raio_niveis * cos(anguloSlices - alpha)));
            forma.adicionarNormal(Ponto(sin(anguloSlices - alpha), raio_2, cos(anguloSlices - alpha)));
            forma.adicionarCoordTextura(t3);

            forma.adicionarPonto(Ponto((raio_niveis + raio_2) * sin(anguloSlices - alpha), altura_aumento - altura_niveis, (raio_niveis + raio_2) * cos(anguloSlices - alpha)));
            forma.adicionarNormal(Ponto(sin(anguloSlices - alpha), raio_2, cos(anguloSlices - alpha)));
            forma.adicionarCoordTextura(t4);
            forma.adicionarPonto(Ponto((raio_niveis + raio_2) * sin(anguloSlices), altura_aumento - altura_niveis, (raio_niveis + raio_2) * cos(anguloSlices)));
            forma.adicionarNormal(Ponto(sin(anguloSlices), raio_2, cos(anguloSlices)));
            forma.adicionarCoordTextura(t1);
            forma.adicionarPonto(Ponto(raio_niveis * sin(anguloSlices - alpha), altura_aumento, raio_niveis * cos(anguloSlices - alpha)));
            forma.adicionarNormal(Ponto(sin(anguloSlices - alpha), raio_2, cos(anguloSlices - alpha)));
            forma.adicionarCoordTextura(t3);
            altura_aumento += altura_niveis;
            raio_niveis -= raio_2;
        }
        anguloSlices += alpha;
        altura_aumento = altura_niveis;
        raio_niveis = raio - raio_2;
    }
    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

void geraCilindro(float raio, float altura, int nr_slices, int nr_stacks, char *ficheiro)
{
    Forma forma = Forma();

    float anguloSlices = (2 * M_PI) / nr_slices;
    float alturaStep = altura / nr_stacks;
    float halfHeight = altura / 2;

    // Criar a base do cilindro
    for (int i = 0; i < nr_slices; i++)
    {
        float theta = i * anguloSlices;
        float nextTheta = (i + 1) * anguloSlices;

        // Base inferior
        forma.adicionarPonto(Ponto(0, -halfHeight, 0));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5, 0.5));
        forma.adicionarPonto(Ponto(raio * cos(theta), -halfHeight, raio * sin(theta)));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta)));
        forma.adicionarPonto(Ponto(raio * cos(nextTheta), -halfHeight, raio * sin(nextTheta)));
        forma.adicionarNormal(Ponto(0,-1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(nextTheta), 0.5 + 0.5*sin(nextTheta)));

        // Base superior
        forma.adicionarPonto(Ponto(0, halfHeight, 0));
        forma.adicionarNormal(Ponto(0,1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5, 0.5));
        forma.adicionarPonto(Ponto(raio * cos(nextTheta), halfHeight, raio * sin(nextTheta)));
        forma.adicionarNormal(Ponto(0,1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(nextTheta), 0.5 + 0.5*sin(nextTheta)));
        forma.adicionarPonto(Ponto(raio * cos(theta), halfHeight, raio * sin(theta)));
        forma.adicionarNormal(Ponto(0,1,0));
        forma.adicionarCoordTextura(Ponto2D(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta)));
    }

    // Criar os lados do cilindro
    for (int i = 0; i < nr_stacks; i++)
    {
        float currentHeight = i * alturaStep - halfHeight;
        float nextHeight = (i + 1) * alturaStep - halfHeight;

        for (int j = 0; j < nr_slices; j++)
        {
            float theta = j * anguloSlices;
            float nextTheta = (j + 1) * anguloSlices;

            // Lado
            forma.adicionarPonto(Ponto(raio * cos(theta), currentHeight, raio * sin(theta)));
            forma.adicionarNormal(Ponto(cos(theta), 0, sin(theta)));
            forma.adicionarCoordTextura(Ponto2D((float)j / nr_slices, (float)i / nr_stacks));
            forma.adicionarPonto(Ponto(raio * cos(theta), nextHeight, raio * sin(theta)));
            forma.adicionarNormal(Ponto(cos(theta), 0, sin(theta)));
            forma.adicionarCoordTextura(Ponto2D((float)j / nr_slices, (float)(i + 1) / nr_stacks));
            forma.adicionarPonto(Ponto(raio * cos(nextTheta), nextHeight, raio * sin(nextTheta)));
            forma.adicionarNormal(Ponto(cos(nextTheta), 0, sin(nextTheta)));
            forma.adicionarCoordTextura(Ponto2D((float)(j + 1) / nr_slices, (float)(i + 1) / nr_stacks));

            forma.adicionarPonto(Ponto(raio * cos(nextTheta), nextHeight, raio * sin(nextTheta)));
            forma.adicionarNormal(Ponto(cos(nextTheta), 0, sin(nextTheta)));
            forma.adicionarCoordTextura(Ponto2D((float)(j + 1) / nr_slices, (float)(i + 1) / nr_stacks));
            forma.adicionarPonto(Ponto(raio * cos(nextTheta), currentHeight, raio * sin(nextTheta)));
            forma.adicionarNormal(Ponto(cos(nextTheta), 0, sin(nextTheta)));
            forma.adicionarCoordTextura(Ponto2D((float)(j + 1) / nr_slices, (float)i / nr_stacks));
            forma.adicionarPonto(Ponto(raio * cos(theta), currentHeight, raio * sin(theta)));
            forma.adicionarNormal(Ponto(cos(theta), 0, sin(theta)));
            forma.adicionarCoordTextura(Ponto2D((float)j / nr_slices, (float)i / nr_stacks));
        }
    }
    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

Ponto geraPontoTorus(float raio_dentro, float raio_fora, float alpha, float beta)
{
    return Ponto(cos(alpha) * (raio_dentro + sin(beta) * raio_fora),
                 cos(beta) * raio_fora,
                 sin(alpha) * (raio_dentro + sin(beta) * raio_fora));
}

void geraTorus(float raio_fora, float raio_dentro, int nr_slices, int nr_stacks, char *ficheiro)
{
    Forma forma = Forma();
    float alpha, beta, nextAlpha, nextBeta;
    float anguloSlices = (2 * M_PI) / nr_slices;
    float anguloStack = (2 * M_PI) / nr_stacks;

    for (int i = 0; i < nr_slices; i++)
    {
        alpha = i * anguloSlices;
        nextAlpha = (i + 1) * anguloSlices;

        for (int j = 0; j < nr_stacks; j++)
        {
            beta = j * anguloStack;
            nextBeta = (j + 1) * anguloStack;

            Ponto p1 = geraPontoTorus(raio_dentro, raio_fora, alpha, beta);
            Ponto p2 = geraPontoTorus(raio_dentro, raio_fora, alpha, nextBeta);
            Ponto p3 = geraPontoTorus(raio_dentro, raio_fora, nextAlpha, nextBeta);
            Ponto p4 = geraPontoTorus(raio_dentro, raio_fora, nextAlpha, beta);

            forma.adicionarPonto(p1);
            forma.adicionarNormal(p1);
            forma.adicionarCoordTextura(Ponto2D((float)i / nr_slices, (float)j / nr_stacks));
            forma.adicionarPonto(p2);
            forma.adicionarNormal(p2);
            forma.adicionarCoordTextura(Ponto2D((float)i / nr_slices, (float)(j + 1) / nr_stacks));
            forma.adicionarPonto(p3);
            forma.adicionarNormal(p3);
            forma.adicionarCoordTextura(Ponto2D((float)(i + 1) / nr_slices, (float)(j + 1) / nr_stacks));

            forma.adicionarPonto(p1);
            forma.adicionarNormal(p1);
            forma.adicionarCoordTextura(Ponto2D((float)i / nr_slices, (float)j / nr_stacks));
            forma.adicionarPonto(p3);
            forma.adicionarNormal(p3);
            forma.adicionarCoordTextura(Ponto2D((float)(i + 1) / nr_slices, (float)(j + 1) / nr_stacks));
            forma.adicionarPonto(p4);
            forma.adicionarNormal(p4);
            forma.adicionarCoordTextura(Ponto2D((float)(i + 1) / nr_slices, (float)j / nr_stacks));
        }
    }
    forma.normalizarNormal();
    forma.escreverParaFicheiro(ficheiro);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << "Uso: " << argv[0] << " [plane|box|cone|sphere|cylinder] tam_lado num_divs ficheiro" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "plane") == 0)
    {
        geraPlano(std::stof(argv[2]), std::stof(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "box") == 0)
    {
        geraCubo(std::stof(argv[2]), std::stof(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "sphere") == 0)
    {
        geraEsfera(std::stof(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]), argv[5]);
    }
    else if (strcmp(argv[1], "cone") == 0)
    {
        geraCone(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "cylinder") == 0)
    {
        geraCilindro(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "torus") == 0)
    {
        geraTorus(std::stof(argv[2]), std::stof(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "bezier") == 0)
    {
        lerFicheiroPatch(argv[2], std::atoi(argv[3]), argv[4]);
    }
    else
    {
        std::cerr << "Opção inválida: " << argv[1] << std::endl;
        return 1;
    }
    return 0;
}
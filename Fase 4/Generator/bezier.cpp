#include "Headers/bezier.h"
using namespace std;

int numPedacos, numPontosTotais;
vector<vector<int>> indices;
vector<Ponto> PontosTotais;

Ponto calcularNormal(Ponto p1, Ponto p2, Ponto p3) {
    Ponto v1 = p1 - p2;
    Ponto v2 = p1 - p3;

    Ponto n = v1 * v2;
    n.normalizar();
    
    return n;
}

void lerFicheiroPatch(char *ficheiro, int nivel_tess, char *ficheiroF)
{

    string caminho = "../files/";
    caminho.append(ficheiro);
    ifstream ficheiroPatch(caminho);

    struct stat buffer;
    stat(caminho.c_str(), &buffer);

    string linha;

    getline(ficheiroPatch, linha);
    sscanf(linha.c_str(), "%d", &numPedacos);

    indices.reserve(numPedacos);
    int i;

    for (i = 0; i < numPedacos; i++)
    {
        indices.push_back(vector<int>());
        indices.at(i).reserve(16);
        getline(ficheiroPatch, linha);
        char *str = strdup(linha.c_str());
        char *aux;

        while ((aux = strsep(&str, ", ")) != NULL)
        {
            if (strlen(aux) == 0)
                ;
            else
            {
                int valor;
                sscanf(aux, "%d", &valor);
                indices.at(i).push_back(valor);
            }
        }

        free(str);
    }

    getline(ficheiroPatch, linha);
    sscanf(linha.c_str(), "%d", &numPontosTotais);

    PontosTotais.reserve(numPontosTotais);
    int j;

    for (j = 0; j < numPontosTotais; j++)
    {
        getline(ficheiroPatch, linha);
        float x, y, z;
        sscanf(linha.c_str(), "%f, %f, %f", &x, &y, &z);
        // printf("%s;%f,%f,%f\n", linha.c_str(), x, y, z);
        PontosTotais.emplace_back(Ponto(x, y, z));
    }

    Forma forma = Forma();

    int i_ped;

    for (i_ped = 0; i_ped < numPedacos; i_ped++)
    {
        vector<int> pedaco = indices[i_ped];
        vector<Ponto> pontos_control;
        vector<Ponto> pontos_grelha;

        int i_pntcntrl;

        for (i_pntcntrl = 0; i_pntcntrl < pedaco.size(); i_pntcntrl++)
        {
            pontos_control.push_back(PontosTotais[pedaco[i_pntcntrl]]);
        }

        

        Matrix<float> mat = {{-1.0f, 3.0f, -3.0f, 1.0f},
                             {3.0f, -6.0f, 3.0f, 0.0f},
                             {-3.0f, 3.0f, 0.0f, 0.0f},
                             {1.0f, 0.0f, 0.0f, 0.0f}};

        Matrix<float> mat_px = {{pontos_control[0].getX(), pontos_control[1].getX(), pontos_control[2].getX(), pontos_control[3].getX()},
                                {pontos_control[4].getX(), pontos_control[5].getX(), pontos_control[6].getX(), pontos_control[7].getX()},
                                {pontos_control[8].getX(), pontos_control[9].getX(), pontos_control[10].getX(), pontos_control[11].getX()},
                                {pontos_control[12].getX(), pontos_control[13].getX(), pontos_control[14].getX(), pontos_control[15].getX()}};

        Matrix<float> mat_py = {{pontos_control[0].getY(), pontos_control[1].getY(), pontos_control[2].getY(), pontos_control[3].getY()},
                                {pontos_control[4].getY(), pontos_control[5].getY(), pontos_control[6].getY(), pontos_control[7].getY()},
                                {pontos_control[8].getY(), pontos_control[9].getY(), pontos_control[10].getY(), pontos_control[11].getY()},
                                {pontos_control[12].getY(), pontos_control[13].getY(), pontos_control[14].getY(), pontos_control[15].getY()}};

        Matrix<float> mat_pz = {{pontos_control[0].getZ(), pontos_control[1].getZ(), pontos_control[2].getZ(), pontos_control[3].getZ()},
                                {pontos_control[4].getZ(), pontos_control[5].getZ(), pontos_control[6].getZ(), pontos_control[7].getZ()},
                                {pontos_control[8].getZ(), pontos_control[9].getZ(), pontos_control[10].getZ(), pontos_control[11].getZ()},
                                {pontos_control[12].getZ(), pontos_control[13].getZ(), pontos_control[14].getZ(), pontos_control[15].getZ()}};

        Matrix<float> mat_x = mat * mat_px * mat;
        Matrix<float> mat_y = mat * mat_py * mat;
        Matrix<float> mat_z = mat * mat_pz * mat;

        float inc = 1.0f / nivel_tess;
        float v = 0, u = 0;
        for (int vi = 0 ; vi <= nivel_tess ; vi++,v += inc)
        {
            u = 0;
            for (int ui=0; ui <= nivel_tess; ui++, u += inc)
            {
                Matrix<float> mat_u = {{u * u * u, u * u, u, 1.0f}};

                Matrix<float> mat_v = {{v * v * v},
                                       {v * v},
                                       {v},
                                       {1.0f}};

                float x_valor = (mat_u * mat_x * mat_v)[0][0];
                float y_valor = (mat_u * mat_y * mat_v)[0][0];
                float z_valor = (mat_u * mat_z * mat_v)[0][0];

                pontos_grelha.push_back(Ponto(x_valor, y_valor, z_valor));
            }
        }

        int v_ind, u_ind, pontos_porlinha;
        pontos_porlinha = nivel_tess + 1;
        float t = 0.0f;
        for (u_ind = 0; u_ind < nivel_tess; u_ind++, t+=inc)
        {
            float s = 0.0f;
            for (v_ind = 0; v_ind < nivel_tess; v_ind++, s+=inc)
            {
                Ponto p0 = pontos_grelha[u_ind + v_ind * pontos_porlinha];
                Ponto p1 = pontos_grelha[u_ind + 1 + v_ind * pontos_porlinha];
                Ponto p2 = pontos_grelha[u_ind + (v_ind + 1) * pontos_porlinha];
                Ponto p3 = pontos_grelha[u_ind + 1 + (v_ind + 1) * pontos_porlinha];

                Ponto2D t0 = Ponto2D(s, t),
                        t1 = Ponto2D(s, t+inc),
                        t2 = Ponto2D(s+inc, t),
                        t3 = Ponto2D(s+inc, t+inc);

                forma.adicionarPonto(p0);
                forma.adicionarNormal(calcularNormal(p0, p2, p1));
                forma.adicionarCoordTextura(t0);
                forma.adicionarPonto(p2);
                forma.adicionarNormal(calcularNormal(p1, p0, p2));
                forma.adicionarCoordTextura(t2);
                forma.adicionarPonto(p1);
                forma.adicionarNormal(calcularNormal(p2, p1, p0));
                forma.adicionarCoordTextura(t1);
                forma.adicionarPonto(p1);
                forma.adicionarNormal(calcularNormal(p1, p2, p3));
                forma.adicionarCoordTextura(t1);
                forma.adicionarPonto(p2);
                forma.adicionarNormal(calcularNormal(p3, p1, p2));
                forma.adicionarCoordTextura(t2);
                forma.adicionarPonto(p3);
                forma.adicionarNormal(calcularNormal(p2, p3, p1));
                forma.adicionarCoordTextura(t3);
            }
        }
    }
    forma.escreverParaFicheiro(ficheiroF);
}
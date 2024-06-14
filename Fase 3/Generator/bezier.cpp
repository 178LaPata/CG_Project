#include "Headers/bezier.h"
#include "../Comum/Headers/ponto.h"
#include "../Comum/Headers/matriz.h"
#include "../Comum/Headers/forma.h"
using namespace std;

int numPedacos, numPontosTotais;
vector<vector<int>> indices;
vector<Ponto> PontosTotais;

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
    printf("%d\n", numPedacos);
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
        float v, u;
        int v_ind, u_ind;
        for (u_ind = 0; u_ind < nivel_tess; u_ind++)
        {
            for (v_ind = 0; v_ind < nivel_tess; v_ind++)
            {
                v = v_ind * inc;
                u = u_ind * inc;
                Matrix<float> mat_u = {{u * u * u, u * u, u, 1.0f}};

                Matrix<float> mat_v = {{v * v * v},
                                       {v * v},
                                       {v},
                                       {1.0f}};

                float x_valor = (mat_u * mat_x * mat_v)[0][0];
                float y_valor = (mat_u * mat_y * mat_v)[0][0];
                float z_valor = (mat_u * mat_z * mat_v)[0][0];

                if (x_valor == 0 && y_valor == 0 && z_valor == 0)
                {
                    printf("u:%f v:%f i:%d\n", u, v, i_ped);
                    for (auto p : pontos_control)
                    {
                        printf("%f, %f, %f; ", p.getX(), p.getY(), p.getZ());
                    }
                    printf("\n");
                    std::cout << mat_u << std::endl;
                    std::cout << mat_v << std::endl;
                    std::cout << mat_x << std::endl;
                    std::cout << mat_y << std::endl;
                    std::cout << mat_z << std::endl;
                }

                pontos_grelha.push_back(Ponto(x_valor, y_valor, z_valor));
            }
        }

        int pontos_porlinha;
        pontos_porlinha = nivel_tess + 1;
        for (u_ind = 0; u_ind < nivel_tess; u_ind++)
        {
            for (v_ind = 0; v_ind < nivel_tess; v_ind++)
            {
                Ponto p0 = pontos_grelha[u_ind + v_ind * pontos_porlinha];
                Ponto p1 = pontos_grelha[u_ind + 1 + v_ind * pontos_porlinha];
                Ponto p2 = pontos_grelha[u_ind + (v_ind + 1) * pontos_porlinha];
                Ponto p3 = pontos_grelha[u_ind + 1 + (v_ind + 1) * pontos_porlinha];

                forma.adicionarPonto(p0);
                forma.adicionarPonto(p1);
                forma.adicionarPonto(p2);
                forma.adicionarPonto(p1);
                forma.adicionarPonto(p3);
                forma.adicionarPonto(p2);
            }
        }
    }
    forma.escreverParaFicheiro(ficheiroF);
}
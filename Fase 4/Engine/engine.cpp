#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <map>

#include "Headers/tinyxml2.h"
#include "Headers/camera.h"
#include "Headers/group.h"
#include "Headers/catmullRom.h"
#include "Headers/luz.h"

using namespace tinyxml2;
using namespace std;

GLenum gl_mode = GL_FILL;
Camera *cam = new Camera();
std::vector<Forma *> formas;
std::vector<Group> groups;
std::vector<Luz> luzes;
std::vector<GLuint> luzesIds = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
float alfa, beta_angle, radius;

map<string, GLuint> vbos; // associar ficheiro de modelo ao seu vbo
map<string, GLuint> normais; // associar ficheiro de modelo ao seu vbo
map<string, GLuint> coordsText; // associar ficheiro de modelo ao seu vbo
map<string, GLuint> textures;
float transfBaseTime, transfTime;
int timebase;
float frames, timePassed;
int updatesPerSec = 144;

// declaracao das funcÃµes
Forma *readFile(string fich);
void lerXML(string file);
Group lerGroupXML(XMLElement *file);
void draw(Forma *f);
void drawGroup(Group g);

void createModelVBO(Forma *f)
{
    string ficheiro = f->getFicheiro();
    if (vbos.find(ficheiro) == vbos.end())
    {
        GLuint vboID, normalsVBO, texCoordVBO;

        vector<Ponto> points = f->getPontos();
        vector<Ponto> normals = f->getNormais();
        vector<Ponto2D> texCoords = f->getCoordTextura();

        vector<float> pointsFloat;
        pointsFloat.reserve(points.size() * 3);
        vector<float> normalsFloat;
        normalsFloat.reserve(points.size() * 3);
        vector<float> coordTextFloat;
        coordTextFloat.reserve(points.size() * 2);

        for (int i = 0; i < points.size(); i++)
        {
            Ponto p = points[i];
            Ponto n = normals[i];
            Ponto2D tc = texCoords[i];

            pointsFloat.push_back(p.getX());
            pointsFloat.push_back(p.getY());
            pointsFloat.push_back(p.getZ());
            normalsFloat.push_back(n.getX());
            normalsFloat.push_back(n.getY());
            normalsFloat.push_back(n.getZ());
            coordTextFloat.push_back(tc.getX());
            coordTextFloat.push_back(tc.getY());
        }

        glGenBuffers(1, &vboID);
        glGenBuffers(1, &normalsVBO);
        glGenBuffers(1, &texCoordVBO);

        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pointsFloat.size(), pointsFloat.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsFloat.size(), normalsFloat.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coordTextFloat.size(), coordTextFloat.data(), GL_STATIC_DRAW);
        

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        f->setVBOID(vboID);
        vbos[ficheiro] = vboID;
        normais[ficheiro] = normalsVBO;
        coordsText[ficheiro] = texCoordVBO;
        points.clear();
    }
    else
        f->setVBOID(vbos[ficheiro]);
}

void createTexture(string textureFile)
{
    if (textures.count(textureFile) != 0)
        return;

    unsigned int t, tw, th;
    unsigned char *texData;
    unsigned int texID;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)("../test_files_phase_4/" + textureFile).c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    textures[textureFile] = texID;
}

Forma *readFile(string fich)
{
    ifstream file;
    file.open(fich.c_str());
    if (file.is_open())
    {
        string line;
        Forma *f = new Forma(fich);
        getline(file, line);
        istringstream iss(line);
        float numP;
        iss >> numP;
        for (int i=0 ; i<numP ; i++)
        {
            getline(file, line);
            iss = istringstream(line);
            float x, y, z;
            if (!(iss >> x >> y >> z))
            {
                cout << "Erro ao ler ponto do ficheiro " << fich << endl;
                continue;
            }
            Ponto p = Ponto(x, y, z);
            f->adicionarPonto(p);
        }

        getline(file, line);
        iss = istringstream(line);
        float numN;
        iss >> numN;
        for (int i=0 ; i<numN ; i++)
        {
            getline(file, line);
            iss = istringstream(line);
            float x, y, z;
            if (!(iss >> x >> y >> z))
            {
                cout << "Erro ao ler ponto do ficheiro " << fich << endl;
                continue;
            }
            Ponto p = Ponto(x, y, z);
            f->adicionarNormal(p);
        }

        getline(file, line);
        iss = istringstream(line);
        float numCT;
        iss >> numCT;
        for (int i=0 ; i<numCT ; i++)
        {
            getline(file, line);
            iss = istringstream(line);
            float x, y;
            if (!(iss >> x >> y))
            {
                cout << "Erro ao ler ponto do ficheiro " << fich << endl;
                continue;
            }
            Ponto2D p = Ponto2D(x, y);
            f->adicionarCoordTextura(p);
        }
        file.close();
        createModelVBO(f);
        return f;
    }
    else
    {
        cout << "Erro ao abrir o ficheiro " << fich << endl;
        return NULL;
    }
}

void lerXML(string file)
{
    XMLDocument doc;

    if (doc.LoadFile(file.c_str()) == XML_SUCCESS)
    {
        XMLElement *raiz = doc.FirstChildElement("world");

        XMLElement *window = raiz->FirstChildElement("window");
        int width = window->IntAttribute("width");
        int height = window->IntAttribute("height");

        XMLElement *camera = raiz->FirstChildElement("camera");
        for (XMLElement *elem = camera->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
        {
            if (strcmp(elem->Name(), "position") == 0)
            {
                Ponto *p = new Ponto(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"));
                cam->setPosition(p);
            }
            else if (strcmp(elem->Name(), "lookAt") == 0)
            {
                Ponto *p = new Ponto(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"));
                cam->setLookAt(p);
            }
            else if (strcmp(elem->Name(), "up") == 0)
            {
                Ponto *p = new Ponto(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"));
                cam->setUp(p);
            }
            else if (strcmp(elem->Name(), "projection") == 0)
            {
                cam->setFov(elem->FloatAttribute("fov"));
                cam->setNear(elem->FloatAttribute("near"));
                cam->setFar(elem->FloatAttribute("far"));
            }
        }

        XMLElement *lights = raiz->FirstChildElement("lights");

        if (lights)
        {
            int i=0;
            for (XMLElement *light = lights->FirstChildElement("light"); light && i<8 ; light = light->NextSiblingElement("light"))
            {
                const char *type = light->Attribute("type");
                Luz luz = Luz(LUZ_DIRECIONAL, Ponto());
                if (strcmp(type, "directional") == 0)
                {
                    luz = Luz(LUZ_DIRECIONAL, Ponto(stof(light->Attribute("dirx"), NULL),
                                                    stof(light->Attribute("diry"), NULL),
                                                    stof(light->Attribute("dirz"), NULL)));
                }
                else if (strcmp(type, "point") == 0)
                {
                    luz = Luz(LUZ_PONTO, Ponto(stof(light->Attribute("posx"), NULL),
                                                    stof(light->Attribute("posy"), NULL),
                                                    stof(light->Attribute("posz"), NULL)));
                }
                else
                {
                    luz = Luz(LUZ_FOCO, Ponto(stof(light->Attribute("dirx"), NULL),
                                                    stof(light->Attribute("diry"), NULL),
                                                    stof(light->Attribute("dirz"), NULL)));
                    luz.definirPosicao(Ponto(stof(light->Attribute("posx"), NULL),
                                                    stof(light->Attribute("posy"), NULL),
                                                    stof(light->Attribute("posz"), NULL)));
                    luz.definirCortarFoco(stof(light->Attribute("cutoff"), NULL));
                }
                luz.definirIdentificadorLuz(luzesIds[i++]);
                luzes.push_back(luz);
            }
        }

        XMLElement *group = raiz->FirstChildElement("group");
        while (group)
        {
            Group g = lerGroupXML(group);
            groups.push_back(g);
            group = group->NextSiblingElement("group");
        }
    }
    else
    {
        cout << "Erro ao carregar o ficheiro XML" << endl;
    }
}


Material parseMaterial(XMLElement *node)
{
    if (!node)
        return Material();

    Material m;

    auto child = node->FirstChildElement("diffuse");
    if (child)
    {
        m.setDiffuse(Ponto(strtof(child->Attribute("R"), NULL) / 255.0f,
                           strtof(child->Attribute("G"), NULL) / 255.0f,
                           strtof(child->Attribute("B"), NULL) / 255.0f));
    }

    child = node->FirstChildElement("ambient");
    if (child)
    {
        m.setAmbient(Ponto(strtof(child->Attribute("R"), NULL) / 255.0f,
                           strtof(child->Attribute("G"), NULL) / 255.0f,
                           strtof(child->Attribute("B"), NULL) / 255.0f));
    }

    child = node->FirstChildElement("specular");
    if (child)
    {
        m.setSpecular(Ponto(strtof(child->Attribute("R"), NULL) / 255.0f,
                            strtof(child->Attribute("G"), NULL) / 255.0f,
                            strtof(child->Attribute("B"), NULL) / 255.0f));
    }

    child = node->FirstChildElement("emissive");
    if (child)
    {
        m.setEmissive(Ponto(strtof(child->Attribute("R"), NULL) / 255.0f,
                            strtof(child->Attribute("G"), NULL) / 255.0f,
                            strtof(child->Attribute("B"), NULL) / 255.0f));
    }

    child = node->FirstChildElement("shininess");
    if (child)
    {
        m.setShininess(strtof(child->Attribute("value"), NULL));
    }

    return m;
}

Group lerGroupXML(XMLElement *file)
{
    Group g = Group();
    for (XMLElement *elem = file->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
    {
        if (strcmp(elem->Name(), "transform") == 0)
        {
            for (XMLElement *trans = elem->FirstChildElement() ; trans != NULL ; trans = trans->NextSiblingElement()) {

                if (strcmp(trans->Name(), "rotate") == 0)
                {
                    const XMLAttribute *x = trans->FindAttribute("x");
                    float x_r;
                    if (x)
                        x_r = atof(x->Value());
                    else
                        x_r = 0;

                    const XMLAttribute *y = trans->FindAttribute("y");
                    float y_r;
                    if (y)
                        y_r = atof(y->Value());
                    else
                        y_r = 0;

                    const XMLAttribute *z = trans->FindAttribute("z");
                    float z_r;
                    if (z)
                        z_r = atof(z->Value());
                    else
                        z_r = 0;

                    const XMLAttribute *time = trans->FindAttribute("time");
                    float angle_r = 0, time_r = 0;
                    if (time)
                    {
                        time_r = atof(time->Value());
                        // g.addRotateDynamic(time_r, x_r, y_r, z_r); TODO:
                    }
                    else
                    {
                        const XMLAttribute *angle = trans->FindAttribute("angle");
                        if (angle)
                            angle_r = atof(angle->Value());
                        else
                            angle_r = 0;
                    }
                    if (!time)
                        g.addRotate(angle_r, x_r, y_r, z_r);
                    else
                        g.addTimeRotate(time_r, x_r, y_r, z_r);
                }
                else if (strcmp(trans->Name(), "translate") == 0)
                {
                    const XMLAttribute *time = trans->FindAttribute("time");
                    if (time)
                    {
                        float time_t = atof(time->Value());
                        std::vector<Ponto> pontos;
                    }

                    const XMLAttribute *x = trans->FindAttribute("x");
                    float x_t;
                    if (x)
                        x_t = atof(x->Value());
                    else
                        x_t = 0;

                    const XMLAttribute *y = trans->FindAttribute("y");
                    float y_t;
                    if (y)
                        y_t = atof(y->Value());
                    else
                        y_t = 0;

                    const XMLAttribute *z = trans->FindAttribute("z");
                    float z_t;
                    if (z)
                        z_t = atof(z->Value());
                    else
                        z_t = 0;

                    g.addTranslate(x_t, y_t, z_t);
                }
                else if (strcmp(trans->Name(), "scale") == 0)
                {
                    const XMLAttribute *x = trans->FindAttribute("x");
                    float x_s;
                    if (x)
                        x_s = atof(x->Value());
                    else
                        x_s = 0;

                    const XMLAttribute *y = trans->FindAttribute("y");
                    float y_s;
                    if (y)
                        y_s = atof(y->Value());
                    else
                        y_s = 0;

                    const XMLAttribute *z = trans->FindAttribute("z");
                    float z_s;
                    if (z)
                        z_s = atof(z->Value());
                    else
                        z_s = 0;

                    g.addScale(x_s, y_s, z_s);
                }
            }
        }
        else if (strcmp(elem->Name(), "color") == 0)
        {
            const XMLAttribute *r = elem->FindAttribute("r");
            float r_c;
            if (r)
                r_c = atof(r->Value());
            else
                r_c = 0;
            const XMLAttribute *ge = elem->FindAttribute("g");
            float g_c;
            if (ge)
                g_c = atof(ge->Value());
            else
                g_c = 0;
            const XMLAttribute *b = elem->FindAttribute("b");
            float b_c;
            if (b)
                b_c = atof(b->Value());
            else
                b_c = 0;

            g.addColor(r_c, g_c, b_c);
        }
        else if (strcmp(elem->Name(), "models") == 0)
        {
            for (XMLElement *modelElem = elem->FirstChildElement("model"); modelElem != NULL; modelElem = modelElem->NextSiblingElement("model"))
            {
                string fich = modelElem->Attribute("file");
                fich = "../files/" + fich;
                cout << "Ficheiro " << fich << " lido" << endl;
                Forma *f = readFile(fich);
                XMLElement *aux = modelElem->FirstChildElement("texture");

                if (aux != NULL)
                { // tem textura
                    string ficheiroTextura = aux->Attribute("file");
                    createTexture(ficheiroTextura);
                    f->setFicheiroTextura(ficheiroTextura);
                    f->setTEXTUREID(textures[ficheiroTextura]);
                }

                /**
                 * fazer parse do material
                 */
                f->setMaterial(parseMaterial(modelElem->FirstChildElement("color")));

                g.addModel(f);
            }
        }
        else if (strcmp(elem->Name(), "group") == 0)
        {
            Group g2 = lerGroupXML(elem);
            g.addGroup(g2);
        }
    }
    return g;
}


void changeSize(int w, int h)
{

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(cam->getFov(), ratio, cam->getNear(), cam->getFar());

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius * cos(beta_angle) * sin(alfa), radius * sin(beta_angle), radius * cos(beta_angle) * cos(alfa),
              cam->getLookAt()->getX(), cam->getLookAt()->getY(), cam->getLookAt()->getZ(),
              cam->getUp()->getX(), cam->getUp()->getY(), cam->getUp()->getZ());

    glPolygonMode(GL_FRONT, gl_mode);

    glDisable(GL_LIGHTING);

    // put axis drawing in here
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(10000.0f, 0.0f, 0.0f);
    glVertex3f(-10000.0f, 0.0f, 0.0f);
    // Y Axis in green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 10000.0f, 0.0f);
    glVertex3f(0.0f, -10000.0f, 0.0f);
    // Z Axis in blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 10000.0f);
    glVertex3f(0.0f, 0.0f, -10000.0f);
    glEnd();

    glEnable(GL_LIGHTING);


    for (auto l : luzes)
        l.aplicar();

    // put drawing instructions here
    for (Group g : groups)
    {
        drawGroup(g);
    }

    timePassed = glutGet(GLUT_ELAPSED_TIME);
    frames++;
    // printf("inc : %f, %f %f\n", 1.0f/updatesPerSec, timePassed - transfBaseTime, 1000.0f / updatesPerSec);
    if (timePassed - transfBaseTime >= 1000.0f / updatesPerSec)
    {
        transfTime += 1.0f / updatesPerSec;
        transfBaseTime = timePassed;
    }

    // End of frame
    glutSwapBuffers();
}

void drawForma(Forma *f)
{
    vector<Ponto> pontos = f->getPontos();
    // TODO: mudar para VBO
    // for (int j = 0; j < pontos.size(); j += 3)
    // {
    //     glBegin(GL_TRIANGLES);
    //     glVertex3f(pontos[j].getX(), pontos[j].getY(), pontos[j].getZ());
    //     glVertex3f(pontos[j + 1].getX(), pontos[j + 1].getY(), pontos[j + 1].getZ());
    //     glVertex3f(pontos[j + 2].getX(), pontos[j + 2].getY(), pontos[j + 2].getZ());
    //     glEnd();
    // }

    // vector<Ponto> normals = f->getNormais();
    // glBegin(GL_LINES);
    //     for (int i=0 ; i<normals.size() ; i++) {
    //         Ponto p2 = pontos[i] + normals[i]*0.2;
    //         glVertex3f(pontos[i].getX(), pontos[i].getY(), pontos[i].getZ());
    //         glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    //     }
    // glEnd();

    // printf("%d %d\n", f->getVBOID(), f->getPontos().size());

    string shapeFile = f->getFicheiro();

    // bind dos pontos
    glBindBuffer(GL_ARRAY_BUFFER, vbos[shapeFile]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // bind das normais
    glBindBuffer(GL_ARRAY_BUFFER, normais[shapeFile]);
    glNormalPointer(GL_FLOAT, 0, 0);

    // bind das coordenadas de textura
    glBindBuffer(GL_ARRAY_BUFFER, coordsText[shapeFile]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    if (shapeFile != "") {
        int texID = textures[f->getFicheiroTextura()];
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    Material m = f->getMaterial();
    Ponto diffuse = m.getDiffuse(),
            ambient = m.getAmbient(),
            specular = m.getSpecular(),
            emissive = m.getEmissive();
    
    float dif[] = {diffuse.getX(), diffuse.getY(), diffuse.getZ(), 1.0f},
            amb[] = {ambient.getX(), ambient.getY(), ambient.getZ(), 1.0f},
            spec[] = {specular.getX(), specular.getY(), specular.getZ(), 1.0f},
            emis[] = {emissive.getX(), emissive.getY(), emissive.getZ(), 1.0f};
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emis);
    glMaterialf(GL_FRONT, GL_SHININESS, m.getShininess());

    glDrawArrays(GL_TRIANGLES, 0, f->getPontos().size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (shapeFile != "") {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void drawGroup(Group g)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    vector<Transformation *> transformations = g.getTransformations();
    for (Transformation *t : transformations)
    {
        t->apply(transfTime);
    }
    Color *color = g.getColor();
    if (color)
        glColor3f(color->getR() / 255.0f, color->getG() / 255.0f, color->getB() / 255.0f);

    vector<Forma *> models = g.getModels();
    for (Forma *f : models)
    {
        drawForma(f);
    }

    vector<Group> groups = g.getGroups();
    for (Group g2 : groups)
    {
        drawGroup(g2);
    }
    glPopMatrix();
}

// write function to process keyboard events
void processKeys(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        beta_angle += 0.1;
        break;
    case 's':
        beta_angle -= 0.1;
        break;
    case 'a':
        alfa -= 0.1;
        break;
    case 'd':
        alfa += 0.1;
        break;
    case 'q':
        exit(0);
        break;
    case 'z':
        radius *= 1.5;
        break;
    case 'x':
        radius /= 1.5;
        break;
    case '1':
        gl_mode = GL_LINE;
        break;
    case '2':
        gl_mode = GL_FILL;
        break;
    case '3':
        gl_mode = GL_POINT;
        break;
    }
    if (beta_angle > M_PI_2)
        beta_angle = M_PI_2 - 0.01;
    if (beta_angle < -M_PI_2)
        beta_angle = -M_PI_2 + 0.01;
    if (radius <= 0)
        radius = 1;
}

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        return 1;
    }
    if (!strcmp(argv[1], "-h"))
    {
        return 0;
    }

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(processKeys);

#ifndef __APPLE__
    glewInit();
#endif

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnableClientState(GL_ARRAY_BUFFER);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_RESCALE_NORMAL);

    printf("Inicializou\n");

    char caminho[1024];
    strcpy(caminho, "../test_files_phase_4/");
    strcat(caminho, argv[1]);
    lerXML(caminho);

    Ponto *lookat = cam->getLookAt();
    // cout << lookat->getX() << " " << lookat->getY() << " " << lookat->getZ() << endl;
    // cout << cam->getFov() << " " << cam->getNear() << " " << cam->getFar() << endl;
    // cout << cam->getPosition()->getX() << " " << cam->getPosition()->getY() << " " << cam->getPosition()->getZ() << endl;
    // cout << cam->getUp()->getX() << " " << cam->getUp()->getY() << " " << cam->getUp()->getZ() << endl;

    radius = sqrt(pow(cam->getPosition()->getX(), 2) + pow(cam->getPosition()->getY(), 2) + pow(cam->getPosition()->getZ(), 2));
    alfa = atan(cam->getPosition()->getZ() / cam->getPosition()->getX());
    beta_angle = atan(cam->getPosition()->getY() / sqrt(pow(cam->getPosition()->getX(), 2) + pow(cam->getPosition()->getZ(), 2)));

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
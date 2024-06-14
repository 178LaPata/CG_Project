#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#include "Headers/tinyxml2.h"
#include "Headers/camera.h"
#include "Headers/group.h"

using namespace tinyxml2;
using namespace std;

GLenum gl_mode = GL_LINE;
Camera *cam = new Camera();
std::vector<Forma *> formas;
std::vector<Group> groups;
float alfa, beta_angle, radius;

// declaracao das funcões
Forma *readFile(string fich);
void lerXML(string file);
Group lerGroupXML(XMLElement *file);
void draw(Forma *f);
void drawGroup(Group g);

Forma *readFile(string fich)
{
    ifstream file;
    file.open(fich.c_str());
    if (file.is_open())
    {
        string line;
        Forma *f = new Forma();
        while (getline(file, line))
        {
            istringstream iss(line);
            float x, y, z;
            if (!(iss >> x >> y >> z))
            {
                cout << "Erro ao ler ponto do ficheiro " << fich << endl;
                continue;
            }
            Ponto p = Ponto(x, y, z);
            f->adicionarPonto(p);
        }
        file.close();
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

Group lerGroupXML(XMLElement *file)
{
    Group g = Group();
    for (XMLElement *elem = file->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
    {
        if (strcmp(elem->Name(), "transform") == 0)
        {
            XMLElement *rotate = elem->FirstChildElement("rotate");
            if (rotate)
            {
                const XMLAttribute *angle = rotate->FindAttribute("angle");
                float angle_r;
                if (angle)
                    angle_r = atof(angle->Value());
                else
                    angle_r = 0;

                const XMLAttribute *x = rotate->FindAttribute("x");
                float x_r;
                if (x)
                    x_r = atof(x->Value());
                else
                    x_r = 0;

                const XMLAttribute *y = rotate->FindAttribute("y");
                float y_r;
                if (y)
                    y_r = atof(y->Value());
                else
                    y_r = 0;

                const XMLAttribute *z = rotate->FindAttribute("z");
                float z_r;
                if (z)
                    z_r = atof(z->Value());
                else
                    z_r = 0;

                g.addRotate(angle_r, x_r, y_r, z_r);
            }
            XMLElement *translate = elem->FirstChildElement("translate");
            if (translate)
            {
                const XMLAttribute *x = translate->FindAttribute("x");
                float x_t;
                if (x)
                    x_t = atof(x->Value());
                else
                    x_t = 0;

                const XMLAttribute *y = translate->FindAttribute("y");
                float y_t;
                if (y)
                    y_t = atof(y->Value());
                else
                    y_t = 0;

                const XMLAttribute *z = translate->FindAttribute("z");
                float z_t;
                if (z)
                    z_t = atof(z->Value());
                else
                    z_t = 0;

                g.addTranslate(x_t, y_t, z_t);
            }
            XMLElement *scale = elem->FirstChildElement("scale");
            if (scale)
            {
                const XMLAttribute *x = scale->FindAttribute("x");
                float x_s;
                if (x)
                    x_s = atof(x->Value());
                else
                    x_s = 0;

                const XMLAttribute *y = scale->FindAttribute("y");
                float y_s;
                if (y)
                    y_s = atof(y->Value());
                else
                    y_s = 0;

                const XMLAttribute *z = scale->FindAttribute("z");
                float z_s;
                if (z)
                    z_s = atof(z->Value());
                else
                    z_s = 0;

                g.addScale(x_s, y_s, z_s);
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

    // put drawing instructions here
    for (Group g : groups)
    {
        drawGroup(g);
    }

    // End of frame
    glutSwapBuffers();
}

void draw(Forma *f)
{
    vector<Ponto> pontos = f->getPontos();
    for (int j = 0; j < pontos.size(); j += 3)
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(pontos[j].getX(), pontos[j].getY(), pontos[j].getZ());
        glVertex3f(pontos[j + 1].getX(), pontos[j + 1].getY(), pontos[j + 1].getZ());
        glVertex3f(pontos[j + 2].getX(), pontos[j + 2].getY(), pontos[j + 2].getZ());
        glEnd();
    }
}

void drawGroup(Group g)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    vector<Transformation *> transformations = g.getTransformations();
    for (Transformation *t : transformations)
    {
        t->apply();
    }
    Color *color = g.getColor();
    if (color)
        glColor3f(color->getR()/255.0f, color->getG()/255.0f, color->getB()/255.0f);

    vector<Forma *> models = g.getModels();
    for (Forma *f : models)
    {
        draw(f);
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
        radius += 10;
        break;
    case 'x':
        radius -= 10;
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
        beta_angle = M_PI_2-0.01;
    if (beta_angle < -M_PI_2)
        beta_angle = -M_PI_2+0.01;
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

    char caminho[1024];
    strcpy(caminho, "../test_files_phase_2/");
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

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}

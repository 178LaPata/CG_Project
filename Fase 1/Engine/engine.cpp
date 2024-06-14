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
#include "Headers/ponto.h"
#include "Headers/camera.h"
#include "Headers/forma.h"

using namespace tinyxml2;
using namespace std;

Camera* cam = new Camera();
std::vector<Forma*> formas;
float alfa, beta_angle, radius;

void readFile(string fich) {
    ifstream file;
    file.open(fich.c_str());
    if (file.is_open()) {
        string line;
        Forma *f = new Forma();
        while (getline(file, line)) {
            istringstream iss(line);
            float x, y, z;
            if (!(iss >> x >> y >> z)) {
                cout << "Erro ao ler ponto do ficheiro " << fich << endl;
                continue;
            }
            Ponto p = Ponto(x, y, z);
            f->adicionarPonto(p); 
        }
        formas.push_back(f);
        file.close();
    } else {
        cout << "Erro ao abrir o ficheiro " << fich << endl;
    }
}

void lerXML(string file) {
    XMLDocument doc;

    if (doc.LoadFile(file.c_str()) == XML_SUCCESS) {
        XMLElement* raiz = doc.FirstChildElement();
        XMLElement* window = raiz->FirstChildElement();
        XMLElement* camera = window->NextSiblingElement();
        for (XMLElement* elem = camera->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
            if (strcmp(elem->Name(), "position") == 0) {
                Ponto* p = new Ponto(atof(elem->Attribute("x")), atof(elem->Attribute("y")), atof(elem->Attribute("z")));
                cam->setPosition(p);
            } else if (strcmp(elem->Name(), "lookAt") == 0) {
                Ponto* p = new Ponto(atof(elem->Attribute("x")), atof(elem->Attribute("y")), atof(elem->Attribute("z")));
                cam->setLookAt(p);
            } else if (strcmp(elem->Name(), "up") == 0) {
                Ponto* p = new Ponto(atof(elem->Attribute("x")), atof(elem->Attribute("y")), atof(elem->Attribute("z")));
                cam->setUp(p);
            } else if (strcmp(elem->Name(), "projection") == 0) {
                cam->setFov(elem->FloatAttribute("fov"));
                cam->setNear(elem->FloatAttribute("near"));
                cam->setFar(elem->FloatAttribute("far"));
            }
        }
        XMLElement* group = camera->NextSiblingElement();
        XMLElement* models = group->FirstChildElement();
        for (XMLElement* elem = models->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
            string fich = elem->Attribute("file");
            fich = "../files/" + fich;
            cout << "Ficheiro " << fich << " lido" << endl;
            //cout << elem->Attribute("file") << endl;
            readFile(fich);
        }
    } else {
        cout << "Erro ao carregar o ficheiro XML" << endl;
    }
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
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
    gluPerspective(cam->getFov() ,ratio, cam->getNear() ,cam->getFar());

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void){

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius*cos(beta_angle)*sin(alfa),radius*sin(beta_angle) , radius*cos(beta_angle)*cos(alfa),
              cam->getLookAt()->getX(), cam->getLookAt()->getY(), cam->getLookAt()->getZ(),
              cam->getUp()->getX(), cam->getUp()->getY(), cam->getUp()->getZ());

    glPolygonMode(GL_FRONT, GL_LINE);

// put axis drawing in here
    glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(100.0f,0.0f,0.0f);
        glVertex3f(-100.0f,0.0f,0.0f);
        // Y Axis in green
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(0.0f,100.0f,0.0f);
        glVertex3f(0.0f,-100.0f,0.0f);
        // Z Axis in blue
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0.0f,0.0f,100.0f);
        glVertex3f(0.0f,0.0f,-100.0f);
    glEnd();

// put drawing instructions here
    for (int i = 0; i < formas.size(); i++) {
        Forma *f = formas[i];
        vector<Ponto> pontos = f->getPontos();
        for (int j = 0; j < pontos.size(); j+=3) {
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(pontos[j].getX(), pontos[j].getY(), pontos[j].getZ());
            glVertex3f(pontos[j+1].getX(), pontos[j+1].getY(), pontos[j+1].getZ());
            glVertex3f(pontos[j+2].getX(), pontos[j+2].getY(), pontos[j+2].getZ());
            glEnd();
        }
    }

    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events
void processKeys(unsigned char key, int x, int y) {
    switch (key) {
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
            break;
    }
    if (beta_angle > M_2_PI) beta_angle = M_2_PI;
    if (beta_angle < -M_2_PI) beta_angle = -M_2_PI;
}

int main(int argc, char **argv) {

    if(argc < 2) {
        return 1;
    }
    if(!strcmp(argv[1], "-h")){
        return 0;
    }

    char caminho[1024];
    strcpy(caminho, "../test_files_phase_1/");
    strcat(caminho, argv[1]);
    lerXML(caminho);
    
    Ponto* lookat = cam->getLookAt();
    //cout << lookat->getX() << " " << lookat->getY() << " " << lookat->getZ() << endl;
    //cout << cam->getFov() << " " << cam->getNear() << " " << cam->getFar() << endl;
    //cout << cam->getPosition()->getX() << " " << cam->getPosition()->getY() << " " << cam->getPosition()->getZ() << endl;
    //cout << cam->getUp()->getX() << " " << cam->getUp()->getY() << " " << cam->getUp()->getZ() << endl;

    radius = sqrt(pow(cam->getPosition()->getX(), 2) + pow(cam->getPosition()->getY(), 2) + pow(cam->getPosition()->getZ(), 2));
    alfa = atan(cam->getPosition()->getZ()/cam->getPosition()->getX());
    beta_angle = atan(cam->getPosition()->getY()/sqrt(pow(cam->getPosition()->getX(), 2) + pow(cam->getPosition()->getZ(), 2)));

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");
    glClearColor(0.0f,0.0f,0.0f,0.0f);
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

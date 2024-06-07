// Trabalho feito por Ângelo Antônio Bertoli Guido - 12547677

#include "./source/App.h"

int main(){

    //Create an app.
    App app;

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();
    app.setUpBuilder(100,100,100); //Set world size

    app.run(); //Enter app loop

    return 0;
}
#include "./source/App.h"

int main(){

    App app;

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();

    app.run();

    return 0;
}
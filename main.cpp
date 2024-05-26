#include "./source/App.h"

int main(){

    App app(300,70,300);

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();
    app.setUpBuilder();

    app.run();

    return 0;
}
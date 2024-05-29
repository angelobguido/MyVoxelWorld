#include "./source/App.h"

int main(){

    App app(100,70,100);

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();
    app.setUpBuilder();

    app.run();

    return 0;
}
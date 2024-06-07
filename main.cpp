#include "./source/App.h"

int main(){

    //Create an app with a world size of the desired values.
    App app;

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();
    app.setUpBuilder(100,100,100);

    app.run();

    return 0;
}
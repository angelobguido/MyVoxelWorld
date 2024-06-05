#include "./source/App.h"

int main(){

    //Create an app with a world size of the desired values.
    App app(100,100,100);

    app.setUpOpenGl();
    app.createRenderer();
    app.createCamera();
    app.setUpBuilder();

    app.run();

    return 0;
}
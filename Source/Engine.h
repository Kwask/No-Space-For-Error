#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <windows.h>

#include "Sprite.h"
#include "Tool.h"
#include "Gauge.h"
#include "Machine.h"
#include "gameFuncs.h"
#include "imgLoader.h"

enum Textures{
  TEX_BACKGROUND,
  TEX_CABLES,
  TEX_ENGINE,
  TEX_GAUGE_INDICATORS,
  TEX_GAUGES,
  TEX_PIPES,
  TEX_TOOLS,
  TEX_DRIVE,
  TEX_FIRE,
  TEX_INTRO,
  TEX_WIN,
  TEX_LOSE,
  TEX_TOTAL
};

class Engine{
public:
  static Engine* getInstance();
  virtual ~Engine();

  bool init();
  bool load();
  bool intro();
  bool shouldQuit();

  void open();
  void close();
  void setupMachines();
  void cleanupMachines();
  void handleGauges();
  void handleMachines();
  void handleTools();
  void idle();
  void render();
  void shakeScreen( int mod );
  void setPause( bool pause );
  void togglePause();
  void won();
  void lost();
  void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
  static void keyCallbackSim( GLFWwindow* window, int key, int scancode, int action, int mods );
  void windowResize( GLFWwindow* window, int width, int height );
  static void windowResizeSim( GLFWwindow* window, int width, int height );
  void mouseCursor( GLFWwindow* window, double x, double y );
  static void mouseCursorSim( GLFWwindow* window, double x, double y );
  void mouseButton( GLFWwindow* window, int button, int action, int mod );
  static void mouseButtonSim( GLFWwindow* window, int button, int action, int mod );

private:
  Engine(): SCREEN_WIDTH( 800 ), SCREEN_HEIGHT( 600 ), MAX_FPS( 60 ), ASPECT_RATIO( SCREEN_WIDTH/SCREEN_HEIGHT ), scale( 4.f ){
    open();
  }

  static Engine* instance;

  double SCREEN_WIDTH;
  double SCREEN_HEIGHT;
  double MAX_FPS;
  double ASPECT_RATIO;

  double fps;
  double scale;
  double mouse_x;
  double mouse_y;
  double mouse_x_speed;
  double mouse_y_speed;

  unsigned int iteration_count;

  bool quit;
  bool pause;
  bool win;
  bool lose;
  bool shake;

  Rect<GLfloat> viewport;

  GLFWwindow* window;

  Sprite textures[TEX_TOTAL];

  Tool tools[TOOL_TOTAL];
  Tool* active_tool;

  Gauge gauges[GAUGE_TOTAL];

  std::vector<Machine*> machines;

};

Engine* Engine::instance = 0;

Engine* Engine::getInstance(){
  if( !instance ){
    instance = new Engine;
  }

  return instance;
}

Engine::~Engine(){
  close();
}

bool Engine::init(){
  quit = false;
  pause = true;
  win = false;
  lose = false;
  shake = false;

  fps = 60;
  iteration_count = 0;
  active_tool = 0;

  scale = 4.f;

  viewport.set( 0.f, 0.f, 200, 150 );

  bool success = true;

  if( !glfwInit() ){
    success = false;
  }else{
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "No Space for Error", 0, 0 );

    if( !window ){
      success = false;
    }else{
      glfwMakeContextCurrent( window );
      glfwSetKeyCallback( window, keyCallbackSim );
      glfwSetWindowSizeCallback( window, windowResizeSim );
      glfwSetCursorPosCallback( window, mouseCursorSim );
      glfwSetMouseButtonCallback( window, mouseButtonSim );
    }
  }

  glViewport( viewport.x, viewport.y, SCREEN_WIDTH, SCREEN_HEIGHT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 1.f, -10.f );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glClearColor( 0.f, 0.f, 0.f, 1.f );

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  return success;
}

bool Engine::load(){
  bool success = true;

  Rect<GLfloat> temp( 0, 0, 200, 150 );
  textures->setScale( scale );

  if( !textures[TEX_BACKGROUND].loadFromFile( "resources/Background.png" )){
    success = false;
  }else{
    textures[TEX_BACKGROUND].setRenderBox( temp );

    Rect<GLfloat>* new_temp = new Rect<GLfloat>( 0, 0, 200, 150 );
    textures[TEX_BACKGROUND].setTexClip( new_temp, 1, 200, 150 );
  }

  if( !textures[TEX_CABLES].loadFromFile( "resources/Cables.png" )){
    success = false;
  }

  if( !textures[TEX_ENGINE].loadFromFile( "resources/Engine.png" )){
    success = false;
  }else{
    textures[TEX_ENGINE].setRenderBox( Rect<GLfloat>( 4, 54, 103, 71 ));

    Rect<GLfloat>* new_temp = new Rect<GLfloat>( 0, 0, 103, 71 );
    textures[TEX_BACKGROUND].setTexClip( new_temp, 1, 103, 71 );
  }

  if( !textures[TEX_GAUGE_INDICATORS].loadFromFile( "resources/GaugeIndicator.png" )){
    success = false;
  }else{
    for( int i = 0; i < GAUGE_TOTAL; i++ ){
      gauges[i].setTexIndic( &textures[TEX_GAUGE_INDICATORS] );
    }
  }

  if( !textures[TEX_GAUGES].loadFromFile( "resources/Gauges.png" )){
    success = false;
  }else{
    for( int i = 0; i < GAUGE_TOTAL; i++ ){
      gauges[i].setTexGauge( &textures[TEX_GAUGES] );
      gauges[i].setType( (GaugeType)i );
    }
  }

  if( !textures[TEX_PIPES].loadFromFile( "resources/Pipes.png" )){
    success = false;
  }

  if( !textures[TEX_TOOLS].loadFromFile( "resources/Tools.png" )){
    success = false;
  }else{
    for( int i = 0; i < TOOL_TOTAL; i++ ){
      tools[i].setTexture( &textures[TEX_TOOLS] );
      tools[i].setType( (ToolType)i );

      double pos_x = 20+20*i;
      double pos_y = 110;
      tools[i].setHitbox( Rect<GLfloat>( pos_x, pos_y, 12, 12 ));
      tools[i].setSpeed( -5.f*( rand()%2 )+i, -5.f*( rand()%2 )+i );
    }
  }

  if( !textures[TEX_DRIVE].loadFromFile( "resources/Drive.png" )){
    success = false;
  }

  if( !textures[TEX_FIRE].loadFromFile( "resources/Fire.png" )){
    success = false;
  }

  if( !textures[TEX_WIN].loadFromFile( "resources/Win.png" )){
    success = false;
  }

  if( !textures[TEX_LOSE].loadFromFile( "resources/Lose.png" )){
    success = false;
  }

  return success;
}

bool Engine::intro(){
  bool success = true;

  if( !textures[TEX_INTRO].loadFromFile( "resources/Intro.png" )){
    success = false;
  }else{
    textures[TEX_INTRO].setRenderBox( Rect<GLfloat>( 0, 0, 100, 75 ));
    textures[TEX_INTRO].render();
    glfwSwapBuffers( window );
  }

  if( !load() ){
    success = false;
  };

  return success;
}

bool Engine::shouldQuit(){
  return quit;
}

void Engine::open(){
  unsigned int seed = (int)glfwGetTime();
  srand( seed );

  glfwSetErrorCallback( errorCallback );

  if( !init() ){
    quit = true;
    std::cout << "Failed to initialize!" << std::endl;
  }

  if( !intro() ){
    quit = true;
    std::cout << "Failed to load!" << std::endl;
  }

  setupMachines();
}

void Engine::close(){
  cleanupMachines();

  for( int i = 0; i < TEX_TOTAL; i++ ){
    textures[i].close();
  }

  glfwDestroyWindow( window );
  window = 0;

  glfwTerminate();
}

void Engine::setupMachines(){

  int pipe_count = 4;
  for( int i = 0; i < pipe_count; i++ ){
    machines.push_back( new Machine( MACH_PIPE,
                                     TOOL_WELDER,
                                     &gauges[GAUGE_AIR],
                                     &textures[TEX_PIPES] ));

    double restore = machines.back()->getRestore();
    machines.back()->setRestore( restore/pipe_count );

    machines.back()->setHitbox( Rect<GLfloat>( 124+14*i, 20, 7, 39 ));
  }

  int cable_count = 2;
  for( int i = 0; i < cable_count; i++ ){
    machines.push_back( new Machine( MACH_CABLE,
                                     TOOL_TAPE,
                                     &gauges[GAUGE_POWER],
                                     &textures[TEX_CABLES] ));

    double restore = machines.back()->getRestore();
    machines.back()->setRestore(( restore*1.5f )/( cable_count ) );

    machines.back()->setHitbox( Rect<GLfloat>( 148+14*i, 68, 6, 16 ));
  }

  int fire_count = 5;
  for( int i = 0; i < fire_count; i++ ){
    machines.push_back( new Machine( MACH_FIRE,
                                     TOOL_EXTINGUISHER,
                                     &gauges[GAUGE_TEMP],
                                     &textures[TEX_FIRE] ));

    double restore = machines.back()->getRestore();
    machines.back()->setRestore( restore/( fire_count+2 ));
  }

  unsigned int size = machines.size();
  int fire_scale = 25;
  machines.at( size-5 )->setHitbox( Rect<GLfloat>( 25, 110, fire_scale, fire_scale ));
  machines.at( size-4 )->setHitbox( Rect<GLfloat>( 70, 120, fire_scale, fire_scale ));
  machines.at( size-3 )->setHitbox( Rect<GLfloat>( 150, 77, fire_scale, fire_scale ));
  machines.at( size-2 )->setHitbox( Rect<GLfloat>( 120, 95, fire_scale, fire_scale ));
  machines.at( size-1 )->setHitbox( Rect<GLfloat>( 160, 115, fire_scale, fire_scale ));


  machines.push_back( new Machine( MACH_DRIVE,
                                   TOOL_WRENCH,
                                   &gauges[GAUGE_DRIVE],
                                   &textures[TEX_DRIVE] ));
  machines.back()->setHitbox( Rect<GLfloat>( 4, 56, 94, 59 ));
  machines.back()->setRestore( machines.back()->getRestore()*3.f );
}

void Engine::cleanupMachines(){
  while( !machines.empty() ){
    delete machines.back();
    machines.pop_back();
  }
}

void Engine::handleGauges(){
  double progress_level = gauges[GAUGE_PROGRESS].getLevel();
  double drive_level = gauges[GAUGE_DRIVE].getLevel();
  double new_progress_level = progress_level + ( 0.006f*( drive_level/100.f ));
  gauges[GAUGE_PROGRESS].setLevel( new_progress_level );

  if( gauges[GAUGE_PROGRESS].getLevel() == 100.f ){
    win = true;
  }

  if( gauges[GAUGE_AIR].getLevel() == 0.f ){
    lose = true;
  }

  if( gauges[GAUGE_TEMP].getLevel() == 0.f ){
    lose = true;
  }

  gauges[GAUGE_POWER].setDecay(( gauges[GAUGE_POWER].getLevel()/100.f )*10.f+20.f );

  machines.back()->setRestore(( gauges[GAUGE_POWER].getLevel()/100.f )*90.f );

  for( int i = 0; i < GAUGE_TOTAL; i++ ){
    gauges[i].idle( fps );
  }
}

void Engine::handleMachines(){
  // chance for things to break inceases as progress goes up.
  double break_chance = ( gauges[GAUGE_PROGRESS].getLevel()/2.f );

  for( unsigned int i = 0; i < machines.size(); i++ ){
    machines[i]->idle( fps );

    if( iteration_count%625 == 600 ){
      if( fmod( rand(), 100.f ) < break_chance ){
        machines[i]->setBroken( true );
        shake = true;
      }
    }
  }
}

void Engine::handleTools(){
  for( int i = 0; i < TOOL_TOTAL; i++ ){
    if( active_tool != 0 ){
      Rect<GLfloat> active_box = *active_tool->getHitbox();
      active_box.x = mouse_x-( active_box.w/2.f );
      active_box.y = mouse_y-( active_box.h/2.f );
      active_tool->setHitbox( active_box );
    }

    tools[i].idle( fps );

    Rect<GLfloat> box = *tools[i].getHitbox();
    double speed_x = tools[i].getSpeedX();
    double speed_y = tools[i].getSpeedY();

    double buffer = 5;
    if( box.x < 0 ){
      box.x = 1.f;
      speed_x = ( -1*speed_x )+( fmod( -1*( rand()%2 ), speed_x/4.f ));
      tools[i].setHitbox( box );
      tools[i].setSpeed( speed_x, speed_y );
    }

    if( box.y < 0 ){
      box.y = 0.f;
      speed_y = ( -1*speed_y )+( fmod( -1*( rand()%2 ), speed_y/4.f ));
      tools[i].setHitbox( box );
      tools[i].setSpeed( speed_x, speed_y );
    }

    if( box.x+box.w >= viewport.w ){
      box.x = ( viewport.w-box.w )- 1.f;
      speed_x = ( -1*speed_x )+( fmod( -1*( rand()%2 ), speed_x/4.f ));
      tools[i].setHitbox( box );
      tools[i].setSpeed( speed_x, speed_y );
    }

    if( box.y+box.h >= viewport.h-buffer ){
      box.y = ( viewport.h-box.h )-( buffer+1.f );
      speed_y = ( -1*speed_y )+( fmod( -1*( rand()%2 ), speed_y/4.f ));
      tools[i].setHitbox( box );
      tools[i].setSpeed( speed_x, speed_y );
    }
  }
}

void Engine::idle(){
  double start_time = glfwGetTime();

  glfwPollEvents();

  if( glfwWindowShouldClose( window )){
    quit = true;
  }

  handleGauges();
  handleMachines();
  handleTools();

  render();

  if( win ){
    won();
  }

  if( lose ){
    lost();
  }

  double frame_time = 1.f/MAX_FPS;
  double elapsed_time = glfwGetTime()-start_time;

  if( elapsed_time < frame_time ){
    Sleep(( frame_time-elapsed_time )*1000.f );
  }

  double total_time = glfwGetTime()-start_time;
  fps = 1.f/total_time;

  /*
  if( iteration_count%600 == 1 ){
    std::cout << "FPS " << fps << std::endl;
    std::cout << "Total time: " << total_time << std::endl;
  }
  */

  iteration_count++;
}

void Engine::render(){
  glClear( GL_COLOR_BUFFER_BIT );
  glPushMatrix();

  if( shake ){
    shakeScreen( 10.f );
    if( iteration_count%25 == 24 ){
      shake = false;
    }
  }

  shakeScreen(( gauges[GAUGE_PROGRESS].getLevel()/50.f )*( gauges[GAUGE_DRIVE].getLevel()/100.f ));

  textures[TEX_BACKGROUND].render();
  textures[TEX_ENGINE].render();

  for( int i = 0; i < GAUGE_TOTAL; i++ ){
    gauges[i].render();
  }

  machines.back()->render();
  for( unsigned int i = 0; i < machines.size()-1; i++ ){
    machines[i]->render();
  }

  for( int i = 0; i < TOOL_TOTAL; i++ ){
    tools[i].render();
  }

  glfwSwapBuffers( window );

  glPopMatrix();
}

void Engine::shakeScreen( int mod ){
  double shake_x = (float)mod;
  double shake_y = (float)mod;

  if( rand()%2 ){
    shake_x = shake_x*-1;
    shake_y = shake_y*-1;
  }

  for( int i = 0; i < TOOL_TOTAL; i++ ){
    double speed_x = tools[i].getSpeedX();
    double speed_y = tools[i].getSpeedY();
    speed_x += shake_x;
    speed_y += shake_y;
    tools[i].setSpeed( speed_x, speed_y );
  }

  glTranslated( shake_x, shake_y, 0.f );
}

void Engine::setPause( bool pause ){
  this->pause = pause;
}

void Engine::togglePause(){
  if( pause ){
    pause = false;
  }else{
    pause = true;
  }
}

void Engine::won(){
  textures[TEX_WIN].setRenderBox( Rect<GLfloat>( 0, 0, 200, 150 ));
  textures[TEX_WIN].render();
  glfwSwapBuffers( window );

  quit = true;

  Sleep( 5000 );
}

void Engine::lost(){
  textures[TEX_LOSE].setRenderBox( Rect<GLfloat>( 0, 0, 200, 150 ));
  textures[TEX_LOSE].render();
  glfwSwapBuffers( window );

  quit = true;

  Sleep( 5000 );
}

void Engine::keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ){
  if( action == GLFW_PRESS ){
    if( key == GLFW_KEY_ESCAPE){
      //glfwSetWindowShouldClose( window, GL_TRUE );
      win = true;
    }
  }
}

void Engine::keyCallbackSim( GLFWwindow* window, int key, int scancode, int action, int mods ){
  getInstance()->keyCallback( window, key, scancode, action, mods );
}

void Engine::windowResize( GLFWwindow* window, int width, int height ){
  double ratio1 = width/SCREEN_WIDTH;
  double ratio2 = height/SCREEN_HEIGHT;

  if( ratio1 > ratio2 ){
    scale = ratio2*4.f;
  }else{
    scale = ratio1*4.f;
  }

  textures->setScale( scale );

  viewport.x = ( width/2.f )-(( SCREEN_WIDTH*( scale/4.f ))/2.f );
  viewport.y = ( height/2.f )-(( SCREEN_HEIGHT*( scale/4.f ))/2.f );

  glViewport( viewport.x, viewport.y, width, height );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0.f, width, height, 0.f, 1.f, -10.f );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void Engine::windowResizeSim( GLFWwindow* window, int width, int height ){
  getInstance()->windowResize( window, width, height );
}

void Engine::mouseCursor( GLFWwindow* window, double x, double y ){
  double last_mouse_x = mouse_x;
  double last_mouse_y = mouse_y;

  mouse_x = ( x/scale )-( viewport.x/scale );
  mouse_y = ( y/scale )-( viewport.y/scale );

  mouse_x_speed = mouse_x-last_mouse_x;
  mouse_y_speed = mouse_y-last_mouse_y;

}

void Engine::mouseCursorSim( GLFWwindow* window, double x, double y ){
  getInstance()->mouseCursor( window, x, y );
}

void Engine::mouseButton( GLFWwindow* window, int button, int action, int mod ){
  Rect<GLfloat> mouse_box( mouse_x, mouse_y, 1, 1 );

  if( action == GLFW_PRESS ){
    if( button == GLFW_MOUSE_BUTTON_1 ){
      if( active_tool == 0 ){
        for( int i = 0; i < TOOL_TOTAL; i++ ){
          if( collision( &mouse_box, tools[i].getHitbox() )){
            active_tool = tools+i;
            active_tool->setPickedUp( true );
          }
        }
      }else{
        for( unsigned int i = 0; i < machines.size(); i++ ){
          if( collision( active_tool->getHitbox(), machines[i]->getHitbox() )){
            if( active_tool->getType() == machines[i]->getRepairTool() ){
              machines[i]->setBroken( false );
            }else if(( active_tool->getType() != machines[i]->getRepairTool() ) and machines[i]->getType() != MACH_FIRE ){
              machines[i]->setBroken( true );
            }
          }
        }
      }
    }else if( button == GLFW_MOUSE_BUTTON_2 ){
      if( active_tool != 0 ){
        active_tool->setPickedUp( false );
        active_tool->setSpeed( mouse_x_speed*fps, mouse_y_speed*fps );
        active_tool = 0;
      }
    }
  }
}

void Engine::mouseButtonSim( GLFWwindow* window, int button, int action, int mod ){
  getInstance()->mouseButton( window, button, action, mod );
}


#endif // ENGINE_H_INCLUDED

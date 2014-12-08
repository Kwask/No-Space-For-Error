#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include <string>

#include "Sprite.h"
#include "Rect.h"

enum ToolType{
  TOOL_WELDER,
  TOOL_WRENCH,
  TOOL_TAPE,
  TOOL_EXTINGUISHER,
  TOOL_TOTAL,
  TOOL_UNSET
};

class Tool{
public:
  Tool();
  virtual ~Tool();

  const ToolType getType();

  const double getSpeedX();
  const double getSpeedY();

  const Rect<GLfloat>* getHitbox();

  void open();
  void close();
  void idle( double fps );
  void runPhysics( double fps );
  void render();
  void setTexture( const Sprite* texture );
  void setHitbox( Rect<GLfloat> hitbox );
  void setPickedUp( bool picked_up );
  void setType( ToolType type );
  void setSpeed( double speed_x, double speed_y );

private:
  bool picked_up;

  ToolType type;

  Sprite texture; // Engine will take care of cleanup

  double speed_x;
  double speed_y;

};

Tool::Tool(){
  open();
}

Tool::~Tool(){
  close();
}

const ToolType Tool::getType(){
  return type;
}

const double Tool::getSpeedX(){
  return speed_x;
}

const double Tool::getSpeedY(){
  return speed_y;
}

const Rect<GLfloat>* Tool::getHitbox(){
  return texture.getRenderBox();
}

void Tool::open(){
  setType( TOOL_UNSET );

  picked_up = false;

  speed_x = 0;
  speed_y = 0;
}

void Tool::close(){

}

void Tool::idle( double fps ){
  if( !picked_up ){
    runPhysics( fps );
  }

  texture.setCurrentFrame( type );
}

void Tool::runPhysics( double fps ){
  Rect<GLfloat> hitbox = *texture.getRenderBox();

  speed_x -= speed_x/100.f;
  speed_y -= speed_y/100.f;

  hitbox.x += speed_x/fps;
  hitbox.y += speed_y/fps;

  texture.setRenderBox( hitbox );
}

void Tool::render(){
  texture.render();
}

void Tool::setTexture( const Sprite* texture ){
  this->texture = *texture;
}

void Tool::setHitbox( Rect<GLfloat> hitbox ){
//  std::cout << "X: " << hitbox.x << "  Y: " << hitbox.y << std::endl;
  texture.setRenderBox( hitbox );
}

void Tool::setPickedUp( bool picked_up ){
  this->picked_up = picked_up;
}

void Tool::setType( ToolType type ){
  this->type = type;

  Rect<GLfloat>* temp = new Rect<GLfloat>[TOOL_TOTAL];
  int size = 10;

  for( int i = 0; i < TOOL_TOTAL; i++ ){
    temp[i].set( size*i, 0, size, size );
  }

  texture.setTexClip( temp, TOOL_TOTAL, TOOL_TOTAL*size, size );
  texture.setCurrentFrame( type );

  switch( type ){
    case TOOL_WELDER:{
      std::cout << "Welder tool created!" << std::endl;
      break;
    }
    case TOOL_WRENCH:{
      std::cout << "Wrench tool created!" << std::endl;
      break;
    }
    case TOOL_TAPE:{
      std::cout << "Tape tool created!" << std::endl;
      break;
    }
    case TOOL_EXTINGUISHER:{
      std::cout << "Extinguisher tool created!" << std::endl;
      break;
    }
    default:
      break;
  }
}

void Tool::setSpeed( double speed_x, double speed_y ){
  this->speed_x = speed_x;
  this->speed_y = speed_y;
}

#endif // TOOL_H_INCLUDED

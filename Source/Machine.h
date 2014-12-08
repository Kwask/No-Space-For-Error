#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED

#include "Tool.h"
#include "Gauge.h"

enum MachineType{
  MACH_PIPE,
  MACH_CABLE,
  MACH_FIRE,
  MACH_DRIVE,
  MACH_TOTAL,
  MACH_UNSET
};

class Machine{
public:
  Machine();
  Machine( MachineType type, ToolType repair_tool, Gauge* linked_gauge, Sprite* texture );
  virtual ~Machine();

  const double getRestore();

  const MachineType getType();

  const ToolType getRepairTool();

  const Gauge* getLinkedGauge();

  const Rect<GLfloat>* getHitbox();

  bool isBroken();

  void open();
  void close();
  void render();
  void idle( double fps );
  void setRestore( double restore );
  void setTexture( const Sprite* texture );
  void setHitbox( Rect<GLfloat> hitbox );
  void setType( MachineType type );
  void setRepairTool( ToolType repair_tool );
  void setLinkedGauge( Gauge* linked_gauge );
  void toggleBroken();
  void setBroken( bool broken );

private:
  int unbroken_begin;
  int unbroken_end;
  int broken_begin;
  int broken_end;

  double restore;

  bool broken;
  bool animated_broken;
  bool animated_proper;

  Sprite texture;

  MachineType type;

  ToolType repair_tool;

  Gauge* linked_gauge;

};

Machine::Machine(){
  open();
}

Machine::Machine( MachineType type, ToolType repair_tool, Gauge* linked_gauge, Sprite* texture ){
  open();

  this->texture = *texture;
  setType( type );
  this->repair_tool = repair_tool;
  this->linked_gauge = linked_gauge;
}

Machine::~Machine(){
  close();
}

const double Machine::getRestore(){
  return restore;
}

const MachineType Machine::getType(){
  return type;
}

const ToolType Machine::getRepairTool(){
  return repair_tool;
}

const Gauge* Machine::getLinkedGauge(){
  return linked_gauge;
}

const Rect<GLfloat>* Machine::getHitbox(){
  return texture.getRenderBox();
}

bool Machine::isBroken(){
  return broken;
}

void Machine::open(){
  restore = 30.f;
  broken = false;
  animated_broken = false;
  animated_proper = false;

  type = MACH_UNSET;
  repair_tool = TOOL_UNSET;

  linked_gauge = 0;
}

void Machine::close(){
  broken = false;

  type = MACH_UNSET;
  repair_tool = TOOL_UNSET;

  linked_gauge = 0;
}

void Machine::render(){
  texture.render();
}

void Machine::idle( double fps ){
  if( broken ){
    texture.setFrameRange( broken_begin, broken_end );
    texture.setAnimated( animated_broken );
    texture.setFrameTime( 0.2f );
  }else{
    texture.setFrameRange( unbroken_begin, unbroken_end );
    texture.setAnimated( animated_proper );
    texture.setFrameTime( 0.2f );

    double level = linked_gauge->getLevel();
    level += restore/fps;
    linked_gauge->setLevel( level );

    if( type == MACH_DRIVE ){
      if( level <= 0.f ){
        broken = true;
      }
    }
  }
}

void Machine::setRestore( double restore ){
  this->restore = restore;
}

void Machine::setTexture( const Sprite* texture ){
  this->texture = *texture;
}

void Machine::setHitbox( Rect<GLfloat> hitbox ){
  texture.setRenderBox( hitbox );
}

void Machine::setType( MachineType type ){
  this->type = type;

  int length = 0;
  int height = 0;
  int frames = 0;
  int rows;

  double frame_time;

  switch( type ){
    case MACH_PIPE:
      length = 7;
      height = 39;
      frames = 2;
      rows = 1;

      unbroken_begin = 0;
      unbroken_end = 0;
      broken_begin = 1;
      broken_end = 1;

      animated_proper = false;
      animated_broken = false;

      break;
    case MACH_CABLE:
      length = 6;
      height = 16;
      frames = 5;
      rows = 1;

      unbroken_begin = 0;
      unbroken_end = 0;
      broken_begin = 1;
      broken_end = 4;

      animated_proper = false;
      animated_broken = false;

      break;
    case MACH_FIRE:
      length = 10;
      height = 10;
      frames = 6;
      rows = 1;

      unbroken_begin = 0;
      unbroken_end = 0;
      broken_begin = 1;
      broken_end = 5;

      animated_proper = false;
      animated_broken = true;

      frame_time = 0.1f;

      break;
    case MACH_DRIVE:{
      length = 94;
      height = 59;
      frames = 4;
      rows = 2;

      unbroken_begin = 4;
      unbroken_end = 7;
      broken_begin = 0;
      broken_end = 3;

      animated_proper = true;
      animated_broken = true;

      frame_time = 0.2f;
      break;
    case MACH_TOTAL:
      break;
    case MACH_UNSET:
      close();
      break;
    }
  }

  Rect<GLfloat>* clip = new Rect<GLfloat>[frames*rows];

  for( int row = 0; row < rows; row++ ){
    for( int frame = 0; frame < frames; frame++ ){
      clip[frame + frames*row].set( frame*length, row*height, length, height );
    }
  }

  texture.setTexClip( clip, frames*rows, length*frames, height*rows );
  texture.setFrameTime( frame_time );
}

void Machine::setRepairTool( ToolType repair_tool ){
  this->repair_tool = repair_tool;
}

void Machine::setLinkedGauge( Gauge* linked_gauge ){
  this->linked_gauge = linked_gauge;
}

void Machine::toggleBroken(){
  if( broken ){
    broken = false;
  }else{
    broken = true;
  }
}

void Machine::setBroken( bool broken ){
  this->broken = broken;
}

#endif // PIPE_H_INCLUDED

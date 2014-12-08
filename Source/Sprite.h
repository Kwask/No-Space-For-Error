#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <math.h>

#include "Rect.h"
#include "renderFuncs.h"
#include "imgLoader.h"

class Sprite{
public:
  Sprite();
  Sprite( const Sprite* other );
  virtual ~Sprite();

  const Rect<GLfloat>* getRenderBox();
  const Rect<GLfloat>* getTexClip();

  const double getScale();

  const int getCurrentFrame();

  bool loadFromFile( std::string filename );

  void open();
  void close();
  void freeTexClip();
  void render();
  void copy( const Sprite* other );
  void operator=( const Sprite* other );
  void setScale( GLfloat new_scale );
  void setTexture( GLuint texture );
  void setTexClip( Rect<GLfloat>* tex_clip, int frame_count );
  void setTexClip( Rect<GLfloat>* tex_clip, int frame_count, GLfloat image_w, GLfloat image_h );
  void setFrameRange( int begin, int end );
  void setRenderBox( Rect<GLfloat> render_box );
  void setFrameTime( double frame_time );
  void setCurrentFrame( int current_frame );
  void toggleAnimated();
  void setAnimated( bool animated );

private:
  static GLfloat scale;

  bool animated;

  GLuint texture;

  Rect<GLfloat>* tex_clip;

  Rect<GLfloat> render_box;

  int frame_count;
  int frame_begin;
  int frame_end;
  int current_frame;

  double frame_time;

};

GLfloat Sprite::scale = 8.f;

Sprite::Sprite(){
  open();
}

Sprite::~Sprite(){
  close();
}

const Rect<GLfloat>* Sprite::getRenderBox(){
  return &render_box;
}

const Rect<GLfloat>* Sprite::getTexClip(){
  return tex_clip;
}

const double Sprite::getScale(){
  return scale;
}

const int Sprite::getCurrentFrame(){
  return current_frame;
}

bool Sprite::loadFromFile( std::string filename ){
  bool success = true;

  texture = loadPNG( filename );

  if( texture == 0 ){
    success = false;
    std::cout << "Failed to load " << filename << "!" << std::endl;
  }

  return success;
}

void Sprite::open(){
  animated = false;
  texture = 0;
  tex_clip = 0;
  frame_count = 0;
  current_frame = 0;
  frame_time = 0;
}

void Sprite::close(){
  if( texture != 0 ){
    glDeleteTextures( 1, &texture );
    texture = 0;
  }

  freeTexClip();
}

void Sprite::freeTexClip(){
  if( tex_clip ){
    if( frame_count > 1 ){
      delete[] tex_clip;
    }else{
      delete tex_clip;
    }
  }

  tex_clip = 0;
  frame_count = 0;
  frame_time = 0;
  current_frame = 0;
}

void Sprite::render(){
  Rect<GLfloat> temp( render_box.x*scale, render_box.y*scale, render_box.w*scale, render_box.h*scale );

  drawSquare( &temp, &tex_clip[current_frame], texture );
//  drawSquare( &temp );

  current_frame = frame_begin;
  if( animated ){
    double cycle_time = frame_time*( frame_end+1-frame_begin );
    double current_time = fmod( glfwGetTime(), cycle_time );
    current_frame = ( current_time/frame_time )+frame_begin;
  }
}

void Sprite::setScale( GLfloat new_scale ){
  scale = new_scale;
}

void Sprite::setTexture( GLuint texture ){
  this->texture = texture;
}

void Sprite::setTexClip( Rect<GLfloat>* tex_clip, int frame_count ){
  freeTexClip();

  this->tex_clip = tex_clip;
  this->frame_count = frame_count;

  setFrameRange( 0, frame_count );
}

void Sprite::setTexClip( Rect<GLfloat>* tex_clip, int frame_count, GLfloat w, GLfloat h ){
  for( int i = 0; i < frame_count; i++ ){
    tex_clip[i].set( tex_clip[i].x/w, tex_clip[i].y/h, tex_clip[i].w/w, tex_clip[i].h/h );
  }

  setTexClip( tex_clip, frame_count );
}

void Sprite::setFrameRange( int begin, int end ){
  frame_begin = begin;
  frame_end = end;
}

void Sprite::setRenderBox( Rect<GLfloat> render_box ){
  this->render_box = render_box;
}

void Sprite::setFrameTime( double frame_time ){
  this->frame_time = frame_time;
}

void Sprite::setCurrentFrame( int current_frame ){
  this->current_frame = current_frame;
}

void Sprite::toggleAnimated(){
  if( animated ){
    animated = false;
  }else{
    animated = true;
  }
}

void Sprite::setAnimated( bool animated ){
  this->animated = animated;
}

#endif // SPRITE_H_INCLUDED

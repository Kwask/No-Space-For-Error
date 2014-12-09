#ifndef GAUGE_H_INCLUDED
#define GAUGE_H_INCLUDED

#include <GLFW/glfw3.h>

#include "Rect.h"
#include "Sprite.h"
#include "Indicator.h"

enum GaugeType{
  GAUGE_PROGRESS,
  GAUGE_DRIVE,
  GAUGE_AIR,
  GAUGE_TEMP,
  GAUGE_POWER,
  GAUGE_TOTAL,
  GAUGE_UNSET
};

class Gauge{
public:
  Gauge();
  virtual ~Gauge();

  Sprite* getIndicTexture();

  const Rect<GLfloat>* getHitbox();

  const double getLevel();

  void open();
  void close();
  void render();
  void idle( double fps );
  void setLevel( double level );
  void setType( GaugeType type );
  void setTexGauge( const Sprite* tex_gauge );
  void setTexIndic( const Sprite* tex_indicator );
  void setDecay( double decay );

private:
  GaugeType type;

  Sprite tex_gauge;

  Indicator indicator;

};

Gauge::Gauge(){
  open();
}

Gauge::~Gauge(){
  close();
}

Sprite* Gauge::getIndicTexture(){
  return indicator.getTexture();
}

const Rect<GLfloat>* Gauge::getHitbox(){
  return tex_gauge.getRenderBox();
}

const double Gauge::getLevel(){
  return indicator.getLevel();
}

void Gauge::open(){
  type = GAUGE_UNSET;
}

void Gauge::close(){

}

void Gauge::render(){
  tex_gauge.render();
  indicator.render();
}

void Gauge::idle( double fps ){
  indicator.idle( fps );
}

void Gauge::setLevel( double level ){
  indicator.setLevel( level );
}

void Gauge::setType( GaugeType new_type ){
  GLfloat gauge_width = 60;
  GLfloat gauge_height = 49;

  GLfloat indic_width = 7;
  GLfloat indic_height = 42;

  Sprite* tex_indicator = indicator.getTexture();

  // lots of magic ahead!
  switch( new_type ){
    case GAUGE_PROGRESS:{
      this->type = new_type;

      Rect<GLfloat>* temp = new Rect<GLfloat>( 0, 0, 14, gauge_height );
      tex_gauge.setTexClip( temp, 1, gauge_width, gauge_height );
      tex_gauge.setRenderBox( Rect<GLfloat>( 37, 1, 14, gauge_height ));

      tex_indicator->setRenderBox( Rect<GLfloat>( 38, 7, 12, indic_height ));
      indicator.setDecay( 0.f );
      indicator.setLevel( 0.f );
      std::cout << "Progress Gauge created!" << std::endl;
      break;
    }
    case GAUGE_DRIVE:{
      this->type = new_type;

      Rect<GLfloat>* temp = new Rect<GLfloat>( 14, 0, 11, gauge_height );
      tex_gauge.setTexClip( temp, 1, gauge_width, gauge_height );
      tex_gauge.setRenderBox( Rect<GLfloat>( 57, 1, 11, gauge_height ));

      tex_indicator->setRenderBox( Rect<GLfloat>( 59, 7, indic_width, indic_height ));
      std::cout << "Drive Gauge created!" << std::endl;
      break;
    }
    case GAUGE_AIR:{
      this->type = new_type;

      Rect<GLfloat>* temp = new Rect<GLfloat>( 25, 0, 9, gauge_height );
      tex_gauge.setTexClip( temp, 1, gauge_width, gauge_height );
      tex_gauge.setRenderBox( Rect<GLfloat>( 102, 1, 9, gauge_height ));

      tex_indicator->setRenderBox( Rect<GLfloat>( 103, 7, indic_width, indic_height ));
      std::cout << "Air Gauge created!" << std::endl;
      break;
    }
    case GAUGE_TEMP:{
      this->type = new_type;

      Rect<GLfloat>* temp = new Rect<GLfloat>( 34, 0, 13, gauge_height );
      tex_gauge.setTexClip( temp, 1, gauge_width, gauge_height );
      tex_gauge.setRenderBox( Rect<GLfloat>( 86, 1, 13, gauge_height ));

      tex_indicator->setRenderBox( Rect<GLfloat>( 89, 7, indic_width, indic_height ));
      std::cout << "Temp Gauge created!" << std::endl;
      break;
    }
    case GAUGE_POWER:{
      this->type = new_type;

      Rect<GLfloat>* temp = new Rect<GLfloat>( 47, 0, 13, gauge_height );
      tex_gauge.setTexClip( temp, 1, gauge_width, gauge_height );
      tex_gauge.setRenderBox( Rect<GLfloat>( 69, 1, 13, gauge_height ));

      tex_indicator->setRenderBox( Rect<GLfloat>( 71, 7, indic_width, indic_height ));
      std::cout << "Power Gauge created!" << std::endl;
      break;
    }
    default:
      this->type = GAUGE_UNSET;
      break;
  }
}

void Gauge::setTexGauge( const Sprite* tex_gauge ){
  this->tex_gauge = *tex_gauge;
}

void Gauge::setTexIndic( const Sprite* tex_indicator ){
  indicator.setTexture( tex_indicator );
}

void Gauge::setDecay( double decay ){
  indicator.setDecay( decay );
}

#endif // GAUGE_H_INCLUDED

#ifndef INDICATOR_H_INCLUDED
#define INDICATOR_H_INCLUDED

class Indicator{
public:
  Indicator();
  virtual ~Indicator();

  Sprite* getTexture();

  const double getLevel();

  void open();
  void close();
  void render();
  void idle( double fps );
  void setTexture( const Sprite* texture );
  void setLevel( double level );
  void setDecay( double decay );

private:
  Sprite texture;

  double level;

  double decay;

};

Indicator::Indicator(){
  open();
}

Indicator::~Indicator(){
  close();
}

Sprite* Indicator::getTexture(){
  return &texture;
}

const double Indicator::getLevel(){
  return level;
}

void Indicator::open(){
  level = 100.f;
  decay = 20.f;
}

void Indicator::close(){

}

void Indicator::render(){
  if( level >= 100.f ){
    texture.setCurrentFrame( 5 );
  }else if( level >= 80.f ){
    texture.setCurrentFrame( 4 );
  }else if( level >= 60.f ){
    texture.setCurrentFrame( 3 );
  }else if( level >= 40.f ){
    texture.setCurrentFrame( 2 );
  }else if( level >= 20.f ){
    texture.setCurrentFrame( 1 );
  }else{
    texture.setCurrentFrame( 0 );
  }

  texture.render();
}

void Indicator::idle( double fps ){
  setLevel( level-( decay/fps ));
}

void Indicator::setTexture( const Sprite* new_texture ){
  this->texture = *new_texture;

  GLfloat indic_width = 7.f;
  GLfloat indic_height = 42.f;

  int indic_frame_count = 6;

  Rect<GLfloat>* indic_clip = new Rect<GLfloat>[indic_frame_count];
  for( int j = 0; j < indic_frame_count; j++ ){
    indic_clip[j].set( j*indic_width, 0.f, indic_width, indic_height );
  }

  texture.setTexClip( indic_clip, indic_frame_count, 42.f, indic_height );
}

void Indicator::setLevel( double new_level ){
  if( new_level < 0.f ){
    new_level = 0.f;
  }else if( new_level >= 100.f ){
    new_level = 100.f;
  }

  this->level = new_level;
}

void Indicator::setDecay( double decay ){
  this->decay = decay;
}

#endif // INDICATOR_H_INCLUDED

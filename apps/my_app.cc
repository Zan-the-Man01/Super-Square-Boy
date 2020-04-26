// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <game/location.h>
#include <choreograph/Choreograph.h>

#include <chrono>

// change the name of this, the cinder one is already App
namespace myapp {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::TextBox;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using game::Location;

using cinder::app::KeyEvent;

const seconds kCountdownTime = seconds(2);
const char kNormalFont[] = "Azonix";
const char kBoldFont[] = "Arial Bold";
const char kDifferentFont[] = "Papyrus";
const std::vector<Color> player_colors = {Color(1, 0.501, 0)};
const std::vector<Color> backgr_colors = {Color(0, 0.933, 0.921)};

MyApp::MyApp() : speed_{50}, tile_size_{40} {}

void MyApp::setup() {
  //cinder::gl::enableDepthWrite();
  //cinder::gl::enableDepthRead();

  try {
    cinder::audio::SourceFileRef sourceFile =
        cinder::audio::load(cinder::app::loadAsset("music.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));
    sourceFile =
        cinder::audio::load(cinder::app::loadAsset("music2.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));
  } catch (const std::exception& ex) {
    exit(1);
  }

  SetUpAnimation();
}

void MyApp::SetUpAnimation() {
  // Create a procedural phrase that moves vertically on a sine wave.
  // Procedural phrases can evaluate any function you like.
  ch::PhraseRef<cinder::vec2> bounce = ch::makeProcedure<cinder::vec2>( 2.0, [] ( ch::Time t, ch::Time duration ) {
    return cinder::vec2( 0, sin( ch::easeInOutQuad(t) * 6 * M_PI ) * 400.0f );
  } );

  // Create a ramp phrase from the left to the right side of the window.
  float w = (float)cinder::app::getWindowWidth();
  float x1 = 0;
  float x2 = w;
  ch::PhraseRef<cinder::vec2> slide = makeRamp( cinder::vec2( x1, 0 ),
                                                cinder::vec2( x2, 0 ), 2.0f, ch::EaseInOutCubic() );

  // Combine the slide and bounce phrases using an AccumulatePhrase.
  // By default, the accumulation operation sums all the phrase values with an initial value.
  float center_y = cinder::app::getWindowHeight() / 2.0f;
  ch::PhraseRef<cinder::vec2> bounce_and_slide = makeAccumulator( cinder::vec2( 0, center_y ), bounce, slide );

  // Provide an explicit combine function.
  // In this case, we subtract each value from the initial value.
  ch::PhraseRef<cinder::vec2> bounce_and_slide_negative = makeAccumulator( cinder::vec2( w, center_y ),
                                                                           bounce, slide, [] (const cinder::vec2 &a, const cinder::vec2 &b) {
        return a - b;
      } );

  // Apply our Sequences to Outputs.
  timeline_.apply( &_position_a, bounce_and_slide );
  timeline_.apply( &_position_b, bounce_and_slide_negative );

  // Place Outputs at initial sequence values.
  timeline_.jumpTo( 0 );
}

template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc, const size_t font_size) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, font_size))
      .size(size)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void MyApp::update() {

  timeline_.step( ch::Time(t_));
  if (animation_started_) {
    t_ += 0.0001;
  }

  if (in_main_menu_ || paused_) {
    return;
  }

  const auto time = system_clock::now();
  if (!end_reached_ && engine_.EndReached()) {
    time_of_end_reached_ = time;
    end_reached_ = true;
  }

  if (FadeEnded()) {
    return;
  }

  if (engine_.IsDead()) {
    return;
  }


  if (time - last_time_ > std::chrono::milliseconds(speed_)) {
    engine_.Step();
    last_time_ = time;
  }
}

void MyApp::draw() {
  if (in_main_menu_) {
    //cinder::gl::drawSolidCircle( _position_a, 30.0f );
    //cinder::gl::drawSolidEllipse(_position_a, 30.0f, 20.0f);

    //const Location loc = engine_.GetPlayerSquare().GetLocation();

    //cinder::gl::color( Color( cinder::CM_HSV, 0.96f, 1.0f, 1.0f ) );
    //cinder::gl::drawSolidCircle( _position_b, 30.0f );
    DrawMainMenu();
    return;
  }
  if (paused_) {
    if (!animation_started_) {
      animation_started_ = true;
      t_ = 0;
      SetUpAnimation();
    }

    if (TimeToPrintPauseScreen()) {
      cinder::gl::clear(backgr_colors[0]);
      DrawPauseScreen();
    }

    if (!AnimationEnded()) {
      if (!TimeToPrintPauseScreen()) {
        cinder::gl::clear(backgr_colors[0]);
      }

      cinder::gl::ScopedColor color(player_colors[0]);
      cinder::gl::drawSolidRect(Rectf(_position_a, _position_b));
    }
    return;
  }
  if (FadeEnded()) {
    DrawEndScreen();
    return;
  }

  if (engine_.IsDead()) {
    const auto time = system_clock::now();
    if (just_died_) {
      just_died_ = false;
      time_of_death_ = time;
    } else if (time - time_of_death_ > std::chrono::milliseconds(1000)){
      engine_.Reset();
      just_died_ = true;
    }
  }

  cinder::gl::clear();
  DrawBackground();
  DrawPlayer();
  DrawSquares();
  DrawSpikes();
}

void MyApp::DrawBackground() const {
  cinder::gl::clear(PercentFade(backgr_colors[0]));
}

void MyApp::DrawPlayer() const {
  if (engine_.IsDead()) {
    cinder::gl::color(PercentFade(backgr_colors[0]));
  } else {
    cinder::gl::color(PercentFade(player_colors[0]));
  }

  const Location loc = engine_.GetPlayerSquare().GetLocation();
  cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                  tile_size_ * loc.Col(),
                                  tile_size_ * loc.Row() + tile_size_,
                                  tile_size_ * loc.Col() + tile_size_));
}

void MyApp::DrawSquares() const {
  cinder::gl::color(Color::black());
  const std::vector<Location> locs = engine_.GetFrame().GetSquareLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                    tile_size_ * loc.Col(),
                                    tile_size_ * loc.Row() + tile_size_,
                                    tile_size_ * loc.Col() + tile_size_));
  }
}

void MyApp::DrawSpikes() const {
  cinder::gl::color(Color(1, 0, 0));
  const std::vector<Location> locs = engine_.GetFrame().GetSpikeLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.Row(),
                                    tile_size_ * loc.Col(),
                                    tile_size_ * loc.Row() + tile_size_,
                                    tile_size_ * loc.Col() + tile_size_));
  }
}

void MyApp::DrawEndScreen() const {
  cinder::gl::clear(Color::black());

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];


  PrintText("YOU WIN!", color, size, center, 100);
}

void MyApp::DrawPauseScreen() const {
  //cinder::gl::clear(backgr_colors[0]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;


  PrintText("PAUSED", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: RESUME GAME", color, size, center, 50);
  PrintText("2: MAIN MENU", color, size,
      {center.x, center.y + small_y_offset}, 50);
}

void MyApp::DrawMainMenu() const {
  cinder::gl::clear(backgr_colors[0]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;


  PrintText("SUPER SQUARE BOY", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: LEVEL ONE", color, size, center, 50);
  PrintText("2: LEVEL TWO", color, size,
            {center.x, center.y + small_y_offset}, 50);
  PrintText("3: EXIT", color, size,
            {center.x, center.y + (2 * small_y_offset)}, 50);
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_1: {
      if (in_main_menu_) {
        in_main_menu_ = false;
        engine_.StartLevel(1);
        sound_tracks_[0]->start();
        paused_ = false;
        animation_started_ = false;
        t_ = 0;
        SetUpAnimation();
      }
      if (paused_) {
        paused_ = false;
        animation_started_ = false;
        t_ = 0;
        SetUpAnimation();
      }
      break;
    }
    case KeyEvent::KEY_2: {
      if (in_main_menu_) {
        in_main_menu_ = false;
        engine_.StartLevel(2);
        sound_tracks_[1]->start();
        paused_ = false;
        animation_started_ = false;
        t_ = 0;
        SetUpAnimation();
      }
      if (paused_) {
        in_main_menu_ = true;
        paused_ = false;
        engine_.Reset();
      }
      break;
    }
    case KeyEvent::KEY_3: {
      if (in_main_menu_) {
        exit(0);
      }
      break;
    }
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE: {
      engine_.AttemptJump();
      break;
    }
    case KeyEvent::KEY_ESCAPE: {
      if (!in_main_menu_) {
        if (paused_) {
          paused_ = false;
          animation_started_ = false;
          t_ = 0;
          SetUpAnimation();
        } else {
          paused_ = true;
          t_ = 0;
          SetUpAnimation();
        }
      }
      break;
    }
  }
}

void MyApp::keyUp(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE: {
      engine_.DontJump();
      break;
    }
  }
}

Color MyApp::PercentFade(Color col) const {
  if (!engine_.EndReached()) {
    return col;
  }

  using std::chrono::milliseconds;
  const double elapsed_time =
      duration_cast<milliseconds>(system_clock::now() - time_of_end_reached_)
          .count();
  const double countdown_time = milliseconds(kCountdownTime).count();
  const double percentage = elapsed_time / countdown_time;
  return col * (1 - percentage);
}

bool MyApp::TimeToPrintPauseScreen() const {
  return animation_started_ && t_ >= kPauseScreenPrintTime;
}

bool MyApp::FadeEnded() const {
  Color col = PercentFade(backgr_colors[0]);
  if (col.r <= 0 && col.g <= 0 && col.b <= 0) {
    int x = 0;  // THIS IS GETTING CALLED IMMEDIATELY WHEN THE END IS REACHED WHEN
                // IT SHOULD WAIT FOR THE COUNTDOWN TO BE OVER
  }
  return col.r <= 0 && col.g <= 0 && col.b <= 0;
}

bool MyApp::AnimationEnded() {
  return animation_started_ && t_ >= kAnimationEndTime;
}

}  // namespace myapp

// TODO: music

// TODO: animations with Coreograph (???)

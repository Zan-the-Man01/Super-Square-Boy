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

namespace supersquareboy {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::TextBox;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;
using game::Location;

using cinder::app::KeyEvent;

const seconds kCountdownTime = seconds(6);
const std::string kNormalFont = "Azonix";
const std::vector<Color> player_colors = {Color(1, 0.501, 0)};
const std::vector<Color> backgr_colors = {Color(0, 0.933, 0.921),
                                          Color(0, 0.933, 0.921),
                                          Color(0.72941, 0.45098, 1)};
const double kPauseScreenPrintTime = 0.0075;
const double kAnimationEndTime = 0.01092;
const int kEndMusicPos = 3;
const int kDeathSoundPos = 4;

SuperSquareBoy::SuperSquareBoy() : speed_{50}, tile_size_{40} {}

void SuperSquareBoy::setup() {
  SetUpSound();
  SetUpAnimation();
}

void SuperSquareBoy::SetUpAnimation() {
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

void SuperSquareBoy::SetUpSound() {
  try {
    cinder::audio::SourceFileRef sourceFile =
        cinder::audio::load(cinder::app::loadAsset("menu.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));

    sourceFile =
        cinder::audio::load(cinder::app::loadAsset("music1.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));

    sourceFile =
        cinder::audio::load(cinder::app::loadAsset("music2.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));

    sourceFile =
        cinder::audio::load(cinder::app::loadAsset("end.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));

    sourceFile =
        cinder::audio::load(cinder::app::loadAsset("death.mp3"));
    sound_tracks_.push_back(cinder::audio::Voice::create(sourceFile));

    StartMusic(0);
  } catch (const std::exception& ex) {
    exit(1);
  }
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

void SuperSquareBoy::update() {
  timeline_.step( ch::Time(anim_time_));
  if (animation_started_) {
    anim_time_ += 0.0001;
  }

  if (in_main_menu_ || paused_ || in_level_select_ || in_credits_screen_) {
    return;
  }

  const auto time = system_clock::now();
  if (!end_reached_ && engine_.EndReached()) {
    time_of_end_reached_ = time;
    end_reached_ = true;
    StartMusic(kEndMusicPos);
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

  if (just_reset_) {
    StartMusic(current_level_);
    just_reset_ = false;
  }
}

void SuperSquareBoy::draw() {
  if (in_main_menu_) {
    DrawMainMenu();
    return;
  }

  if (in_level_select_) {
    DrawLevelSelect();
    return;
  }

  if (in_credits_screen_) {
    DrawCreditsScreen();
    return;
  }

  if (paused_) {
    if (!animation_started_) {
      animation_started_ = true;
      anim_time_ = 0;
      SetUpAnimation();
    }

    if (TimeToPrintPauseScreen()) {
      cinder::gl::clear(backgr_colors[current_level_]);
      DrawPauseScreen();
    }

    if (!AnimationEnded()) {
      if (!TimeToPrintPauseScreen()) {
        cinder::gl::clear(backgr_colors[current_level_]);
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
      StartMusic(kDeathSoundPos);
      time_of_death_ = time;

    } else if (time - time_of_death_ > std::chrono::seconds(1)) {
      engine_.Reset();
      just_reset_ = true;
      just_died_ = true;
      num_attempts++;
    }
  }

  cinder::gl::clear();
  DrawBackground();
  DrawAttemptsCounter();
  DrawPlayer();
  DrawSquares();
  DrawSpikes();
}

void SuperSquareBoy::DrawBackground() const {
  cinder::gl::clear(FadedColor(backgr_colors[current_level_]));
}

void SuperSquareBoy::DrawPlayer() const {
  if (engine_.IsDead()) {
    cinder::gl::color(FadedColor(backgr_colors[current_level_]));
  } else {
    cinder::gl::color(FadedColor(player_colors[0]));
  }

  const Location loc = engine_.GetPlayerSquare().GetLocation();
  cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.X(),
                                  tile_size_ * loc.Y(),
                                  tile_size_ * loc.X() + tile_size_,
                                  tile_size_ * loc.Y() + tile_size_));
}

void SuperSquareBoy::DrawSquares() const {
  cinder::gl::color(Color::black());
  const std::vector<Location> locs = engine_.GetFrame().GetSquareLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidRect(Rectf(tile_size_ * loc.X(),
                                    tile_size_ * loc.Y(),
                                    tile_size_ * loc.X() + tile_size_,
                                    tile_size_ * loc.Y() + tile_size_));
  }
}

void SuperSquareBoy::DrawSpikes() const {
  cinder::gl::color(Color(0.5, 0.5, 0.5));
  const std::vector<Location> locs = engine_.GetFrame().GetSpikeLocs();

  for (const auto& loc : locs) {
    cinder::gl::drawSolidTriangle(
        cinder::vec2(tile_size_ * loc.X(),tile_size_ * loc.Y() + tile_size_),
        cinder::vec2(tile_size_ * loc.X() + tile_size_,tile_size_ * loc.Y() + tile_size_),
        cinder::vec2(tile_size_ * loc.X() + (tile_size_ / 2),
            tile_size_ * loc.Y()));
  }
}

void SuperSquareBoy::DrawEndScreen() const {
  cinder::gl::clear(Color::black());

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];


  PrintText("LEVEL COMPLETE", color, size, center, 100);
}

void SuperSquareBoy::DrawPauseScreen() const {
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

void SuperSquareBoy::DrawMainMenu() const {
  cinder::gl::clear(backgr_colors[current_level_]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;

  PrintText("SUPER SQUARE BOY", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: LEVEL SELECT", color, size, center, 50);
  PrintText("2: CREDITS", color, size,
            {center.x, center.y + small_y_offset}, 50);
  PrintText("3: EXIT", color, size,
            {center.x, center.y + (2 * small_y_offset)}, 50);
}

void SuperSquareBoy::DrawLevelSelect() const {
  cinder::gl::clear(backgr_colors[current_level_]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 75;
  const float esc_y_offset = 50;
  const float left_x_offset = 500;


  PrintText("SUPER SQUARE BOY", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("1: LEVEL ONE", color, size, center, 50);
  PrintText("2: LEVEL TWO", color, size,
            {center.x, center.y + small_y_offset}, 50);
  PrintText("[ESC]: BACK", color, size,
            {center.x - left_x_offset, center.y + (7.5 * esc_y_offset)}, 30);
}

void SuperSquareBoy::DrawCreditsScreen() const {
  cinder::gl::clear(backgr_colors[current_level_]);

  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = player_colors[0];
  const float main_y_offset = 150;
  const float small_y_offset = 50;
  const float left_x_offset = 500;

  PrintText("SUPER SQUARE BOY", color, size, {center.x, center.y - main_y_offset}, 100);
  PrintText("CREATED BY ALEXANDER MARCOZZI", color, size,
      {center.x, center.y - small_y_offset}, 40);
  PrintText("EMAIL: alex.marcozzi1@gmail.com", color, size,
            center, 40);
  PrintText("MUSIC", color, size,
            {center.x, center.y + (2 * small_y_offset)}, 40);

  PrintText("MAIN MENU: 'AWAY WITH THE FAIRIES' - PHILANTHROPE", color, size,
            {center.x, center.y + (3 * small_y_offset)}, 40);

  PrintText("LEVEL 1: RPM - ENV", color, size,
            {center.x, center.y + (4 * small_y_offset)}, 40);

  PrintText("LEVEL 2: DEEP BLUE - K-391", color, size,
            {center.x, center.y + (5 * small_y_offset)}, 40);

  PrintText("LEVEL 3: [PLACEHOLDER] - [ARTIST]", color, size,
            {center.x, center.y + (6 * small_y_offset)}, 40);

  PrintText("LEVEL 4: [PLACEHOLDER] - [ARTIST]", color, size,
            {center.x, center.y + (7 * small_y_offset)}, 40);

  PrintText("[ESC]: BACK", color, size,
            {center.x - left_x_offset, center.y + (7.5 * small_y_offset)}, 30);
}

void SuperSquareBoy::DrawAttemptsCounter() const {
  const cinder::vec2 center = getWindowCenter();
  const cinder::ivec2 size = {1000, 100};
  const Color color = FadedColor(player_colors[0]);
  const float y_offset = 300;

  PrintText("ATTEMPT " + std::to_string(num_attempts), color, size,
            {center.x, center.y - y_offset}, 80);
}

void SuperSquareBoy::SelectLevel(int level_num) {
  current_level_ = level_num;
  in_level_select_ = false;
  engine_.StartLevel(current_level_);
  paused_ = false;
  animation_started_ = false;
  anim_time_ = 0;
  SetUpAnimation();
}

void SuperSquareBoy::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_1: {
      if (in_main_menu_) {
        in_main_menu_ = false;
        in_level_select_ = true;
      } else if (in_level_select_) {
        SelectLevel(1);
      } else if (paused_) {
        Unpause();
      }

      break;
    }
    case KeyEvent::KEY_2: {
      if (in_main_menu_) {
        in_main_menu_ = false;
        in_credits_screen_ = true;
      } else if (in_level_select_) {
        SelectLevel(2);
      } else if (paused_) {
        Reset();
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
      if (in_level_select_) {
        in_level_select_ = false;
        in_main_menu_ = true;
      } else if (in_credits_screen_) {
        in_credits_screen_ = false;
        in_main_menu_ = true;
      }

      if (!in_main_menu_) {
        if (paused_) {
          Unpause();
        } else {
          Pause();
        }
      }
      break;
    }
  }
}

void SuperSquareBoy::keyUp(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_SPACE: {
      engine_.DontJump();
      break;
    }
  }
}

Color SuperSquareBoy::FadedColor(Color col) const {
  if (!engine_.EndReached() || !end_reached_) {
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

bool SuperSquareBoy::TimeToPrintPauseScreen() const {
  return animation_started_ && anim_time_ >= kPauseScreenPrintTime;
}

bool SuperSquareBoy::FadeEnded() const {
  Color col = FadedColor(backgr_colors[current_level_]);
  return col.r <= 0 && col.g <= 0 && col.b <= 0;
}

bool SuperSquareBoy::AnimationEnded() {
  return animation_started_ && anim_time_ >= kAnimationEndTime;
}

void SuperSquareBoy::Reset() {
  current_level_ = 0;
  just_died_ = true;
  just_reset_ = true;
  end_reached_ = false;
  paused_ = false;
  in_main_menu_ = true;
  in_level_select_ = false;
  in_credits_screen_ = false;
  animation_started_ = false;
  engine_.Reset();
  StartMusic(0);
}

void SuperSquareBoy::Pause() {
  sound_tracks_[current_level_]->pause();
  paused_ = true;
  anim_time_ = 0;
  SetUpAnimation();
}

void SuperSquareBoy::Unpause() {
  sound_tracks_[current_level_]->start();
  paused_ = false;
  animation_started_ = false;
  anim_time_ = 0;
  SetUpAnimation();
}

void SuperSquareBoy::StartMusic(int level_num) {
  for (auto& track : sound_tracks_) {
    track->stop();
  }

  sound_tracks_[level_num]->start();
}

}  // namespace myapp

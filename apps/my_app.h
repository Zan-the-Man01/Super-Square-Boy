// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <choreograph/Choreograph.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <game/engine.h>

namespace myapp {

/**
 * Class that plays the game Super Square Boy.
 */
class MyApp : public cinder::app::App {
 public:

  /**
   * Default constructor.
   */
  MyApp();

  /**
   * Sets up the game to be played.
   */
  void setup() override;

  /**
   * Updates the game on a frame by frame basis
   */
  void update() override;

  /**
   * Draws the game in its current state.
   */
  void draw() override;

  /**
   * Detects and handles the keys the player is pressing.
   */
  void keyDown(cinder::app::KeyEvent) override;

  /**
   * Detects and handles the keys the player is not pressing.
   */
  void keyUp(cinder::app::KeyEvent) override;

 private:

  /**
   * Sets up the animation used for the pause screen.
   */
  void SetUpAnimation();

  /**
   * Draws the background of the game.
   */
  void DrawBackground() const;

  /**
   * Draws the player at their current position.
   */
  void DrawPlayer() const;

  /**
   * Draws the squares at their current positions.
   */
  void DrawSquares() const;

  /**
   * Draws the spikes at their current positions.
   */
  void DrawSpikes() const;

  /**
   * Draws the end screen.
   */
  void DrawEndScreen() const;

  /**
   * Draws the pause screen.
   */
  void DrawPauseScreen() const;

  /**
   * Draws the main menu.
   */
  void DrawMainMenu() const;

  void DrawLevelSelect() const;

  void DrawCreditsScreen() const;

  void SelectLevel(int level_num);

  /**
   * Gets the correctly faded color based on how long the fade has lasted.
   * @param col the color to fade.
   * @return
   */
  cinder::Color FadedColor(cinder::Color col) const;

  /**
   * Gets the time in the animation when the pause screen should be printed.
   *
   * @return true if it is time to print, else false.
   */
  bool TimeToPrintPauseScreen() const;

  /**
   * Gets if the fade has ended.
   *
   * @return true if the fade has ended, else false.
   */
  bool FadeEnded() const;

  /**
   * Gets if the animation has ended.
   *
   * @return true if the animation has ended, else false.
   */
  bool AnimationEnded();

  /**
   * Resets the game state.
   */
  void Reset();

  void Pause();

  void Unpause();

  void StartMusic(int level_num);

  void StopMusic(int level_num);

  void PauseMusic(int level_num);

 private:
  /** The engine that plays a level of the game. */
  game::Engine engine_;

  /** The size of a tile. */
  const size_t tile_size_;

  /** The speed of the game. */
  const size_t speed_;

  /** The last time the engine took a step. */
  std::chrono::time_point<std::chrono::system_clock> last_time_;

  /** The time the player died. */
  std::chrono::time_point<std::chrono::system_clock> time_of_death_;

  /** The time the player reached the end. */
  std::chrono::time_point<std::chrono::system_clock> time_of_end_reached_;

  /** If the player has just died. */
  bool just_died_ = true;

  /** If the game has just been reset. */
  bool just_reset_ = true;

  /** If the player has reached the end. */
  bool end_reached_ = false;

  /** If the game is paused. */
  bool paused_ = false;

  /** If the player is in the main menu.. */
  bool in_main_menu_ = true;

  bool in_level_select_ = false;

  bool in_credits_screen_ = false;

  /** If the animation has started. */
  bool animation_started_ = false;

  /** The sound tracks for each level. */
  std::vector<cinder::audio::VoiceRef> sound_tracks_;

  /** The timeline to handle animations. */
  ch::Timeline timeline_;

  /** One side of the animated rectangle. */
  ch::Output<ci::vec2> _position_a;

  /** The other side of the animated rectangle. */
  ch::Output<ci::vec2> _position_b;

  /** The time position of the animation (between 0 and 1). */
  double anim_time_ = 0;

  /** The current level being played */
  int current_level_ = 0;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

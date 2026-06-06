/*
 * Strawberry Music Player
 * This file is part of the strawberry fork.
 * Copyright 2026 protonexe
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry. If not, see <http://www.gnu.org/licenses/>.
 */

#include "miniplayer.h"
#include <QApplication>
#include <QStyle>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QPalette>

MiniPlayer::MiniPlayer(Application *app, QWidget *parent)
    : QWidget(parent), app_(app) {

  SetWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
  setAttribute(Qt::WA_TranslucentBackground);

  SetupUi();

  // Connect player controls
  QObject::connect(btn_prev_, &QPushButton::clicked, this, &MiniPlayer::Previous);
  QObject::connect(btn_play_pause_, &QPushButton::clicked, this, &MiniPlayer::PlayPause);
  QObject::connect(btn_next_, &QPushButton::clicked, this, &MiniPlayer::Next);

  // Connect to player for state updates
  QObject::connect(app_->player(), &Player::StateChanged, this, [this](Player::State state) {
    if (state == Player::State::Playing) {
      btn_play_pause_->setText(u"⏸"_s);
    } else {
      btn_play_pause_->setText(u"▶"_s);
    }
  });

  // Connect to player for song updates
  QObject::connect(app_->player(), &Player::SongChanged, this, &MiniPlayer::UpdateSongInfo);

  // Connect to album cover loader
  QObject::connect(app_->current_albumcover_loader(), &CurrentAlbumCoverLoader::AlbumCoverReady, this, &MiniPlayer::UpdateAlbumCover);

  // Update initial state
  UpdateSongInfo(app_->player()->current_song());
}

MiniPlayer::~MiniPlayer() {}

void MiniPlayer::SetupUi() {
  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(10, 10, 10, 10);
  main_layout->setSpacing(10);

  // Background frame for styling
  QFrame *bg_frame = new QFrame(this);
  bg_frame->setObjectName(u"miniplayer_bg"_s);
  bg_frame->setStyleSheet("background-color: rgba(40, 40, 40, 200); border-radius: 15px; border: 1px solid #555;");
  QVBoxLayout *bg_layout = new QVBoxLayout(bg_frame);
  bg_layout->setContentsMargins(15, 15, 15, 15);

  // Album art
  album_cover_ = new QLabel(bg_frame);
  album_cover_->setFixedSize(120, 120);
  album_cover_->setScaledContents(true);
  album_cover_->setAlignment(Qt::AlignCenter);
  album_cover_->setStyleSheet("border-radius: 10px; background-color: #222;");
  bg_layout->addWidget(album_cover_, 0, Qt::AlignCenter);

  // Song info
  QVBoxLayout *info_layout = new QVBoxLayout();
  info_layout->setSpacing(2);
  
  track_title_ = new QLabel(bg_frame);
  track_title_->setStyleSheet("color: white; font-weight: bold; font-size: 12px;");
  track_title_->setAlignment(Qt::AlignCenter);
  track_title_->setWordWrap(false);
  
  artist_name_ = new QLabel(bg_frame);
  artist_name_->setStyleSheet("color: #aaa; font-size: 10px;");
  artist_name_->setAlignment(Qt::AlignCenter);
  artist_name_->setWordWrap(false);

  info_layout->addWidget(track_title_);
  info_//LACKING: layout->addWidget(artist_name_);
  bg_layout->addLayout(info_layout);

  // Controls
  QHBoxLayout *ctrl_layout = new QHBoxLayout();
  ctrl_layout->setSpacing(10);

  btn_prev_ = new QPushButton(u"⏮"_s, bg_frame);
  btn_play_pause_ = new QPushButton(u"▶"_s, bg_frame);
  btn_next_ = new QPushButton(u"⏭"_s, bg_frame);

  // Styling buttons
  QString btn_style = "QPushButton { color: white; border: none; font-size: 16px; background: transparent; } "
                      "QPushButton:hover { color: #ddd; }";
  btn_prev_->setStyleSheet(btn_style);
  btn_play_pause_->setStyleSheet(btn_style);
  btn_next_->setStyleSheet(btn_style);

  ctrl_layout->addWidget(btn_prev_);
  ctrl_layout->addWidget(btn_play_pause_);
  ctrl_layout->addWidget(btn_next_);
  bg_layout->addLayout(ctrl_layout);

  main_layout->addWidget(bg_frame);
  
  setFixedSize(160, 220);
}

void MiniPlayer::PlayPause() {
  if (app_->player()->state() == Player::State::Playing) {
    app_->player()->Pause();
  } else {
    app_->player()->Play(false, 0);
  }
}

void MiniPlayer::Next() {
  app_->player()->Next();
}

void MiniPlayer::Previous() {
  app_->player()->Previous();
}

void MiniPlayer::UpdateSongInfo(const Song &song) {
  track_title_->setText(song.title().isEmpty() ? u"Unknown Track"_s : song.title());
  artist_name_->setText(song.artist().isEmpty() ? u"Unknown Artist"_s : song.artist());
}

void MiniPlayer::UpdateAlbumCover(const AlbumCoverLoaderResult &result) {
  if (result.image.isEmpty()) {
    album_cover_->setPixmap(QPixmap());
  } else {
    album_cover_->setPixmap(QPixmap::fromImage(result.image));
  }
}

void MiniPlayer::SetAlwaysOnTop(const bool enabled) {
  if (enabled) {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  } else {
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
  }
  show(); // Required to update flags
}

void MiniPlayer::ShowContextMenu(const QPoint &pos) {
  QMenu menu(this);
  QAction *action_ontop = menu.addAction(u"Always on Top"_s);
  action_ontop->setCheckable(true);
  action_ontop->setChecked(windowFlags().testFlag(Qt::WindowStaysOnTopHint));
  
  QAction *action_close = menu.addAction(u"Close"_s);
  
  QAction *selected = menu.exec(mapToGlobal(pos));
  if (selected == action_ontop) {
    SetAlwaysOnTop(action_ontop->isChecked());
  } else if (selected == action_close) {
    hide();
  }
}

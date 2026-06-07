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
#include <QAction>
#include <QMenu>
#include <QPixmap>
#include <QStyle>

MiniPlayer::MiniPlayer(Application *app, QWidget *parent)
    : QWidget(parent, Qt::Window | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool),
      app_(app) {

  SetupUi();

  QObject::connect(btn_prev_, &QPushButton::clicked, this, &MiniPlayer::Previous);
  QObject::connect(btn_play_pause_, &QPushButton::clicked, this, &MiniPlayer::PlayPause);
  QObject::connect(btn_next_, &QPushButton::clicked, this, &MiniPlayer::Next);
}

MiniPlayer::~MiniPlayer() {}

void MiniPlayer::SetupUi() {
  setFixedSize(160, 220);

  QVBoxLayout *main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(10, 10, 10, 10);
  main_layout->setSpacing(10);

  album_cover_ = new QLabel(this);
  album_cover_->setFixedSize(120, 120);
  album_cover_->setScaledContents(true);
  album_cover_->setAlignment(Qt::AlignCenter);
  main_layout->addWidget(album_cover_, 0, Qt::AlignCenter);

  track_title_ = new QLabel(this);
  track_title_->setAlignment(Qt::AlignCenter);
  main_layout->addWidget(track_title_);

  artist_name_ = new QLabel(this);
  artist_name_->setAlignment(Qt::AlignCenter);
  main_layout->addWidget(artist_name_);

  QHBoxLayout *ctrl_layout = new QHBoxLayout();
  ctrl_layout->setSpacing(10);

  btn_prev_ = new QPushButton(this);
  btn_play_pause_ = new QPushButton(this);
  btn_next_ = new QPushButton(this);

  ctrl_layout->addWidget(btn_prev_);
  ctrl_layout->addWidget(btn_play_pause_);
  ctrl_layout->addWidget(btn_next_);
  main_layout->addLayout(ctrl_layout);
}

void MiniPlayer::PlayPause() {
  // Placeholder
}

void MiniPlayer::Next() {
  // Placeholder
}

void MiniPlayer::Previous() {
  // Placeholder
}

void MiniPlayer::UpdateSongInfo(const Song &song) {
  Q_UNUSED(song);
}

void MiniPlayer::UpdateAlbumCover(const AlbumCoverLoaderResult &result) {
  Q_UNUSED(result);
}

void MiniPlayer::SetAlwaysOnTop(const bool enabled) {
  Q_UNUSED(enabled);
}

void MiniPlayer::ShowContextMenu(const QPoint &pos) {
  Q_UNUSED(pos);
}

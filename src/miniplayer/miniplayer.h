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

#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>

#include "includes/shared_ptr.h"
#include "core/application.h"
#include "core/player.h"
#include "core/song.h"
#include "covermanager/albumcoverloaderresult.h"

class MiniPlayer : public QWidget {
  Q_OBJECT

 public:
  explicit MiniPlayer(Application *app, QWidget *parent = nullptr);
  ~MiniPlayer() override;

  void SetAlwaysOnTop(const bool enabled);

 private slots:
  void PlayPause();
  void Next();
  void Previous();
  void UpdateSongInfo(const Song &song);
  void UpdateAlbumCover(const AlbumCoverLoaderResult &result);
  void ShowContextMenu(const QPoint &pos);

 private:
  void SetupUi();

  Application *app_;
  QLabel *album_cover_;
  QLabel *track_title_;
  QLabel *artist_name_;
  QPushButton *btn_prev_;
  QPushButton *btn_play_pause_;
  QPushButton *btn_next_;
};

#endif  // MINIPLAYER_H

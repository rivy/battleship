/*
 * Battleship, classic battleship game
 * Copyright (C) 2014-2015 Pavel Dolgov
 *
 * See the LICENSE file for terms of use.
 */

#include "GameController.hpp"
#include "getShipCoordinates.hpp"

static int getEnemysNumber(int player_number) {
    return 3 - player_number;
}

GameController* GameController::make(GameDesk* desk) {
    if (desk == NULL) {
        throw Exception("Received NULL pointer to "
                        "GameDesk when try to create "
                        "GameController");
    }
    GameController* controller = new GameController();
    controller->desk_ = desk;
    return controller;
}

void GameController::makeMove(int player_number,
                              const Point& point) {
    int enemy = getEnemysNumber(player_number);
    if (desk_->getVisibility(point, enemy)) {
        throw Exception("Some of players wants to shoot "
                        "at cell which has already shot "
                        "down");
    }
    desk_->setVisibility(point, true, enemy);
    if (desk_->getCellState(point, enemy)) {
        Points pts = getShipCoordinates(*desk_, point,
                                        enemy);
        if (checkBurst(pts, enemy)) {
            int beginning = (pts.is_horizontal) ?
                            pts.p1.row : pts.p1.col;
            int end = (pts.is_horizontal) ?
                            pts.p2.row : pts.p2.col;
            for (int i = beginning; i <= end; i++) {
                Point pt;
                pt.col = (pts.is_horizontal) ?
                         pts.p1.col : i;
                pt.row = (pts.is_horizontal) ? i :
                         pts.p1.row;
                desk_->setFlooding(pt, true, enemy);
            }
        }
    }
}

GameController::GameController() {
}

bool GameController::checkBurst(const Points& ship,
                                int player_number) const {
    int beginning = (ship.is_horizontal) ? ship.p1.row :
                    ship.p1.col;
    int end = (ship.is_horizontal) ? ship.p2.row :
              ship.p2.col;
    for (int i = beginning; i <= end; i++) {
        Point pt;
        pt.col = (ship.is_horizontal) ? ship.p1.col : i;
        pt.row = (ship.is_horizontal) ? i : ship.p1.row;
        if (!desk_->getVisibility(pt, player_number)) {
            return false;
        }
    }
    return true;
}

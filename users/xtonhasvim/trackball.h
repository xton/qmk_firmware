/* Copyright 2015-2017 Christon DeWan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/*
 * must define: 
 *
 * TRACKBALL_ENABLED // enable support
 *
 * // the line for each direction (numeric part of pin)
 * TB_LINE_UP
 * TB_LINE_DN
 * TB_LINE_LT
 * TB_LINE_RT
 *
 * //the pad for each direction (a letter)
 * TB_PAD_UP
 * TB_PAD_DN
 * TB_PAD_LT
 * TB_PAD_RT
 *
 */

void matrix_scan_trackball(void);
void matrix_init_trackball(void);
bool process_record_trackball(uint16_t keycode, keyrecord_t *record);

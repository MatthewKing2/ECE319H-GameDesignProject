/*
 * MenuScreens.h
 *
 *  Created on: Apr 19, 2024
 *      Author: simon
 */

#ifndef MENUSCREENS_H_
#define MENUSCREENS_H_

void refreshStart(int currentChoice, bool spanish);
void startMenu(bool* spanish);
void deathScreen(bool spanish);
void endScreen(/*Player p1, Player p2, Player p3, Player p4,*/ bool spanish);
void winScreen(bool spanish);


#endif /* MENUSCREENS_H_ */

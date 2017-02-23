/*
 * stopwatch.c
 *
 *  Created on: 2016/11/20
 *      Author: Sin Shimozono
 */

#include "stopwatch.h"

void stopwatch_start(stopwatch * w) {
	w->start_clock = clock();
	w->start_time = time(NULL);
}

void stopwatch_stop(stopwatch * w) {
	w->stop_clock = clock();
	w->stop_time = time(NULL);
	w->secs = difftime(w->start_time, w->stop_time);
	w->clocks = w->stop_clock - w->start_clock;
	w->millis = (w->clocks)/(CLOCKS_PER_SEC / 1000);
}

void stopwatch_lap(stopwatch * w) {
	stopwatch_stop(w);
}

void stopwatch_reset(stopwatch * w) {
	stopwatch_start(w);
}

unsigned long stopwatch_secs(stopwatch * w) {
	return w->secs;
}

unsigned long stopwatch_clocks(stopwatch * w) {
	return w->clocks;
}


unsigned long stopwatch_millis(stopwatch * w) {
	return w->millis;
}


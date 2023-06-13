//
// Created by Roman Braunstingl on 13.06.23.
//

#ifndef BIFIT_BIFIT_GC_H
#define BIFIT_BIFIT_GC_H

#include "bifit_gc_common.h"
#include "bifit_gc_mark.h"
#include "bifit_gc_sweep.h"

void bifit_gc_run() {
    bifit_gc_mark();
    bifit_gc_sweep();
}

#endif //BIFIT_BIFIT_GC_H

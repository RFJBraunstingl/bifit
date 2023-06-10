//
// Created by Roman Braunstingl on 10.06.23.
//

#ifndef BIFIT_BIFIT_NATIVE_CLASSES_H
#define BIFIT_BIFIT_NATIVE_CLASSES_H

#include "bifit_native_classes_common.h"
#include "System.h"
#include "PrintStream.h"
#include "String.h"

void bifit_native_classes_bind() {
    bifit_native_classes_put(bifit_native_class_System_init());
    bifit_native_classes_put(bifit_native_class_PrintStream_init());
    bifit_native_classes_put(bifit_native_class_String_init());
}

#endif //BIFIT_BIFIT_NATIVE_CLASSES_H

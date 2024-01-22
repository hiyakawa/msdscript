/**
 * \file cmdline.h
 * \brief Declarations of use_arguments
 * \author Laura Zhang
 */

#pragma once

typedef enum {
    do_nothing,
    do_interp,
    do_print,
    do_pretty_print,
} run_mode_t;

int use_arguments(int argc, char **argv);

/**
 * @file    error.h
 * @author  Luís Tavares
 * @author  Márcio Duarte
 * @brief   Defines the possible errors.
 * @see     ../README.md
 * 
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * @brief Error related with the arguments passed when calling the program.
 * 
 */
#define ARGS_ERROR 1
/**
 * @brief Error related to directories.
 * 
 */
#define DIR_ERROR 2
/**
 * @brief Error related to pipes.
 * 
 */
#define PIPE_ERROR 3
/**
 * @brief Error related to forks.
 * 
 */
#define FORK_ERROR 4

#endif
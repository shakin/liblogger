/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
  Copyright (C) 2011 Alexey Veretennikov (alexey dot veretennikov at gmail.com)
 
  This file is part of liblogger.

  liblogger is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  liblogger is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with liblogger.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>

#include "logger.h"

int logger_log_level = LOG_LEVEL_ALL;
int logger_output_socket = 0;

#define LOGGER_MAX_HEADER_SIZE 512
char logger_log_header[LOGGER_MAX_HEADER_SIZE+1];
char logger_log_message[LOGGER_MAX_ENTRY_SIZE+1];

static void logger_write_private(const char* name,
                                 int entry_type,
                                 const char* format,
                                 va_list args)
{
  const char* header_format;
  switch(entry_type)
  {
  case LOG_ENTRY_WARNING:
    header_format = "[%s][%s][%p] /warning: %s\n";
    break;
  case LOG_ENTRY_ERROR:
    header_format = "[%s][%s][%p] /error: %s\n";
    break;
  case LOG_ENTRY_NORMAL:
  default:
    header_format = "[%s][%s][%p]: %s\n";
    break;
  case LOG_ENTRY_INFO:
		header_format = "[%s][%s][%p]: /info: %s\n";
		break;
  }
  snprintf(logger_log_header,
           LOGGER_MAX_HEADER_SIZE,
           header_format,
           name,
           "29 Sep 2011",
           "0xDEADEAD",
           format);
  vsnprintf(logger_log_message,
            LOGGER_MAX_ENTRY_SIZE,
            logger_log_header,
            args);
  printf("%s",logger_log_message);
}


void logger_set_log_level(int level)
{
  assert(level >= LOG_LEVEL_ERRORS && level <= LOG_LEVEL_ALL);
  logger_log_level = level;
}

void logger_init()
{
  logger_output_socket = 0;
}

void logger_fini()
{
  if (logger_output_socket)
    close(logger_output_socket);
  logger_output_socket = 0;
}



void logger_write(const char* name,int entry_type, const char* format, ...)
{
  if (entry_type <= logger_log_level)
  {
    va_list vl;
    va_start(vl, format);
    logger_write_private(name, entry_type, format, vl);
    va_end(vl);
  }
}

